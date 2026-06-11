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
# # levelset2 — ported from `samples/levelset2.xp`
# A monkey-saddle surface drawn as level bands: the surface is re-drawn inside a
# stack of thin horizontal `clip_slice` planes, each shaded by height, over a
# wire-mesh graph and a 3-D coordinate frame. (C++ default — `FLATFILL` undefined
# — uses the position-dependent `color` shading.)

# %%
from __future__ import annotations

import math

import epix
from epix import Point

MAX = 1

sqrt3 = math.sqrt(3)


# function to be graphed
def f(x: float, y: float) -> epix.Point:
    return Point(x=x, y=y, z=0.75 * y * (y - sqrt3 * x) * (y + sqrt3 * x))


def color(u: float, v: float, w: float) -> epix.Point:
    return Point(x=-0.5 * w, y=0.25 * w, z=0.95 * w)


# In the C++ the 3rd ctor arg is the comma-expression `(24, 24)` == 24, which
# converts to mesh(24); reproduced here as epix.Mesh(24).
R: epix.Domain = epix.Domain(
    lower_left=Point(x=-MAX, y=-MAX),
    upper_right=Point(x=MAX, y=MAX),
    coarse=epix.Mesh(24),
    fine=epix.Mesh(72, 72),
)

# %%
with epix.figure(
    lower_left=Point(x=-2 * MAX, y=-2 * MAX),
    upper_right=Point(x=2 * MAX, y=2 * MAX),
    size="6 x 6in",
) as fig:
    epix.border(epix.green(0.6), width="1pt")
    epix.backing(epix.black())

    epix.camera.at(epix.sph(radius=4 * MAX, theta=math.pi / 6, phi=math.pi / 6))

    # positioned for viewpt in first orthant
    epix.set_yellow()
    epix.grid(
        lower_left=Point(x=-MAX, y=-MAX, z=-MAX),
        upper_right=Point(x=MAX, y=MAX, z=-MAX),
        nx=4,
        ny=4,
    )
    epix.grid(
        lower_left=Point(x=-MAX, y=-MAX, z=-MAX),
        upper_right=Point(x=MAX, y=-MAX, z=MAX),
        nx=4,
        ny=4,
    )
    epix.grid(
        lower_left=Point(x=-MAX, y=-MAX, z=-MAX),
        upper_right=Point(x=-MAX, y=MAX, z=MAX),
        nx=4,
        ny=4,
    )

    Ax: epix.Axis = epix.Axis(
        Point(x=-MAX, y=-MAX, z=MAX),
        Point(x=MAX, y=-MAX, z=MAX),
        n=4,
        offset=Point(x=0, y=6),
        align=epix.LabelPos.t,
    )
    Ay: epix.Axis = epix.Axis(
        Point(x=MAX, y=-MAX, z=-MAX),
        Point(x=MAX, y=MAX, z=-MAX),
        n=4,
        offset=Point(x=-2, y=-2),
        align=epix.LabelPos.bl,
    )
    Az: epix.Axis = epix.Axis(
        Point(x=MAX, y=-MAX, z=-MAX),
        Point(x=MAX, y=-MAX, z=MAX),
        n=4,
        offset=Point(x=-2, y=-2),
        align=epix.LabelPos.bl,
    )

    Ax.frac().draw()
    Ay.frac().draw()
    Az.frac().draw()

    epix.clip_box(Point(x=MAX, y=MAX, z=MAX))

    # wire mesh surface
    epix.plain(epix.green())
    epix.plot(f, R)

    # level bands
    epix.plain(epix.red())

    for i in range(-5, 6):
        epix.clip_slice(
            loc=Point(x=0, y=0, z=0.2 * i), normal=Point(x=0, y=0, z=1), thickness=0.1
        )
        epix.surface(f, R, color)  # domain- or position-dependent coloring
        epix.clip_restore()  # remove temporary slicing planes, keep clip box

    epix.pst_format()
fig
