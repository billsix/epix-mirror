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
import math

import epix
from epix import P


def sin_n(x, n):  # Taylor polynomial of sin x
    N = int(math.floor(n))  # convert n to an index bound
    sqx = -(x**2)  # store -x^2 for efficiency
    val = x  # places to store results of evaluation
    summand = x

    # step through odd numbers from 1 to 2N+1
    for i in range(1, 2 * N + 2, 2):
        summand *= sqx / ((i + 1) * (i + 2))  # (-1)^i x^{2i+1}/(2i+1)!
        val += summand

    return P(x, val)


# %%
with epix.figure(P(0, -2.5), P(6 * math.pi, 1), "5x3.5in") as fig:
    epix.camera.filter(epix.CMY_Neutral())
    epix.set_crop()
    epix.plain(epix.Black(0.5))
    epix.grid(12, 7)

    # coordinate axis
    Ax = epix.axis(P(0, 0), P(6 * math.pi, 0), 12, P(0, -4), epix.LabelPos.b)
    Ax.trig().subdivide(4)

    Ay = epix.axis(P(0, epix.ymin()), P(0, epix.ymax()), 7, P(-4, 0), epix.LabelPos.l)
    Ay.frac().draw_labels()

    # define a legend and set global attributes
    L = epix.legend()
    L.backing(epix.Yellow(0.1)).border(epix.Blue(), 1).item_border(0)

    epix.bbold(epix.Green())
    epix.plot(epix.Sin, 0, epix.xmax(), 120)

    # add a legend item
    L.path_item(r"$y=\sin x$")

    R = epix.domain(P(0, 1), P(6 * math.pi, 41), epix.mesh(60, 40), epix.mesh(240, 40))

    # print in descending degree so paths overlap better
    for i in range(3, -1, -1):
        epix.pen(epix.RGB(0.25 * i, 0, 1 - 0.25 * i))
        epix.plot(sin_n, R.slice2(5 * i + 1))

        # add a legend item; key set automatically
        L.path_item(f"$y=p_{{{10 * i + 5}}}(x)$")

    epix.plain(epix.Black())
    epix.label_mask(epix.White())
    Ax.draw()

    # place at bottom left
    L.draw(epix.canvas().bl(), P(2, 2), epix.LabelPos.tr)

    epix.pst_format()
fig
