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
import math

import epix
from epix import P

# camera location (in spherical coordinates); must be in first orthant
VIEWPT = epix.sph(4, math.pi / 6, math.pi / 6)

MESH = 12  # number of coordinate grid squares

# location of tangency point
x_0 = 7.0 / MESH
y_0 = 6.0 / MESH
z_0 = 0.25  # height of top of slicing planes

MAX = 1  # maximum coordinate

sqrt3 = math.sqrt(3)


# colors
def color_coord():
    epix.plain(epix.RGB(1, 0.9, 0.7))


def color_axis():
    epix.rgb(0.8, 0.2, 0.9)


# graph and mesh
def color_surf():
    epix.plain(epix.RGB(1, 0.5, 0))
    epix.fill(epix.RGB(1, 0.8, 0.2))


# slicing plane and border
def color_xslice():
    epix.red()


def fill_xslice():
    epix.rgb(0.8, 0.5, 0.1)


def color_yslice():
    epix.blue()


def fill_yslice():
    epix.rgb(0.6, 0.3, 0)


# function to be graphed
def f(x, y):
    return P(x, y, 0.75 * y * (y - sqrt3 * x) * (y + sqrt3 * x))


# %%
with epix.figure(P(-2, -2), P(2, 1.5), "6 x 5.25in") as fig:
    # "legend"
    epix.masklabel(
        P(epix.xmax(), epix.ymax()),
        P(-2, -2),
        r"$z=\displaystyle\frac{1}{2}(y^3-3x^2y)$",
        epix.LabelPos.bl,
    )

    epix.font_size("scriptsize")
    epix.camera.at(VIEWPT)
    epix.border(epix.Green(0.6), "1pt")

    R = epix.domain(
        P(-MAX, -MAX),
        P(MAX, MAX),
        epix.mesh(4 * MESH, 4 * MESH),
        epix.mesh(8 * MESH, 8 * MESH),
    )

    # coordinate grids
    color_coord()
    epix.grid(P(-MAX, -MAX, -MAX), P(-MAX, MAX, MAX), MESH, MESH)
    epix.grid(P(-MAX, -MAX, -MAX), P(MAX, -MAX, MAX), MESH, MESH)
    epix.grid(P(-MAX, -MAX, -MAX), P(MAX, MAX, -MAX), MESH, MESH)

    epix.clip_box(P(-2, -2, -1), P(2, 2, 1))

    # back half and front left quarter
    color_surf()
    epix.surface(f, R.resize1(-MAX, 0))
    epix.surface(f, R.resize1(0, MAX).resize2(-MAX, 0))

    # coordinate axes
    color_axis()
    epix.bold()

    epix.clip_box(P(-2, -2, -1), P(2, 2, 2))

    epix.dart(P(-MAX, 0, 0), P(0.25 + MAX, 0, 0))
    epix.dart(P(0, -MAX, 0), P(0, 0.25 + MAX, 0))
    epix.dart(P(0, 0, 0), P(0, 0, 0.25 + MAX))

    epix.label(P(0.25 + MAX, 0, 0), P(-2, -2), "$x$", epix.LabelPos.bl)
    epix.label(P(0, 0.25 + MAX, 0), P(4, -2), "$y$", epix.LabelPos.r)
    epix.label(P(0, 0, 0.25 + MAX), P(0, 4), "$z$", epix.LabelPos.t)

    # front quarter of surface; chop into four pieces
    color_surf()
    epix.surface(f, R.resize1(0, x_0).resize2(0, y_0))  # behind both planes

    fill_xslice()
    epix.rect(P(x_0, 0, -MAX), P(x_0, y_0, z_0))  # left part of plane x = x_0

    epix.pen(3)
    color_xslice()
    epix.plot(f, R.slice1(x_0).resize2(0, y_0))

    color_surf()
    epix.surface(f, R.resize1(x_0, MAX).resize2(0, y_0))  # back right piece

    fill_yslice()
    epix.rect(P(0, y_0, -MAX), P(MAX, y_0, z_0))  # plane y = y_0

    epix.pen(3)
    color_yslice()
    epix.plot(f, R.slice2(y_0).resize1(0, MAX))

    color_surf()
    epix.surface(f, R.resize1(0, x_0).resize2(y_0, MAX))  # front left piece

    fill_xslice()
    epix.rect(P(x_0, y_0, -MAX), P(x_0, MAX, z_0))  # right part of plane x = x_0

    epix.pen(3)
    color_xslice()
    epix.plot(f, R.slice1(x_0).resize2(y_0, MAX))

    color_surf()
    epix.surface(f, R.resize1(x_0, MAX).resize2(y_0, MAX))  # front right piece

    epix.clip_box(P(-MAX, -MAX, -2), P(MAX, MAX, 2))

    # labels and graph slices
    color_yslice()
    epix.label(
        f(MAX, y_0),
        P(-4, 0),
        r"$\displaystyle\frac{\partial f}{\partial x}$: $y$ constant",
        epix.LabelPos.l,
    )

    color_xslice()
    epix.label(
        f(x_0, MAX),
        P(4, 0),
        r"$\displaystyle\frac{\partial f}{\partial y}$: $x$ constant",
        epix.LabelPos.br,
    )

    epix.pst_format()
fig
