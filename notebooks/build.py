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
import epix
from math import cos, sin, pi

# %% [markdown]
# A graph $y = x^2$ with axes:

# %%
with epix.figure(epix.P(-1.2, -0.2), epix.P(1.2, 1.2), "3x2in") as fig:
    epix.h_axis(4)
    epix.v_axis(4)
    epix.pen(epix.Blue())
    epix.plot(lambda t: epix.P(t, t * t), -1, 1)
    epix.pen(epix.Black())
    epix.label(epix.P(0.5, 0.5), r"$y=x^2$")
fig

# %% [markdown]
# A parametric curve (the unit circle), straight from a Python lambda:

# %%
with epix.figure(epix.P(-1.2, -1.2), epix.P(1.2, 1.2), "2x2in") as fig:
    epix.pen(epix.Red())
    epix.plot(lambda t: epix.P(cos(t), sin(t)), 0, 2 * pi)
fig

# %% [markdown]
# Shapes, markers, and arrows — with the scoped enums (`MarkType`, `LabelPos`):

# %%
with epix.figure(epix.P(-2, -2), epix.P(2, 2), "3x3in") as fig:
    epix.pen(epix.Blue())
    epix.circle(epix.P(0, 0), 1.0)
    epix.pen(epix.Red())
    epix.dot(epix.P(1, 1))
    epix.marker(epix.P(-1, -1), epix.MarkType.BOX)
    epix.arrow(epix.P(0, 0), epix.P(1.5, 0))
    epix.label(epix.P(0, 1), epix.P(0, 0), "top", epix.LabelPos.t)
fig
