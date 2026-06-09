"""epix — Python front-end for ePiX (figures for LaTeX).

- Build a figure in Python via the nanobind bindings + the `figure()`/`render()`
  helpers (Phase 2), or render an existing `.xp` with `show()` (Phase 1).
- Figures display inline in Jupyter (PNG) via the render helper.

The scene API (P, Color, picture, begin, plot, label, pen, fill, line, the color
constructors, axes, …) is re-exported straight from the compiled `_epix`
bindings, so it grows automatically as the bound surface expands.

(Provisional package name / API shape — still open.)
"""
from __future__ import annotations

from ._epix import *           # noqa: F401,F403  (scene API: P, Color, picture, ...)
from .render import Figure, render_xp, render_eepic, show
from .figure import render, figure, animate, Animation
