"""Figure capture + the ``figure()`` context manager over the bindings (``_epix``).

ePiX is global-state: ``picture()``/``begin()`` set up, drawing functions mutate
the active picture, then we capture it via ``print_eepic`` and render inline.

    import epix
    epix.picture(epix.Point(-1, -1), epix.Point(1, 1), "2x1in")
    epix.begin()
    epix.plot(lambda t: epix.Point(t, t * t), -1, 1)
    fig = epix.render()           # -> Figure (displays inline in Jupyter)

    with epix.figure(epix.Point(-1, -1), epix.Point(1, 1), "2x1in") as fig:
        epix.label(epix.Point(0, 0), "Hello, world!")
    fig                           # displays inline

The scene API (P, picture, begin, plot, label, pen, fill, line, colors, …) comes
straight from the `_epix` bindings; this module just adds capture + render.
ePiX's drawing state is global; ``figure()`` resets it before each picture (and
``animate()`` before each frame), so figures drawn one after another in a process do
not affect each other.  Code that sets a picture up by hand (``bounding_box`` /
``picture`` / ``begin`` / ``render``) calls ``epix.reset_state()`` first for the same
guarantee.
"""

from __future__ import annotations

import base64
import os
import shutil
import subprocess
import tempfile
from contextlib import contextmanager

from . import _epix
from .render import Figure, render_eepic


def render(dpi: int = 150) -> Figure:
    """Capture the current ePiX picture (via ``print_eepic``) as a :class:`Figure`.

    The eepic is captured now; the PNG is rasterized lazily on first display and
    is best-effort. A few demos produce an enormous eepic (tens of thousands of
    colored facets) that exceeds LaTeX's memory when rasterized — those display
    as nothing rather than raising, so the notebook doesn't crash. Building the
    figure needs only libepix, which is what the byte-identity harness relies on.
    """
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
def figure(lower_left, upper_right, size, dpi: int = 150):
    """Context manager: sets up the picture, captures + renders it on exit.

    Each figure starts from ePiX's startup state (``reset_state()``): no pen, fill,
    line style, label style, palette, camera or output format carries over from an
    earlier figure in the same process, so a notebook may draw any number of them.
    """
    _epix.reset_state()
    _epix.picture(lower_left, upper_right, size)
    _epix.begin()
    pending: _Pending = _Pending()
    yield pending
    pending.figure = render(dpi=dpi)


@contextmanager
def activated(screen):
    """Context manager: activate ``screen`` for the duration of a ``with`` block.

    ePiX's ``activate``/``deactivate`` bracket a run of drawing calls that should
    land on a secondary ``screen`` (an inset panel, a separate view) instead of
    the main picture.  That bracket is exactly a context manager::

        with epix.activated(panel):
            epix.surface(F, R)
            epix.inset(panel, lower_left=..., upper_right=...)

    instead of the manual ``epix.activate(panel)`` ... ``epix.deactivate(panel)``
    pair.  Equivalent to that pair -- the same two calls in the same order, with
    ``deactivate`` guaranteed even if the block raises -- so figures stay
    byte-identical.
    """
    _epix.activate(screen)
    try:
        yield screen
    finally:
        _epix.deactivate(screen)


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
    assembles a looping gif via ImageMagick ``convert`` — when the rendering
    toolchain is present; on the lean image (no ``gs``/``convert``) the frames
    keep their eepic and the gif is empty.
    """
    frames: list[Figure] = []
    with tempfile.TemporaryDirectory() as d:
        eepics: list[str] = []
        for i in range(count):
            ep: str = os.path.join(d, f"f{i:04d}.eepic")
            pid: int = os.fork()
            if pid == 0:  # child: isolated ePiX state
                try:
                    # The fork copies whatever state the parent has accumulated
                    # (an earlier figure in the same notebook); start each frame
                    # from the startup state instead.
                    _epix.reset_state()
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

        paths: list[str] = []
        can_render: bool = all(shutil.which(t) for t in ("elaps", "gs", "convert"))
        for i, ep in enumerate(eepics):  # parent renders eepic -> png
            # The eepic is always captured (it's the correctness artifact); the
            # PNG is best-effort. A heavy frame (tens of thousands of facets) can
            # exceed LaTeX/ghostscript limits when rasterized, exactly like the
            # giant single figures render() guards against -- in that case the
            # frame's png is empty and it is dropped from the assembled gif.
            fig: Figure = render_eepic(ep, dpi=dpi)
            if can_render and fig.png:
                p: str = os.path.join(d, f"f{i:04d}.png")
                fig.save(p)
                paths.append(p)
            frames.append(fig)
        gif: bytes = b""
        if paths:
            gif_path: str = os.path.join(d, "anim.gif")
            subprocess.run(
                ["convert", "-loop", "0", "-delay", str(delay), *paths, gif_path],
                check=True,
                capture_output=True,
            )
            with open(gif_path, "rb") as f:
                gif = f.read()
    return Animation(gif, frames)
