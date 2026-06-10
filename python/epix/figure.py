"""Figure capture + the ``figure()`` context manager over the bindings (``_epix``).

ePiX is global-state: ``picture()``/``begin()`` set up, drawing functions mutate
the active picture, then we capture it via ``print_eepic`` and render inline.

    import epix
    epix.picture(epix.P(-1, -1), epix.P(1, 1), "2x1in")
    epix.begin()
    epix.plot(lambda t: epix.P(t, t * t), -1, 1)
    fig = epix.render()           # -> Figure (displays inline in Jupyter)

    with epix.figure(epix.P(-1, -1), epix.P(1, 1), "2x1in") as fig:
        epix.label(epix.P(0, 0), "Hello, world!")
    fig                           # displays inline

The scene API (P, picture, begin, plot, label, pen, fill, line, colors, …) comes
straight from the `_epix` bindings; this module just adds capture + render.
One figure at a time (matches ePiX's global model) — fine for notebooks.
"""

from __future__ import annotations

import base64
import os
import subprocess
import tempfile
from contextlib import contextmanager

from . import _epix
from .render import Figure, render_eepic


def render(dpi: int = 150) -> Figure:
    """Capture the current ePiX picture (via ``print_eepic``) and render to PNG."""
    fd, path = tempfile.mkstemp(suffix=".eepic")
    os.close(fd)
    try:
        _epix.print_eepic(path)
        return render_eepic(path, dpi=dpi)
    finally:
        os.unlink(path)


class _Pending:
    """Yielded by ``figure()``; becomes the rendered Figure after the block."""

    figure: Figure | None = None

    def _repr_png_(self):
        return self.figure._repr_png_() if self.figure is not None else None

    @property
    def png(self):
        return None if self.figure is None else self.figure.png

    @property
    def eepic(self):
        return None if self.figure is None else self.figure.eepic

    def save(self, path):
        self.figure.save(path)


@contextmanager
def figure(sw, ne, size, dpi: int = 150):
    """Context manager: sets up the picture, captures + renders it on exit."""
    _epix.picture(sw, ne, size)
    _epix.begin()
    pending = _Pending()
    yield pending
    pending.figure = render(dpi=dpi)


class Animation:
    """A rendered animation: a gif (shown inline in Jupyter) + the per-frame Figures."""

    def __init__(self, gif: bytes, frames: list[Figure]):
        self.gif = gif
        self.frames = frames

    def _repr_html_(self):
        b64 = base64.b64encode(self.gif).decode("ascii")
        return f'<img src="data:image/gif;base64,{b64}"/>'

    def save(self, path: str) -> None:
        with open(path, "wb") as f:
            f.write(self.gif)


def animate(build, count: int = 24, dpi: int = 100, delay: int = 8) -> Animation:
    """Render an animation by calling ``build()`` once per frame.

    Mirrors ePiX's ``.flx`` model: frame ``i`` is rendered with the ``tix()``
    counter set to ``i / count`` (a parameter in ``[0, 1)``). ``build()`` sets up
    the whole frame (``picture()``/``begin()`` + drawing, reading ``epix.tix()``).

    Each frame's scene is built in a forked child so it gets a *fresh* copy of
    ePiX's global state — ePiX accumulates per-render state (the color palette,
    etc.) with no public reset, exactly as `flix` works around by running a new
    process per frame. The parent (clean state) then renders each eepic to PNG and
    assembles a looping gif via ImageMagick ``convert``.
    """
    frames: list[Figure] = []
    with tempfile.TemporaryDirectory() as d:
        eepics = []
        for i in range(count):
            ep = os.path.join(d, f"f{i:04d}.eepic")
            pid = os.fork()
            if pid == 0:  # child: isolated ePiX state
                try:
                    _epix.set_tix(i / count)
                    build()
                    _epix.print_eepic(ep)
                    os._exit(0)
                except BaseException:
                    os._exit(1)
            _, status = os.waitpid(pid, 0)
            if status != 0 or not os.path.exists(ep):
                raise RuntimeError(f"animation frame {i} failed to render")
            eepics.append(ep)

        paths = []
        for i, ep in enumerate(eepics):  # parent renders eepic -> png
            fig = render_eepic(ep, dpi=dpi)
            frames.append(fig)
            p = os.path.join(d, f"f{i:04d}.png")
            fig.save(p)
            paths.append(p)
        gif_path = os.path.join(d, "anim.gif")
        subprocess.run(
            ["convert", "-loop", "0", "-delay", str(delay), *paths, gif_path],
            check=True,
            capture_output=True,
        )
        with open(gif_path, "rb") as f:
            gif = f.read()
    return Animation(gif, frames)
