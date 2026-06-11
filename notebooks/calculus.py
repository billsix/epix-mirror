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
# # calculus — ported from `samples/calculus.xp`
#
# A function with its numerical derivative and integral, a boxed legend, and
# trig-labelled axes. Faithful port — the figure is **byte-identical** to the
# C++ original. `Deriv(f)` / `Integral(f, 0)` become `plot_deriv` / `plot_integral`
# on an ordinary Python function.

# %%
from __future__ import annotations

from math import pi

import epix
from epix import Point

MAX = 2 * pi


def f(t: float) -> float:
    return t * epix.sin(t)


# %%
with epix.figure(
    lower_left=Point(x=-MAX, y=-MAX), upper_right=Point(x=MAX, y=MAX), size="240x240pt"
) as fig:
    epix.pen(epix.black(0.3))
    epix.grid(nx=8, ny=8)
    epix.font_size("scriptsize")
    epix.bottom_axis(4, Point(x=0, y=-4)).trig().draw_labels()
    epix.left_axis(4, Point(x=-4, y=0)).trig().draw_labels()

    L: epix.Legend = epix.Legend()
    L.item_border(0).border(epix.red(), 0.4)

    epix.bold(epix.black())
    epix.plot(f, epix.xmin(), epix.xmax(), n=90)
    L.path_item(r"$y=x\sin x$")

    epix.pen(epix.green())
    epix.plot_deriv(f, epix.xmin(), epix.xmax(), n=90)
    L.path_item(r"$y=\frac{d}{dx}(x\sin x)$")

    epix.pen(epix.blue())
    epix.plot_integral(f, 0, epix.xmin(), epix.xmax(), n=90)
    L.path_item(r"$y=\int_0^x t\sin t\,dt$")

    L.draw(loc=Point(x=0, y=2 * pi), offset=Point(x=2, y=-2), align=epix.LabelPos.br)
fig
