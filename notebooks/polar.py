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
# # polar — ported from `samples/polar.xp`
# A polar rose on a polar grid (degrees mode).

# %%
from __future__ import annotations

import epix
from epix import Point


def f(t: float) -> float:
    return 2 * epix.cos(3 * t)


# %%
with epix.figure(
    lower_left=Point(x=-2, y=-2), upper_right=Point(x=2, y=2), size="2x2in"
) as fig:
    epix.degrees()
    epix.set_black(0.3)
    epix.polar_grid(radius=2, nx=4, ny=24)  # radius, rings, sectors
    epix.set_black()
    epix.bottom_axis(4, offset=Point(x=0, y=-4)).subdivide(n=2).draw()
    epix.left_axis(4, offset=Point(x=-4, y=0)).subdivide(n=2).draw()
    epix.bold()
    epix.polarplot(f, 0, 180, n=120)
fig
