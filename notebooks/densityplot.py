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
# # densityplot — ported from `samples/densityplot.xp`
# An interference pattern rendered as a color-shaded surface (viewed top-down).

# %%
from __future__ import annotations

from math import sqrt

import epix
from epix import Point

MAX = 18


def ripple(u: float, v: float) -> float:
    return pow(epix.cos(0.5 * sqrt(u * u + v * v)), 2)


def f(u: float, v: float) -> float:
    return 0.25 * (1 + ripple(u, v - 10) + ripple(u, v + 10))


def F(u: float, v: float) -> epix.Point:
    return Point(x=u, y=v, z=f(u, v))


def color(u: float, v: float, w: float) -> epix.Point:
    return Point(x=0.2 + 0.8 * w, y=0.3, z=0.8 - 0.6 * w)


R: epix.Domain = epix.Domain(
    lower_left=Point(x=-MAX, y=-MAX),
    upper_right=Point(x=MAX, y=MAX),
    coarse=epix.Mesh(nx=2 * MAX, ny=2 * MAX),
)

# %%
with epix.figure(
    lower_left=Point(x=-MAX, y=-MAX), upper_right=Point(x=MAX, y=MAX), size="3x3in"
) as fig:
    epix.pen(epix.magenta(1.4), width=0.2)
    epix.surface(F, R, color)
fig
