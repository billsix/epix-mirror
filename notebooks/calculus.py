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
import epix
from epix import P
from math import pi

MAX = 2 * pi


def f(t):
    return t * epix.Sin(t)


# %%
with epix.figure(P(-MAX, -MAX), P(MAX, MAX), "240x240pt") as fig:
    epix.pen(epix.Black(0.3))
    epix.grid(8, 8)
    epix.font_size("scriptsize")
    epix.bottom_axis(4, P(0, -4)).trig().draw_labels()
    epix.left_axis(4, P(-4, 0)).trig().draw_labels()

    L = epix.legend()
    L.item_border(0).border(epix.Red(), 0.4)

    epix.bold(epix.Black())
    epix.plot(f, epix.xmin(), epix.xmax(), 90)
    L.path_item(r"$y=x\sin x$")

    epix.pen(epix.Green())
    epix.plot_deriv(f, epix.xmin(), epix.xmax(), 90)
    L.path_item(r"$y=\frac{d}{dx}(x\sin x)$")

    epix.pen(epix.Blue())
    epix.plot_integral(f, 0, epix.xmin(), epix.xmax(), 90)
    L.path_item(r"$y=\int_0^x t\sin t\,dt$")

    L.draw(P(0, 2 * pi), P(2, -2), epix.LabelPos.br)
fig
