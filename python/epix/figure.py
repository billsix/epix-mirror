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
from collections.abc import Callable, Generator, Sequence
from contextlib import contextmanager

from . import _epix
from .render import Figure, render_eepic


def render(dpi: int = 150, background: str | None = None) -> Figure:
    """Capture the current ePiX picture (via ``print_eepic``) as a :class:`Figure`.

    The eepic is captured now; the PNG is rasterized lazily on first display and
    is best-effort. A few demos produce an enormous eepic (tens of thousands of
    colored facets) that exceeds LaTeX's memory when rasterized — those display
    as nothing rather than raising, so the notebook doesn't crash. Building the
    figure needs only libepix, which is what the byte-identity harness relies on.
    ``background`` (``"#rrggbb"``) makes the PNG opaque in that colour; the default
    leaves it transparent (see :class:`~epix.render.Figure`).
    """
    fd, path = tempfile.mkstemp(suffix=".eepic")
    os.close(fd)
    try:
        _epix.print_eepic(path)
        return render_eepic(path, dpi=dpi, background=background)
    finally:
        os.unlink(path)


class _Pending:
    """Yielded by ``figure()``; becomes the rendered Figure after the block."""

    figure: Figure | None = None

    def _repr_png_(self) -> bytes | None:
        return self.figure._repr_png_() if self.figure is not None else None

    @property
    def png(self) -> bytes | None:
        return None if self.figure is None else self.figure.png

    @property
    def eepic(self) -> str | None:
        return None if self.figure is None else self.figure.eepic

    def save(self, path: str) -> None:
        if self.figure is None:
            raise RuntimeError(
                "save() called inside the figure() block, before rendering"
            )
        self.figure.save(path)


@contextmanager
def figure(
    lower_left: _epix.Point,
    upper_right: _epix.Point,
    size: str,
    dpi: int = 150,
    background: str | None = None,
) -> Generator[_Pending, None, None]:
    """Context manager: sets up the picture, captures + renders it on exit.

    Each figure starts from ePiX's startup state (``reset_state()``): no pen, fill,
    line style, label style, palette, camera or output format carries over from an
    earlier figure in the same process, so a notebook may draw any number of them.
    ``background`` (``"#rrggbb"``) makes the rendered PNG opaque in that colour; the
    default, ``None``, leaves it transparent.
    """
    _epix.reset_state()
    _epix.picture(lower_left, upper_right, size)
    _epix.begin()
    pending: _Pending = _Pending()
    yield pending
    pending.figure = render(dpi=dpi, background=background)


def fit_bounds(
    points: Sequence[_epix.Point], width: float, margin: float = 0.18
) -> tuple[_epix.Point, _epix.Point, str]:
    """The bounding box and size that fit ``points`` in a figure ``width`` inches wide.

    The box is the smallest one holding every point's ``x``/``y``, grown on each side
    by ``margin`` times the larger of its width and height. The size keeps one inch
    the same number of units on both axes, so circles stay round: the height is
    ``width`` scaled by the box's height over its width. A single point (or points
    that all coincide) gets a box one unit across. Returns
    ``(lower_left, upper_right, size)``, the first three arguments of
    :func:`figure`. 2D only: ``z`` is ignored.

    For example, the points (0, 0) and (2, 1) with no margin fit a 2-by-1 box, which
    at 4 inches wide is 2 inches tall::

        lower_left, upper_right, size = epix.fit_bounds(
            points=[epix.Point(x=0, y=0), epix.Point(x=2, y=1)], width=4, margin=0
        )
        # lower_left = (0, 0), upper_right = (2, 1), size = "4.00x2.00in"
    """
    if not points:
        raise ValueError("fit_bounds: at least one point is needed")
    xs: list[float] = [p.x1() for p in points]
    ys: list[float] = [p.x2() for p in points]
    span_x: float = max(xs) - min(xs)
    span_y: float = max(ys) - min(ys)
    extent: float = max(span_x, span_y) or 1.0  # coincident points: a unit box
    grow: float = margin * extent
    # an axis the points don't spread along still gets the full extent, so the box
    # never has zero width or height
    pad_x: float = grow + (extent / 2 if span_x == 0 else 0)
    pad_y: float = grow + (extent / 2 if span_y == 0 else 0)
    lower_left: _epix.Point = _epix.Point(x=min(xs) - pad_x, y=min(ys) - pad_y)
    upper_right: _epix.Point = _epix.Point(x=max(xs) + pad_x, y=max(ys) + pad_y)
    box_width: float = upper_right.x1() - lower_left.x1()
    box_height: float = upper_right.x2() - lower_left.x2()
    # the physical size, in inches, keeps the box's aspect ratio
    return (
        lower_left,
        upper_right,
        f"{width:.2f}x{width * box_height / box_width:.2f}in",
    )


@contextmanager
def figure_around(
    points: Sequence[_epix.Point],
    width: float,
    margin: float = 0.18,
    dpi: int = 150,
    background: str | None = None,
) -> Generator[_Pending, None, None]:
    """:func:`figure`, with its box and size fitted to ``points``.

    The box and size come from :func:`fit_bounds`. Use it for figures drawn from values
    that can change, as in a notebook whose reader edits the vectors and reruns: the
    picture always holds the points, at a fixed width and with equal scales on both
    axes::

        with epix.figure_around(points=[a, b, a + b], width=3.4) as fig:
            epix.arrow(tail=origin, head=a, scale=0.7)
            ...
        fig

    Everything else is :func:`figure`'s, including the state reset before drawing
    and the optional ``background``.
    """
    lower_left: _epix.Point
    upper_right: _epix.Point
    size: str
    lower_left, upper_right, size = fit_bounds(
        points=points, width=width, margin=margin
    )
    pending: _Pending
    with figure(
        lower_left, upper_right, size, dpi=dpi, background=background
    ) as pending:
        yield pending


@contextmanager
def activated(screen: _epix.Screen) -> Generator[_epix.Screen, None, None]:
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

    def __init__(self, gif: bytes, frames: list[Figure]) -> None:
        self.gif = gif
        self.frames = frames

    def _repr_html_(self) -> str:
        b64 = base64.b64encode(self.gif).decode("ascii")
        return f'<img src="data:image/gif;base64,{b64}"/>'

    def save(self, path: str) -> None:
        with open(path, "wb") as f:
            f.write(self.gif)


def animate(
    build: Callable[[], None], count: int = 24, dpi: int = 100, delay: int = 8
) -> Animation:
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
