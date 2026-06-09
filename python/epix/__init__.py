"""epix — Python front-end for ePiX (figures for LaTeX).

Phase 1 ships the inline-display layer (render an ePiX figure to a PNG for
Jupyter). Phase 2 will add the nanobind bindings so figures are *built* in
Python; this package is the import surface for both.

(Provisional package name; the final Python API shape/naming is still open.)
"""
from __future__ import annotations

from .render import Figure, render_xp, show

__all__ = ["Figure", "render_xp", "show"]
