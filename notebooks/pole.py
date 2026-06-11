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
# # pole — ported from `samples/pole.xp`
# Re(1/(x+iy)) as a surface with a simple pole, clipped top/bottom for transparency.

# %%
from __future__ import annotations

import epix
from epix import Point

mesh_density: int = 8
MAX: float = 1.5
box_size: int = 2


def F(x: float, y: float) -> epix.Point:  # fake pole
    return Point(x=x, y=y, z=x / (0.01 + x * x + y * y))


domain_min: epix.Point = Point(x=-MAX, y=-MAX)
domain_max: epix.Point = Point(x=MAX, y=MAX)
R: epix.Domain = epix.Domain(
    domain_min,
    domain_max,
    epix.Mesh(nx=2 * mesh_density, ny=2 * mesh_density),
    epix.Mesh(nx=6 * mesh_density, ny=6 * mesh_density),
)

# %%
with epix.figure(
    lower_left=Point(x=-box_size, y=-box_size),
    upper_right=Point(x=box_size, y=box_size),
    size="2.5x2.5in",
) as fig:
    epix.label(
        Point(x=epix.xmax(), y=2.5),
        Point(x=-2, y=-2),
        r"$z=\mathrm{Re}\,\displaystyle\frac{1}{x+iy}$",
        epix.LabelPos.bl,
    )
    epix.camera.at(Point(x=6, y=8, z=5))
    epix.clip_box(
        lower_left=Point(x=-box_size, y=-box_size, z=-box_size),
        upper_right=Point(x=box_size, y=box_size, z=0),
    )
    epix.plain(epix.magenta())
    epix.fill(epix.white())
    epix.surface(F, R)  # bottom half
    epix.grid(
        lower_left=domain_min, upper_right=domain_max, nx=mesh_density, ny=mesh_density
    )
    epix.pen(epix.white(0.75), width="0.15pt")
    epix.plot(F, R)  # simulate transparency
    epix.clip_box(
        lower_left=Point(x=-box_size, y=-box_size, z=-box_size),
        upper_right=Point(x=box_size, y=box_size, z=2 * box_size),
    )
    epix.plain(epix.magenta())
    epix.arrow(tail=Point(x=-box_size, y=0, z=0), head=Point(x=box_size, y=0, z=0))
    epix.label(Point(x=box_size, y=0), Point(x=-4, y=-2), "$x$", epix.LabelPos.l)
    epix.arrow(tail=Point(x=0, y=-box_size, z=0), head=Point(x=0, y=box_size, z=0))
    epix.label(Point(x=0, y=box_size), Point(x=2, y=-2), "$y$", epix.LabelPos.br)
    epix.arrow(tail=Point(x=0, y=0, z=0), head=Point(x=0, y=0, z=2.5))
    epix.label(Point(x=0, y=0, z=2.5), Point(x=0, y=4), "$z$", epix.LabelPos.t)
    epix.clip_box(
        lower_left=Point(x=-box_size, y=-box_size, z=0),
        upper_right=Point(x=box_size, y=box_size, z=box_size),
    )
    epix.surface(F, R)  # top half
    epix.bbold(epix.red())
    epix.plot(F, R.resize1(0.25, MAX).slice2(0))
fig
