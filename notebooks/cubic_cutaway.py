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
# # cubic_cutaway — ported from `samples/cubic_cutaway.xp`
# The three branches of the cube root `z^3 = 1 − w²` over the complex `w`-plane,
# assembled as one colored `scenery` and drawn twice (lower-back then upper-front)
# with `clip_box` for manual hidden-surface removal, plus the real cross-sections.

# %%
from __future__ import annotations

import math

import epix
from epix import Point

ht = 2  # truncation height
sz = 2  # domain size
sq = 12  # domain mesh
eps = 0.0000001


def poly(x: float, y: float) -> epix.Complex:
    return epix.Complex(1 - x, -y) * epix.Complex(1 + x, y)  # 1 - (x + iy)^2


# z^3 = 1 - w^2, w = x + iy -- three branches
def f0(x: float, y: float) -> epix.Point:
    return epix.root_complex(poly(x, y), 3) + x * epix.E_3


def f1(x: float, y: float) -> epix.Point:
    return epix.root_complex(poly(x, y), 3, 1) + x * epix.E_3


def f2(x: float, y: float) -> epix.Point:
    return epix.root_complex(poly(x, y), 3, 2) + x * epix.E_3


# real sections
def fp(t: float) -> epix.Point:
    return Point(x=t, y=0, z=math.sqrt(1 - t * t * t))


def fm(t: float) -> epix.Point:
    return Point(x=t, y=0, z=-math.sqrt(1 - t * t * t))


# coloring
def dens(d: float) -> float:
    return 0.5 * (1 + math.tanh(d))


def color(u: float, v: float) -> epix.Point:
    return Point(x=dens(v), y=0.2, z=1 - dens(v))


# pen styles; called multiple times, so factored out
def real_section() -> None:
    epix.bbold(epix.magenta(0.6))
    epix.base(epix.rgb(0.8, 0, 1), width=2.5)


def mesh_pen() -> None:
    epix.plain(epix.rgb(1, 0.8, 0.5))  # surface mesh


def grid_pen() -> None:
    epix.plain(epix.black(0.4))  # coordinate grid


def axis_pen() -> None:
    epix.bold(epix.black())  # axes


# lower and upper half-planes
LHP: epix.Domain = epix.Domain(
    lower_left=Point(x=-sz, y=-sz),
    upper_right=Point(x=sz, y=-eps),
    coarse=epix.Mesh(nx=2 * sq, ny=sq),
    fine=epix.Mesh(nx=10 * sq, ny=5 * sq),
)
UHP: epix.Domain = epix.Domain(
    lower_left=Point(x=-sz, y=eps),
    upper_right=Point(x=sz, y=sz),
    coarse=epix.Mesh(nx=2 * sq, ny=sq),
    fine=epix.Mesh(nx=10 * sq, ny=5 * sq),
)

# %%
with epix.figure(
    lower_left=Point(x=-2, y=-2), upper_right=Point(x=2, y=2), size="6 x 6in"
) as fig:
    epix.camera.at(Point(x=4, y=8, z=4)).look_at(Point(x=0.5, y=0, z=0.5))

    epix.fill()
    mesh_pen()

    # three branches of cube root over each half-plane
    sc: epix.Scenery = epix.Scenery(f0, LHP, color)
    sc.add(f1, LHP, color)
    sc.add(f2, LHP, color)

    sc.add(f0, UHP, color)
    sc.add(f1, UHP, color)
    sc.add(f2, UHP, color)

    # Manual object hiding: draw lower back portion
    epix.clip_box(lower_left=Point(x=-2, y=-2, z=-ht), upper_right=Point(x=2, y=0, z=0))
    sc.draw()

    # lower portion of real section
    real_section()
    epix.plot(fm, epix.xmin(), 1, n=240)

    epix.base(epix.neutral(), width=0)

    # Re w axis and z-grid
    epix.clip_box()
    epix.nofill()
    axis_pen()
    epix.line(tail=Point(x=0, y=0, z=-sz), head=Point(x=0, y=0, z=0))

    grid_pen()
    epix.grid(lower_left=Point(x=-2, y=-2), upper_right=Point(x=2, y=2), nx=24, ny=24)

    # Im z axis
    axis_pen()
    epix.arrow(tail=Point(x=0, y=-1.1 * sz, z=0), head=Point(x=0, y=1.1 * sz, z=0))

    # upper back portion
    epix.fill()
    epix.clip_box(lower_left=Point(x=-2, y=-2, z=0), upper_right=Point(x=2, y=0, z=ht))
    sc.draw()

    # upper portion of real section
    real_section()
    epix.plot(fp, epix.xmin(), 1, n=240)

    epix.base(epix.neutral(), width=0)

    # remaining axes, labels
    epix.clip_box()
    axis_pen()
    epix.arrow(tail=Point(x=-1.1 * sz, y=0, z=0), head=Point(x=1.1 * sz, y=0, z=0))
    epix.arrow(tail=Point(x=0, y=0, z=0), head=Point(x=0, y=0, z=sz))

    epix.label(
        Point(x=1.1 * sz, y=0),
        offset=Point(x=-2, y=-4),
        text=r"$\operatorname{Re} z$",
        align=epix.LabelPos.l,
    )
    epix.label(
        Point(x=0, y=1.1 * sz),
        offset=Point(x=2, y=-2),
        text=r"$\operatorname{Im} z$",
        align=epix.LabelPos.br,
    )
    epix.label(
        Point(x=0, y=0, z=sz),
        offset=Point(x=0, y=4),
        text=r"$\operatorname{Re} \sqrt{1-z^3}$",
        align=epix.LabelPos.t,
    )

    epix.pst_format()
fig
