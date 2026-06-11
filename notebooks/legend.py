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
# # legend — ported from `samples/legend.xp`
# `sin x` and its Taylor polynomials of degree 5, 15, 25, 35, captioned by a
# boxed `legend` whose keys are filled automatically as items are added.

# %%
from __future__ import annotations

import math

import epix
from epix import Point


def sin_n(x: float, n: int) -> epix.Point:  # Taylor polynomial of sin x
    N = int(math.floor(n))  # convert n to an index bound
    sqx = -(x**2)  # store -x^2 for efficiency
    val = x  # places to store results of evaluation
    summand = x

    # step through odd numbers from 1 to 2N+1
    for i in range(1, 2 * N + 2, 2):
        summand *= sqx / ((i + 1) * (i + 2))  # (-1)^i x^{2i+1}/(2i+1)!
        val += summand

    return Point(x=x, y=val)


# %%
with epix.figure(
    lower_left=Point(x=0, y=-2.5), upper_right=Point(x=6 * math.pi, y=1), size="5x3.5in"
) as fig:
    epix.camera.filter(epix.cmy_neutral())
    epix.set_crop()
    epix.plain(epix.black(0.5))
    epix.grid(nx=12, ny=7)

    # coordinate axis
    Ax: epix.Axis = epix.Axis(
        Point(x=0, y=0),
        Point(x=6 * math.pi, y=0),
        n=12,
        offset=Point(x=0, y=-4),
        align=epix.LabelPos.b,
    )
    Ax.trig().subdivide(n=4)

    Ay: epix.Axis = epix.Axis(
        Point(x=0, y=epix.ymin()),
        Point(x=0, y=epix.ymax()),
        n=7,
        offset=Point(x=-4, y=0),
        align=epix.LabelPos.l,
    )
    Ay.frac().draw_labels()

    # define a legend and set global attributes
    L: epix.Legend = epix.Legend()
    L.backing(epix.yellow(0.1)).border(epix.blue(), width=1).item_border(0)

    epix.bbold(epix.green())
    epix.plot(epix.sin, 0, epix.xmax(), n=120)

    # add a legend item
    L.path_item(r"$y=\sin x$")

    R: epix.Domain = epix.Domain(
        lower_left=Point(x=0, y=1),
        upper_right=Point(x=6 * math.pi, y=41),
        coarse=epix.Mesh(nx=60, ny=40),
        fine=epix.Mesh(nx=240, ny=40),
    )

    # print in descending degree so paths overlap better
    for i in range(3, -1, -1):
        epix.pen(epix.rgb(0.25 * i, 0, 1 - 0.25 * i))
        epix.plot(sin_n, R.slice2(5 * i + 1))

        # add a legend item; key set automatically
        L.path_item(f"$y=p_{{{10 * i + 5}}}(x)$")

    epix.plain(epix.black())
    epix.label_mask(epix.white())
    Ax.draw()

    # place at bottom left
    L.draw(loc=epix.canvas().bl(), offset=Point(x=2, y=2), align=epix.LabelPos.tr)

    epix.pst_format()
fig
