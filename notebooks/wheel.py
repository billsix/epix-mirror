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
# # wheel — ported from `samples/wheel.xp`
# A rolling wheel tracing a cycloid, shown as a stack of panels (`screen`/`inset`).

# %%
from __future__ import annotations

from math import ceil, pi

import epix
from epix import Point


def F(t: float, r: float) -> epix.Point:
    return Point(x=t - r * epix.sin(t), y=1 - r * epix.cos(t))  # a cycloid


# %%
with epix.figure(
    lower_left=Point(x=0, y=0), upper_right=Point(x=15, y=27), size="4 x 7.2in"
) as fig:
    dt: float = 5 * pi / 11
    t: float = dt
    for i in range(9):
        panel: epix.Screen = epix.Screen(
            lower_left=Point(x=-1, y=0), upper_right=Point(x=15, y=2)
        )  # drawing area
        with epix.activated(panel):
            epix.plain(epix.black())
            epix.line(tail=Point(x=-1, y=0), head=Point(x=15, y=0))  # the ground
            epix.circle(center=Point(x=t, y=1), radius=1)  # the wheel
            R: epix.Domain = epix.Domain(
                lower_left=Point(x=0, y=0),
                upper_right=Point(x=t, y=1),
                coarse=epix.Mesh(nx=10 * i, ny=5),
                fine=epix.Mesh(nx=int(ceil(1 + 4 * t)), ny=5),
            )
            epix.bold()
            for j in range(6):
                epix.pen(epix.rgb(1 - 0.125 * j, 0.125 * j, 0.5 + 0.125 * j))
                epix.plot(F, R.slice2(0.2 * j))
            epix.bold(epix.green())
            epix.line(tail=Point(x=t, y=1), head=F(t, 1))
            epix.inset(
                lower_left=Point(x=0, y=25 - 3 * i),
                upper_right=Point(x=15, y=27 - 3 * i),
            )  # page layout
        t += dt
fig
