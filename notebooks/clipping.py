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
# # clipping — ported from `samples/clipping.xp`
# A monkey saddle sliced by two vertical planes, illustrating the geometric
# meaning of partial derivatives. The surface is hand-ordered into pieces
# interspersed with axes and cutting planes, each clipped to a box.

# %%
from __future__ import annotations

import math

import epix
from epix import Point

# camera location (in spherical coordinates); must be in first orthant
VIEWPT = epix.sph(radius=4, theta=math.pi / 6, phi=math.pi / 6)

MESH = 12  # number of coordinate grid squares

# location of tangency point
x_0 = 7.0 / MESH
y_0 = 6.0 / MESH
z_0 = 0.25  # height of top of slicing planes

MAX = 1  # maximum coordinate

sqrt3 = math.sqrt(3)


# colors
def color_coord() -> None:
    epix.plain(epix.rgb(1, 0.9, 0.7))


def color_axis() -> None:
    epix.set_rgb(0.8, 0.2, 0.9)


# graph and mesh
def color_surf() -> None:
    epix.plain(epix.rgb(1, 0.5, 0))
    epix.fill(epix.rgb(1, 0.8, 0.2))


# slicing plane and border
def color_xslice() -> None:
    epix.set_red()


def fill_xslice() -> None:
    epix.set_rgb(0.8, 0.5, 0.1)


def color_yslice() -> None:
    epix.set_blue()


def fill_yslice() -> None:
    epix.set_rgb(0.6, 0.3, 0)


# function to be graphed
def f(x: float, y: float) -> epix.Point:
    return Point(x=x, y=y, z=0.75 * y * (y - sqrt3 * x) * (y + sqrt3 * x))


# %%
with epix.figure(
    lower_left=Point(x=-2, y=-2), upper_right=Point(x=2, y=1.5), size="6 x 5.25in"
) as fig:
    # "legend"
    epix.masklabel(
        Point(x=epix.xmax(), y=epix.ymax()),
        offset=Point(x=-2, y=-2),
        text=r"$z=\displaystyle\frac{1}{2}(y^3-3x^2y)$",
        align=epix.LabelPos.bl,
    )

    epix.font_size("scriptsize")
    epix.camera.at(VIEWPT)
    epix.border(epix.green(0.6), width="1pt")

    R: epix.Domain = epix.Domain(
        lower_left=Point(x=-MAX, y=-MAX),
        upper_right=Point(x=MAX, y=MAX),
        coarse=epix.Mesh(nx=4 * MESH, ny=4 * MESH),
        fine=epix.Mesh(nx=8 * MESH, ny=8 * MESH),
    )

    # coordinate grids
    color_coord()
    epix.grid(
        lower_left=Point(x=-MAX, y=-MAX, z=-MAX),
        upper_right=Point(x=-MAX, y=MAX, z=MAX),
        nx=MESH,
        ny=MESH,
    )
    epix.grid(
        lower_left=Point(x=-MAX, y=-MAX, z=-MAX),
        upper_right=Point(x=MAX, y=-MAX, z=MAX),
        nx=MESH,
        ny=MESH,
    )
    epix.grid(
        lower_left=Point(x=-MAX, y=-MAX, z=-MAX),
        upper_right=Point(x=MAX, y=MAX, z=-MAX),
        nx=MESH,
        ny=MESH,
    )

    epix.clip_box(lower_left=Point(x=-2, y=-2, z=-1), upper_right=Point(x=2, y=2, z=1))

    # back half and front left quarter
    color_surf()
    epix.surface(f, R.resize1(-MAX, 0))
    epix.surface(f, R.resize1(0, MAX).resize2(-MAX, 0))

    # coordinate axes
    color_axis()
    epix.bold()

    epix.clip_box(lower_left=Point(x=-2, y=-2, z=-1), upper_right=Point(x=2, y=2, z=2))

    epix.dart(tail=Point(x=-MAX, y=0, z=0), head=Point(x=0.25 + MAX, y=0, z=0))
    epix.dart(tail=Point(x=0, y=-MAX, z=0), head=Point(x=0, y=0.25 + MAX, z=0))
    epix.dart(tail=Point(x=0, y=0, z=0), head=Point(x=0, y=0, z=0.25 + MAX))

    epix.label(
        Point(x=0.25 + MAX, y=0, z=0),
        offset=Point(x=-2, y=-2),
        text="$x$",
        align=epix.LabelPos.bl,
    )
    epix.label(
        Point(x=0, y=0.25 + MAX, z=0),
        offset=Point(x=4, y=-2),
        text="$y$",
        align=epix.LabelPos.r,
    )
    epix.label(
        Point(x=0, y=0, z=0.25 + MAX),
        offset=Point(x=0, y=4),
        text="$z$",
        align=epix.LabelPos.t,
    )

    # front quarter of surface; chop into four pieces
    color_surf()
    epix.surface(f, R.resize1(0, x_0).resize2(0, y_0))  # behind both planes

    fill_xslice()
    epix.rect(
        lower_left=Point(x=x_0, y=0, z=-MAX), upper_right=Point(x=x_0, y=y_0, z=z_0)
    )  # left part of plane x = x_0

    epix.pen(3)
    color_xslice()
    epix.plot(f, R.slice1(x_0).resize2(0, y_0))

    color_surf()
    epix.surface(f, R.resize1(x_0, MAX).resize2(0, y_0))  # back right piece

    fill_yslice()
    epix.rect(
        lower_left=Point(x=0, y=y_0, z=-MAX), upper_right=Point(x=MAX, y=y_0, z=z_0)
    )  # plane y = y_0

    epix.pen(3)
    color_yslice()
    epix.plot(f, R.slice2(y_0).resize1(0, MAX))

    color_surf()
    epix.surface(f, R.resize1(0, x_0).resize2(y_0, MAX))  # front left piece

    fill_xslice()
    epix.rect(
        lower_left=Point(x=x_0, y=y_0, z=-MAX), upper_right=Point(x=x_0, y=MAX, z=z_0)
    )  # right part of plane x = x_0

    epix.pen(3)
    color_xslice()
    epix.plot(f, R.slice1(x_0).resize2(y_0, MAX))

    color_surf()
    epix.surface(f, R.resize1(x_0, MAX).resize2(y_0, MAX))  # front right piece

    epix.clip_box(
        lower_left=Point(x=-MAX, y=-MAX, z=-2), upper_right=Point(x=MAX, y=MAX, z=2)
    )

    # labels and graph slices
    color_yslice()
    epix.label(
        f(MAX, y_0),
        offset=Point(x=-4, y=0),
        text=r"$\displaystyle\frac{\partial f}{\partial x}$: $y$ constant",
        align=epix.LabelPos.l,
    )

    color_xslice()
    epix.label(
        f(x_0, MAX),
        offset=Point(x=4, y=0),
        text=r"$\displaystyle\frac{\partial f}{\partial y}$: $x$ constant",
        align=epix.LabelPos.br,
    )

    epix.pst_format()
fig
