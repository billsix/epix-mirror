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
# # sqrt — ported from `samples/sqrt.xp`
# The Riemann surface of the complex square root (uses the `pair` type as ℂ).

# %%
from __future__ import annotations

import epix
from epix import Point


def f(x: float, y: float) -> epix.Point:
    z: epix.Pair = epix.Pair(x, y)
    z_squared: epix.Pair = z * z
    return Point(x=z_squared.x1(), y=z_squared.x2(), z=x)


def F(r: float, t: float) -> epix.Point:
    return f(r * epix.cos(t), r * epix.sin(t))


def g(t: float) -> epix.Point:
    return t * Point(x=t, y=0, z=1)


R: epix.Domain = epix.Domain(
    lower_left=Point(x=0, y=0),
    upper_right=Point(x=1.25, y=0.5),
    coarse=epix.Mesh(nx=6, ny=24),
    fine=epix.Mesh(nx=12, ny=60),
)

# %%
with epix.figure(
    lower_left=Point(x=-1.5, y=-1.5), upper_right=Point(x=1.5, y=1.5), size="2.5x2.5in"
) as fig:
    epix.revolutions()
    epix.camera.at(Point(x=4, y=-2, z=3))
    epix.pen(epix.blue(1.7))
    epix.plot(F, R)
    epix.pen(epix.blue())
    epix.plot(F, R.resize2(0.5, 1))
    epix.pen(epix.black())
    epix.arrow(tail=Point(x=0, y=0, z=0), head=2 * epix.E_1)
    epix.arrow(tail=Point(x=0, y=0, z=0), head=2 * epix.E_2)
    epix.arrow(tail=Point(x=0, y=0, z=0), head=1.5 * epix.E_3)
    epix.masklabel(
        2 * epix.E_1,
        offset=Point(x=0, y=0),
        text=r"$\mathrm{Re}\,z$",
        align=epix.LabelPos.r,
    )
    epix.label(
        2 * epix.E_2,
        offset=Point(x=0, y=0),
        text=r"$\mathrm{Im}\,z$",
        align=epix.LabelPos.r,
    )
    epix.label(
        1.5 * epix.E_3,
        offset=Point(x=2, y=0),
        text=r"$\mathrm{Re}\,\sqrt{z}$",
        align=epix.LabelPos.r,
    )
    epix.bold(epix.red())
    epix.plot(g, -1.25, 1.25, n=40)
fig
