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
# # plotting3 — ported from `samples/plotting3.xp`
# sin x and a family of its Taylor polynomials (a 2-variable function sliced).

# %%
from __future__ import annotations

from math import floor, pi

import epix
from epix import Point


def sin_n(
    x: float, n: float
) -> epix.Point:  # Taylor polynomial of sin x, degree from n
    N = int(floor(n))
    sqx: float = -(x**2)
    val: float = x
    summand: float = x
    for i in range(1, 2 * N + 2, 2):
        summand *= sqx / ((i + 1) * (i + 2))
        val += summand
    return Point(x=x, y=val)


# %%
with epix.figure(
    lower_left=Point(x=0, y=-1), upper_right=Point(x=6 * pi, y=1), size="5x1in"
) as fig:
    epix.set_crop()
    epix.h_axis(4)
    epix.v_axis(1)
    epix.bold()
    epix.set_green()
    epix.plot(epix.sin, 0, epix.xmax(), n=120)
    R: epix.Domain = epix.Domain(
        lower_left=Point(x=0, y=1),
        upper_right=Point(x=6 * pi, y=41),
        coarse=epix.Mesh(nx=60, ny=40),
        fine=epix.Mesh(nx=120, ny=40),
    )
    for i in range(4):
        epix.pen(epix.rgb(0.25 * i, 0, 1 - 0.25 * i))
        epix.plot(sin_n, R.slice2(5 * i + 1))
fig
