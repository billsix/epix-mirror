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
# # levelset — ported from `samples/levelset.xp`
# Level sets of the saddle `z = k(x−y)(x+y)`: the contour plot of the scalar
# field over the base plane (`plot(f, p1, p2, …)`), plus the two families of
# hyperbolic level curves lifted onto the surface, drawn over each slice of a
# domain via `domain.slices2()`.

# %%
from __future__ import annotations

import math

import epix
from epix import Point

MAX = 3
k = 0.25


def Fp(u: float, v: float) -> epix.Point:
    r = math.sqrt(abs(v))
    if v >= 0:
        return Point(x=r * math.cosh(u), y=r * math.sinh(u), z=k * v)
    else:
        return Point(x=-r * math.sinh(u), y=-r * math.cosh(u), z=k * v)


def Fm(u: float, v: float) -> epix.Point:
    r = math.sqrt(abs(v))
    if v >= 0:
        return Point(x=-r * math.cosh(u), y=r * math.sinh(u), z=k * v)
    else:
        return Point(x=-r * math.sinh(u), y=r * math.cosh(u), z=k * v)


def f(x: float, y: float) -> float:
    return k * (x - y) * (x + y)


# %%
with epix.figure(
    lower_left=Point(x=-MAX, y=-MAX), upper_right=Point(x=MAX, y=MAX), size="2x2in"
) as fig:
    epix.camera.at(Point(x=1, y=-3, z=5))
    epix.camera.range(40)

    epix.clip_box(Point(x=MAX, y=MAX, z=2 * MAX))
    epix.clip()

    epix.set_red()
    epix.plot(
        f,
        p1=Point(x=-MAX, y=-MAX),
        p2=Point(x=MAX, y=MAX),
        coarse=epix.Mesh(4, 4),
        fine=epix.Mesh(40, 40),
    )

    R: epix.Domain = epix.Domain(
        lower_left=Point(x=-4, y=-9),
        upper_right=Point(x=4, y=9),
        coarse=epix.Mesh(1, 12),
        fine=epix.Mesh(90, 1),
    )

    epix.bold()
    epix.set_rgb(0.5, 0.5, 1)
    epix.line(tail=Point(x=MAX, y=-MAX, z=0), head=Point(x=-MAX, y=MAX, z=0))
    epix.line(tail=Point(x=MAX, y=MAX, z=0), head=Point(x=-MAX, y=-MAX, z=0))
    epix.plot(Fp, R.slices2())
    epix.plot(Fm, R.slices2())
fig
