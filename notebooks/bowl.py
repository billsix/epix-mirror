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
# # bowl — ported from `samples/bowl.xp`
# The paraboloid z = x² + y², drawn front and back (the `scenery` surface object).

# %%
from __future__ import annotations

import epix
from epix import Point


def f(u: float, v: float) -> epix.Point:
    return u * Point(x=epix.cos(v), y=epix.sin(v), z=u)


# %%
with epix.figure(
    lower_left=Point(x=-2, y=-1), upper_right=Point(x=2, y=2), size="6x4.5in"
) as fig:
    epix.backing(epix.black())
    epix.set_white()
    epix.label(
        Point(x=0, y=epix.ymax()),
        offset=Point(x=0, y=-2),
        text=r"\LARGE$z=x^2+y^2$",
        align=epix.LabelPos.b,
    )
    epix.revolutions()
    epix.camera.at(Point(x=3, y=2, z=2.5))
    epix.set_red()
    epix.polar_grid(radius=1, nx=8, ny=36)
    epix.arrow(tail=Point(x=0, y=0, z=0), head=Point(x=1.25, y=0, z=0))
    epix.arrow(tail=Point(x=0, y=0, z=0), head=Point(x=0, y=1.25, z=0))
    epix.label(
        Point(x=1.25, y=0, z=0),
        offset=Point(x=-2, y=-2),
        text="$x$",
        align=epix.LabelPos.bl,
    )
    epix.label(
        Point(x=0, y=1.25, z=0),
        offset=Point(x=2, y=-1),
        text="$y$",
        align=epix.LabelPos.br,
    )
    epix.fill()
    epix.set_rgb(1, 0.8, 0.2)
    R: epix.Domain = epix.Domain(
        lower_left=Point(x=0, y=0),
        upper_right=Point(x=1, y=1),
        coarse=epix.Mesh(nx=12, ny=180),
        fine=epix.Mesh(nx=12, ny=180),
    )
    bowl_front: epix.Scenery = epix.Scenery(f, R)
    bowl_front.draw(cull=-1)  # cull back-pointing facets
    epix.set_red()
    epix.arrow(tail=Point(x=0, y=0, z=0), head=Point(x=0, y=0, z=1))
    epix.pen(epix.blue(1.6), width="0.5pt")
    epix.fill(epix.blue(1.4))
    bowl_back: epix.Scenery = epix.Scenery(f, R)
    bowl_back.draw(cull=1)  # cull front-pointing facets
    epix.nofill()
    epix.bold(epix.green())
    epix.base(epix.green(0.6), width="1.5pt")
    epix.plot(f, R.slice1(1))
fig
