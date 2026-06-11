# ---
# jupyter:
#   jupytext:
#     text_representation:
#       extension: .py
#       format_name: percent
#       format_version: '1.3'
#   kernelspec:
#     display_name: epix
#     language: python
#     name: epix
# ---

# %% [markdown]
# # Building ePiX figures in Python
#
# Phase 2: construct a figure with the nanobind bindings — the same drawing API
# as the C++ `.xp` programs, but Pythonic (plots take ordinary Python lambdas).
# Output is byte-identical to the C++ library.

# %%
from __future__ import annotations

from math import cos, pi, sin

import epix

# %% [markdown]
# A graph $y = x^2$ with axes:

# %%
with epix.figure(
    lower_left=epix.Point(x=-1.2, y=-0.2),
    upper_right=epix.Point(x=1.2, y=1.2),
    size="3x2in",
) as fig:
    epix.h_axis(4)
    epix.v_axis(4)
    epix.pen(epix.blue())
    epix.plot(lambda t: epix.Point(x=t, y=t * t), -1, 1)
    epix.pen(epix.black())
    epix.label(epix.Point(x=0.5, y=0.5), r"$y=x^2$")
fig

# %% [markdown]
# A parametric curve (the unit circle), straight from a Python lambda:

# %%
with epix.figure(
    lower_left=epix.Point(x=-1.2, y=-1.2),
    upper_right=epix.Point(x=1.2, y=1.2),
    size="2x2in",
) as fig:
    epix.pen(epix.red())
    epix.plot(lambda t: epix.Point(x=cos(t), y=sin(t)), 0, 2 * pi)
fig

# %% [markdown]
# Shapes, markers, and arrows — with the scoped enums (`MarkType`, `LabelPos`):

# %%
with epix.figure(
    lower_left=epix.Point(x=-2, y=-2), upper_right=epix.Point(x=2, y=2), size="3x3in"
) as fig:
    epix.pen(epix.blue())
    epix.circle(center=epix.Point(x=0, y=0), radius=1.0)
    epix.pen(epix.red())
    epix.dot(epix.Point(x=1, y=1))
    epix.marker(epix.Point(x=-1, y=-1), epix.MarkType.BOX)
    epix.arrow(tail=epix.Point(x=0, y=0), head=epix.Point(x=1.5, y=0))
    epix.label(
        epix.Point(x=0, y=1),
        offset=epix.Point(x=0, y=0),
        text="top",
        align=epix.LabelPos.t,
    )
fig
