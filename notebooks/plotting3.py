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
from math import floor, pi

import epix
from epix import P


def sin_n(x, n):  # Taylor polynomial of sin x, degree from n
    N = int(floor(n))
    sqx = -(x**2)
    val = x
    summand = x
    for i in range(1, 2 * N + 2, 2):
        summand *= sqx / ((i + 1) * (i + 2))
        val += summand
    return P(x, val)


# %%
with epix.figure(P(0, -1), P(6 * pi, 1), "5x1in") as fig:
    epix.set_crop()
    epix.h_axis(4)
    epix.v_axis(1)
    epix.bold()
    epix.green()
    epix.plot(epix.Sin, 0, epix.xmax(), 120)
    R = epix.domain(P(0, 1), P(6 * pi, 41), epix.mesh(60, 40), epix.mesh(120, 40))
    for i in range(4):
        epix.pen(epix.RGB(0.25 * i, 0, 1 - 0.25 * i))
        epix.plot(sin_n, R.slice2(5 * i + 1))
fig
