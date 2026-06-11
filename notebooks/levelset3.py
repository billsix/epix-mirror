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
# # levelset3 — ported from `samples/levelset3.xp`
# A Gaussian-modulated saddle `0.9·MAX·e^{-(x²+y²)/2}·xy` drawn as height-banded
# level sets via a stack of thin `clip_slice` planes, each tinted by height and
# captioned in one of three `legend`s (low / middle / high bands).

# %%
from __future__ import annotations

import math

import epix
from epix import Point

MESH: int = 12  # number of coordinate grid squares
MAX: int = 3  # maximum coordinate

sqrt3: float = math.sqrt(3)


# function to be graphed
def f(x: float, y: float) -> epix.Point:
    return Point(x=x, y=y, z=0.9 * MAX * math.exp(-0.5 * (x * x + y * y)) * x * y)


def color(u: float, v: float, w: float) -> epix.Point:
    return Point(x=-0.5 * w, y=0.25 * w, z=0.5 * w)


Bk: epix.Color = epix.green(0.7)

# %%
with epix.figure(
    lower_left=Point(x=-MAX, y=-MAX), upper_right=Point(x=MAX, y=MAX), size="6 x 6in"
) as fig:
    epix.backing(Bk)

    epix.camera.at(0, -10, 4)

    epix.border(epix.green(0.6), width="1pt")

    epix.clip_box(Point(x=MAX, y=MAX, z=MAX))

    # the C++ 3rd ctor arg is the comma-expression `(6*MESH, 6*MESH)` == 72,
    # which converts to mesh(72); reproduced here as epix.Mesh(6 * MESH).
    R: epix.Domain = epix.Domain(
        lower_left=Point(x=-MAX, y=-MAX),
        upper_right=Point(x=MAX, y=MAX),
        coarse=epix.Mesh(6 * MESH),
        fine=epix.Mesh(6 * MESH, 6 * MESH),
    )

    L1: epix.Legend = epix.Legend()
    L2: epix.Legend = epix.Legend()
    L3: epix.Legend = epix.Legend()
    L1.backing(Bk)
    L2.backing(Bk)
    L3.backing(Bk)

    for i in range(-10, 11):
        epix.clip_slice(
            loc=Point(x=0, y=0, z=0.1 * i), normal=Point(x=0, y=0, z=1), thickness=0.1
        )
        epix.set_rgb(0.25 + 0.1 * i, 0, -0.1 * i)

        buf = rf"${0.1 * i - 0.05:g}\leq z \leq {0.1 * i + 0.05:g}$"

        if i < -3:
            L1.fill_item(buf)
        elif i < 4:
            L2.fill_item(buf)
        else:
            L3.fill_item(buf)

        epix.nofill()
        epix.surface(f, R)
        epix.clip_restore()

    epix.camera.at(0, 0, 1000)
    epix.font_size("scriptsize")
    epix.label_color(epix.black())
    L1.border(0)
    L2.border(0)
    L3.border(0)

    L1.draw(loc=epix.canvas().bl(), offset=Point(x=2, y=2), align=epix.LabelPos.tr)
    L2.draw(
        loc=epix.canvas().bl() + Point(x=2, y=0),
        offset=Point(x=2, y=2),
        align=epix.LabelPos.tr,
    )
    L3.draw(
        loc=epix.canvas().bl() + Point(x=4, y=0),
        offset=Point(x=2, y=2),
        align=epix.LabelPos.tr,
    )

    epix.pst_format()
fig
