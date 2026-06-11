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
# # layout — ported from `samples/layout.xp`
# A 2×3 grid of views of a surface from rotating camera angles (`screen`/`inset`).

# %%
from __future__ import annotations

from math import pi

import epix
from epix import Point


def f(u: float, v: float) -> epix.Point:
    return Point(x=(u - v) * (u + v), y=2 * u * v, z=u)


# %%
with epix.figure(
    lower_left=Point(x=0, y=0), upper_right=Point(x=2, y=3), size="5x7.5in"
) as fig:
    R: epix.Domain = epix.Domain(
        lower_left=Point(x=-1, y=-1),
        upper_right=Point(x=1, y=1),
        coarse=epix.Mesh(nx=12, ny=12),
        fine=epix.Mesh(nx=24, ny=24),
    )
    for i in range(2):
        for j in range(3):
            panel: epix.Screen = epix.Screen(
                lower_left=Point(x=-3, y=-3), upper_right=Point(x=3, y=3)
            )
            epix.activate(panel)
            epix.border(epix.red(0.6), width="1pt")
            epix.camera.at(
                epix.sph(radius=10, theta=(2 * j + i + 1) * pi / 8, phi=pi / 6)
            )
            epix.plot(f, R)
            epix.arrow(tail=Point(x=0, y=0, z=0), head=2 * epix.E_1)
            epix.arrow(tail=Point(x=0, y=0, z=0), head=2 * epix.E_2)
            epix.inset(
                lower_left=Point(x=i + 0.05, y=2.05 - j),
                upper_right=Point(x=i + 0.95, y=2.95 - j),
            )
            epix.deactivate(panel)
fig
