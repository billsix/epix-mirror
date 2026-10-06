"""epix — Python front-end for ePiX (figures for LaTeX).

- Build a figure in Python via the nanobind bindings + the `figure()`/`render()`
  helpers (Phase 2), or render an existing `.xp` with `show()` (Phase 1).
- Figures display inline in Jupyter (PNG) via the render helper.

The scene API (Point, Color, picture, begin, plot, label, pen, fill, line, the
color constructors, axes, …) is re-exported straight from the compiled `_epix`
bindings, so it grows automatically as the bound surface expands.

(Provisional package name / API shape — still open.)
"""

from __future__ import annotations

from ._epix import *  # noqa: F401,F403  (scene API: Point, Color, picture, ...)
from .figure import Animation as Animation
from .figure import activated as activated
from .figure import animate as animate
from .figure import figure as figure
from .figure import render as render
from .render import Figure as Figure
from .render import render_eepic as render_eepic
from .render import render_xp as render_xp
from .render import show as show
