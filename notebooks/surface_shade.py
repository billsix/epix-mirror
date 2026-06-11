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
# # surface_shade — ported from `samples/surface_shade.xp`
# A monkey-saddle surface shaded by a position-dependent color function.

# %%
from __future__ import annotations

from math import tanh

import epix
from epix import Point


def f(u: float, v: float) -> epix.Point:
    return Point(x=u, y=v, z=0.5 * u * v * (u - v) * (u + v))


def color(x: float, y: float, z: float) -> epix.Point:
    return Point(x=-tanh(z), y=0, z=tanh(z))  # quasi-geographic colors


# %%
with epix.figure(
    lower_left=Point(x=-3, y=-3), upper_right=Point(x=3, y=3), size="6x6in"
) as fig:
    epix.revolutions()
    epix.set_crop()
    epix.fill()
    epix.backing(epix.black())
    epix.plain(epix.yellow())
    epix.camera.at(Point(x=8, y=-8, z=4))
    R: epix.Domain = epix.Domain(
        lower_left=Point(x=-2, y=-2),
        upper_right=Point(x=2, y=2),
        coarse=epix.Mesh(nx=36, ny=36),
        fine=epix.Mesh(nx=72, ny=72),
    )
    epix.surface(f, R, color)
fig
