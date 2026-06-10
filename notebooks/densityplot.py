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
# # densityplot — ported from `samples/densityplot.xp`
# An interference pattern rendered as a color-shaded surface (viewed top-down).

# %%
from math import sqrt

import epix
from epix import P

MAX = 18


def ripple(u, v):
    return pow(epix.Cos(0.5 * sqrt(u * u + v * v)), 2)


def f(u, v):
    return 0.25 * (1 + ripple(u, v - 10) + ripple(u, v + 10))


def F(u, v):
    return P(u, v, f(u, v))


def color(u, v, w):
    return P(0.2 + 0.8 * w, 0.3, 0.8 - 0.6 * w)


R = epix.domain(P(-MAX, -MAX), P(MAX, MAX), epix.mesh(2 * MAX, 2 * MAX))

# %%
with epix.figure(P(-MAX, -MAX), P(MAX, MAX), "3x3in") as fig:
    epix.pen(epix.Magenta(1.4), 0.2)
    epix.surface(F, R, color)
fig
