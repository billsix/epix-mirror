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
# # inverse — ported from `samples/inverse.xp`
# A function and its inverse (reflection across y=x, via an `affine` map on a `screen`).

# %%
from math import sqrt

import epix
from epix import P

x0 = 0.4
sqrt3 = sqrt(3)


def f(x):
    return 2 * x * (x - sqrt3 * x0) * (x + sqrt3 * x0)


# %%
with epix.figure(P(-2, -1), P(2, 1), "6 x 3in") as fig:
    epix.degrees()
    tmp = epix.screen(P(-1, -1), P(1, 1))
    epix.activate(tmp)
    epix.pen(epix.Black(0.1))
    epix.line(P(-1, -1), P(1, 1))
    epix.pen(epix.Black(0.3))
    epix.aarrow(P(0.4, 0.6), P(0.6, 0.4), 0.5)
    epix.pen(epix.Black())
    epix.font_size("footnotesize")
    epix.h_axis(8)
    epix.v_axis(8)
    epix.masklabel(P(1, 0), "$x$")
    epix.masklabel(P(0, 1), "$y$")
    epix.label(P(0.5, -0.5), "$y=f(x)$")
    epix.label_angle(-90)
    epix.label(P(-0.5, 0.5), "$x=f^{-1}(y)$")
    tmp.set_crop()
    epix.plain(epix.Blue(1.4))
    epix.tan_line(f, x0)
    epix.plain(epix.Red(1.4))
    epix.plot(f, -1, -x0, 20)
    epix.bold(epix.Red())
    epix.plot(f, -x0, x0, 20)
    epix.plain(epix.Red(1.4))
    epix.plot(f, x0, 1, 20)
    epix.inset(tmp, P(-2, -1), P(0, 1))
    af = epix.affine()
    af.reflect(45).shift(P(0.25, 0))
    epix.inset(tmp.apply(af), P(0, -1), P(2, 1))
fig
