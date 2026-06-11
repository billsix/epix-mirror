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
# # vfield — ported from `samples/vfield.xp`
# A 3-D vector field with several integral curves (`ode_plot`) and a dart field.

# %%
from __future__ import annotations

from math import pi

import epix
from epix import Point


def F(x: float, y: float, z: float) -> epix.Point:
    return -0.25 * Point(x=0.5 * x + y, y=0.5 * y - x, z=1 + z)


R: epix.Domain = epix.Domain(
    lower_left=Point(x=-1, y=-1, z=-1),
    upper_right=Point(x=1, y=1, z=1),
    coarse=epix.Mesh(nx=6, ny=6, nz=4),
    fine=epix.Mesh(),
)

# %%
with epix.figure(
    lower_left=Point(x=-2, y=-2), upper_right=Point(x=2, y=2), size="4x4in"
) as fig:
    epix.border()
    epix.label(
        Point(x=0, y=epix.ymax()),
        offset=Point(x=0, y=-4),
        text="$F(x,y,z)=-(x+2y, y-2x, 1+z)$",
        align=epix.LabelPos.b,
    )
    epix.camera.at(Point(x=11, y=8, z=5))
    grid_count: int = 6
    epix.plain(epix.black(0.3))
    epix.grid(
        lower_left=Point(x=-1, y=-1, z=-1),
        upper_right=Point(x=-1, y=1, z=1),
        nx=grid_count,
        ny=grid_count,
    )
    epix.grid(
        lower_left=Point(x=-1, y=-1, z=-1),
        upper_right=Point(x=1, y=-1, z=1),
        nx=grid_count,
        ny=grid_count,
    )
    epix.grid(
        lower_left=Point(x=-1, y=-1, z=-1),
        upper_right=Point(x=1, y=1, z=-1),
        nx=grid_count,
        ny=grid_count,
    )
    epix.bold()
    for i in range(12):
        t: float = pi / 2 + i * pi / 6
        epix.pen(epix.rgb(0.25 * (3 + epix.sin(t)), 0.25, 0.25 * (3 + epix.cos(t))))
        epix.ode_plot(F, epix.cyl(radius=1.4, theta=t, z=1), 0, 10, n=60)
    epix.plain(epix.black())
    epix.base(epix.white(), width="2pt")
    epix.dart_field(F, R, scale=0.5)  # darts at 50% of default length
    label_offset: float = 0.125
    epix.font_size("scriptsize")
    epix.degrees()
    epix.label_angle(30)
    epix.label(
        Point(x=1 + label_offset, y=-1, z=-1),
        offset=Point(x=-4, y=-2),
        text="$y=-1$",
        align=epix.LabelPos.bl,
    )
    epix.label(
        Point(x=1 + label_offset, y=0, z=-1),
        offset=Point(x=-4, y=-2),
        text="$y=0$",
        align=epix.LabelPos.bl,
    )
    epix.label(
        Point(x=1 + label_offset, y=1, z=-1),
        offset=Point(x=-4, y=-2),
        text="$y=1$",
        align=epix.LabelPos.bl,
    )
    epix.label_angle(-15)
    epix.label(
        Point(x=-1, y=1 + label_offset, z=-1),
        offset=Point(x=4, y=0),
        text="$x=-1$",
        align=epix.LabelPos.br,
    )
    epix.label(
        Point(x=0, y=1 + label_offset, z=-1),
        offset=Point(x=4, y=0),
        text="$x=0$",
        align=epix.LabelPos.br,
    )
    epix.label(
        Point(x=1, y=1 + label_offset, z=-1),
        offset=Point(x=4, y=0),
        text="$x=1$",
        align=epix.LabelPos.br,
    )
    epix.label(
        Point(x=1, y=-1 - label_offset, z=-1),
        offset=Point(x=-4, y=0),
        text="$z=-1$",
        align=epix.LabelPos.tl,
    )
    epix.label(
        Point(x=1, y=-1 - label_offset, z=0),
        offset=Point(x=-4, y=0),
        text="$z=0$",
        align=epix.LabelPos.tl,
    )
    epix.label(
        Point(x=1, y=-1 - label_offset, z=1),
        offset=Point(x=-4, y=0),
        text="$z=1$",
        align=epix.LabelPos.tl,
    )
fig
