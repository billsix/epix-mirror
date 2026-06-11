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
# # plotting2 — ported from `samples/plotting2.xp`
# A parametric surface over a polar domain, with coordinate arrows.

# %%
from __future__ import annotations

from math import pi

import epix
from epix import Point


def f(r: float, th: float) -> epix.Point:
    return Point(x=r * epix.cos(th), y=r * epix.sin(th), z=pow(r, 3) * epix.cos(3 * th))


# %%
with epix.figure(
    lower_left=Point(x=-1, y=-1), upper_right=Point(x=1, y=1), size="2x1in"
) as fig:
    R: epix.Domain = epix.Domain(
        lower_left=Point(x=0, y=0),
        upper_right=Point(x=1, y=2 * pi),
        coarse=epix.Mesh(nx=8, ny=40),
        fine=epix.Mesh(nx=16, ny=120),
    )
    epix.camera.at(3, 1, 2)
    epix.arrow(tail=Point(x=0, y=0, z=0), head=Point(x=1.25, y=0, z=0))
    epix.arrow(tail=Point(x=0, y=0, z=0), head=Point(x=0, y=1.25, z=0))
    epix.plain(epix.blue(1.2))
    epix.fill(epix.yellow())
    epix.surface(f, R)
fig
