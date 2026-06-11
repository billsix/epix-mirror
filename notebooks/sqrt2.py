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
# # sqrt2 — ported from `samples/sqrt2.xp`
# Both branches of the complex square root, as a self-intersecting surface
# (a `scenery` built from two domain strips).

# %%
from __future__ import annotations

import epix
from epix import Point

MAX = 1.5


def f(u: float, v: float) -> epix.Point:
    return Point(x=u * u * epix.cos(v), y=u * u * epix.sin(v), z=u * epix.cos(0.5 * v))


def sqrt_re(t: float) -> epix.Point:
    return f(t, 0)


# %%
with epix.figure(
    lower_left=Point(x=-4, y=-4), upper_right=Point(x=4, y=2), size="4x3in"
) as fig:
    epix.camera.filter(epix.rgb_neutral())
    epix.backing(epix.magenta(0.1))
    epix.set_crop()
    epix.revolutions()
    epix.label(
        Point(x=epix.xmin(), y=epix.ymax()),
        offset=Point(x=2, y=-2),
        text=r"Branches of $\sqrt{z}$",
        align=epix.LabelPos.br,
    )
    epix.camera.at(6, -2, 2)
    epix.camera.range(4)
    R: epix.Domain = epix.Domain(
        lower_left=Point(x=0, y=0),
        upper_right=Point(x=MAX, y=2),
        coarse=epix.Mesh(nx=16, ny=80),
        fine=epix.Mesh(nx=16, ny=160),
    )
    epix.pen(epix.green(0.7), width="0.15pt")
    surf: epix.Scenery = epix.Scenery(f, R.resize2(1, 2))
    epix.fill(epix.white(0.7))
    epix.pen(epix.black())
    surf.add(f, R.resize2(0, 1))
    surf.draw()
    epix.nofill()
    epix.bbold(epix.red())
    epix.plot(sqrt_re, -MAX, 0, n=30)
    epix.bold(epix.blue())
    epix.arrow(tail=Point(x=0, y=0, z=0), head=Point(x=2, y=0, z=0))
    epix.arrow(tail=Point(x=0, y=0, z=0), head=Point(x=0, y=2, z=0))
    epix.label_color(epix.blue())
    epix.label(
        Point(x=2, y=0, z=0),
        offset=Point(x=0, y=-4),
        text=r"$\mathrm{Re}\,z$",
        align=epix.LabelPos.b,
    )
    epix.label(
        Point(x=0, y=2, z=0),
        offset=Point(x=4, y=0),
        text=r"$\mathrm{Im}\,z$",
        align=epix.LabelPos.r,
    )
    epix.bbold(epix.yellow())
    epix.plot(sqrt_re, 0, MAX, n=30)
fig
