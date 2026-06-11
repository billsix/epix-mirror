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
from __future__ import annotations

from math import sqrt

import epix
from epix import Point

x0: float = 0.4
sqrt3: float = sqrt(3)


def f(x: float) -> float:
    return 2 * x * (x - sqrt3 * x0) * (x + sqrt3 * x0)


# %%
with epix.figure(
    lower_left=Point(x=-2, y=-1), upper_right=Point(x=2, y=1), size="6 x 3in"
) as fig:
    epix.degrees()
    panel: epix.Screen = epix.Screen(
        lower_left=Point(x=-1, y=-1), upper_right=Point(x=1, y=1)
    )
    epix.activate(panel)
    epix.pen(epix.black(0.1))
    epix.line(tail=Point(x=-1, y=-1), head=Point(x=1, y=1))
    epix.pen(epix.black(0.3))
    epix.aarrow(tail=Point(x=0.4, y=0.6), head=Point(x=0.6, y=0.4), scale=0.5)
    epix.pen(epix.black())
    epix.font_size("footnotesize")
    epix.h_axis(n=8)
    epix.v_axis(n=8)
    epix.masklabel(Point(x=1, y=0), "$x$")
    epix.masklabel(Point(x=0, y=1), "$y$")
    epix.label(Point(x=0.5, y=-0.5), "$y=f(x)$")
    epix.label_angle(-90)
    epix.label(Point(x=-0.5, y=0.5), "$x=f^{-1}(y)$")
    panel.set_crop()
    epix.plain(epix.blue(1.4))
    epix.tan_line(f, x0)
    epix.plain(epix.red(1.4))
    epix.plot(f, -1, -x0, n=20)
    epix.bold(epix.red())
    epix.plot(f, -x0, x0, n=20)
    epix.plain(epix.red(1.4))
    epix.plot(f, x0, 1, n=20)
    epix.inset(child=panel, lower_left=Point(x=-2, y=-1), upper_right=Point(x=0, y=1))
    af: epix.Affine = epix.Affine()
    af.reflect(45).shift(Point(x=0.25, y=0))
    epix.inset(
        child=panel.apply(af), lower_left=Point(x=0, y=-1), upper_right=Point(x=2, y=1)
    )
fig
