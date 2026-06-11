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
# # butterfly — ported from `samples/butterfly.xp`
# The cusp ("butterfly") catastrophe surface `Phi(u,v)` with the twisted-cubic
# cusp locus and its three coordinate-plane projections, drawn in a cropped 3-D
# scene with background grids and axes.

# %%
from __future__ import annotations

import math

import epix
from epix import Point

VIEWPT = Point(x=20, y=0, z=8)

tmin = 2 / math.sqrt(3)

light = epix.blue(1.6)
dark = epix.red(0.8)
lt_gray = epix.black(0.2)
net_color = epix.yellow(0.1)


def label_tag(loc: epix.Point, head: epix.Point, txt: str, align) -> None:
    epix.dart(tail=loc, head=head)
    epix.masklabel(loc, offset=Point(x=0, y=0), text=txt, align=align)


# surface
def Phi(u: float, v: float) -> epix.Point:
    return Point(x=4 * v, y=0.75 * (u * u - 2 * v) * u, z=2 * u)


def cubic(t: float) -> epix.Point:
    return Phi(t, 1.5 * t * t)


# cubic projections to background planes
def Phi12(t: float) -> epix.Point:
    cusp_pt = cubic(t)
    return Point(x=cusp_pt.x1(), y=cusp_pt.x2(), z=-8)


def Phi13(t: float) -> epix.Point:
    cusp_pt = cubic(t)
    return Point(x=cusp_pt.x1(), y=-8, z=cusp_pt.x3())


def Phi23(t: float) -> epix.Point:
    cusp_pt = cubic(t)
    return Point(x=-8, y=cusp_pt.x2(), z=cusp_pt.x3())


R: epix.Domain = epix.Domain(
    lower_left=Point(x=-2, y=-2),
    upper_right=Point(x=2, y=2),
    coarse=epix.Mesh(nx=20, ny=20),
    fine=epix.Mesh(nx=40, ny=40),
)
edge: epix.Domain = epix.Domain(
    lower_left=Point(x=-2, y=-2),
    upper_right=Point(x=2, y=2),
    coarse=epix.Mesh(nx=1, ny=1),
    fine=epix.Mesh(nx=40, ny=40),
)

# %%
with epix.figure(
    lower_left=Point(x=-14, y=-15), upper_right=Point(x=10, y=9), size="6x6in"
) as fig:
    epix.set_crop()
    epix.camera.at(20, 0, 8)

    # background grids
    epix.pen(lt_gray)
    epix.grid(
        lower_left=Point(x=-8, y=-8, z=-8),
        upper_right=Point(x=8, y=8, z=-8),
        nx=8,
        ny=8,
    )
    epix.grid(
        lower_left=Point(x=-8, y=-8, z=-8),
        upper_right=Point(x=8, y=-8, z=6),
        nx=7,
        ny=8,
    )  # n.b. (z,x) divisions
    epix.grid(
        lower_left=Point(x=-8, y=-8, z=-8),
        upper_right=Point(x=-8, y=8, z=6),
        nx=8,
        ny=7,
    )

    # background coordinate axes
    epix.pen(epix.black())
    epix.line(tail=Point(x=0, y=-8, z=-8), head=Point(x=0, y=8, z=-8))
    epix.line(tail=Point(x=-8, y=0, z=-8), head=Point(x=8, y=0, z=-8))

    epix.line(tail=Point(x=0, y=-8, z=-8), head=Point(x=0, y=-8, z=6))
    epix.line(tail=Point(x=-8, y=-8, z=0), head=Point(x=8, y=-8, z=0))

    epix.line(tail=Point(x=-8, y=0, z=-8), head=Point(x=-8, y=0, z=6))
    epix.line(tail=Point(x=-8, y=-8, z=0), head=Point(x=-8, y=8, z=0))

    # projections of cubic
    epix.bold(dark)
    epix.plot(Phi12, -tmin, tmin, n=80)
    epix.plot(Phi13, -tmin, tmin, n=80)
    epix.plot(Phi23, -tmin, tmin, n=80)

    # surface
    epix.plain(net_color)
    epix.fill(lt_gray)
    epix.surface(Phi, R.resize1(-2, 0))  # bottom half

    # and cubic curve
    epix.bold(dark)
    epix.plot(cubic, -tmin, 0, n=80)

    epix.plain(net_color)
    epix.surface(Phi, R.resize1(0, 2))  # top half

    epix.bold(dark)
    epix.plot(cubic, 0, tmin, n=80)

    # surface boundary
    epix.bold(epix.yellow())
    epix.plot(Phi, edge)

    # guide lines to cusp projection
    epix.plain(light)
    epix.dashed()
    epix.line(tail=cubic(tmin), head=Phi12(tmin))
    epix.line(tail=cubic(-tmin), head=Phi12(-tmin))

    # fake transparency
    epix.plain(dark)
    epix.plot(Phi12, -tmin, tmin, n=20)  # cusp

    epix.label_color(epix.black())
    epix.label(
        Point(x=8, y=0, z=-8),
        offset=Point(x=0, y=-4),
        text="$x$",
        align=epix.LabelPos.b,
    )
    epix.label(
        Point(x=0, y=8, z=-8), offset=Point(x=4, y=0), text="$y$", align=epix.LabelPos.r
    )

    epix.pen(light)
    epix.label_border(light, "0.4pt")
    epix.solid()
    label_tag(
        Phi(-2, -2) + Point(x=0, y=2, z=6),
        Phi(-1.75, -1.75),
        r"$y=\frac{3}{32}(z^3-2xz)$",
        epix.LabelPos.t,
    )
    label_tag(
        cubic(-1) + Point(x=0, y=2, z=3),
        cubic(-1),
        r"$(x,y,z)=(6t^2, -\frac{3}{2}t^3, 2t)$",
        epix.LabelPos.t,
    )

    epix.pst_format()
fig
