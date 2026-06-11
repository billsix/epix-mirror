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
# # sphere — ported from `samples/sphere.xp`
#
# Loxodromes (constant-bearing spirals) on a sphere, with hidden/visible 3-D
# plotting, a lat/long grid, and a perspective camera. Faithful port — the figure
# is **byte-identical** to the C++ original. Note: `epix.cos`/`epix.sin` honour
# the `degrees()` mode (don't use `math.cos` here).

# %%
from __future__ import annotations

from math import exp, pi, sqrt

import epix
from epix import Point

k = 2 * pi / (360 * sqrt(3))  # loxodrome pitch (degrees mode)


def exp_cos(t: float) -> float:
    return exp(k * t) * epix.cos(t)


def exp_sin(t: float) -> float:
    return exp(k * t) * epix.sin(t)


def minus_exp_cos(t: float) -> float:
    return -exp_cos(t)


def minus_exp_sin(t: float) -> float:
    return -exp_sin(t)


# %%
with epix.figure(
    lower_left=Point(x=-1, y=-1), upper_right=Point(x=1, y=1), size="2.5 x 2.5in"
) as fig:
    epix.degrees()
    epix.camera.at(Point(x=1, y=2.5, z=3))
    epix.sphere()  # unit sphere's horizon

    epix.pen(epix.blue(1.6))  # hidden portions
    epix.backplot_N(exp_cos, exp_sin, -540, 540, n=180)
    epix.backplot_N(minus_exp_cos, minus_exp_sin, -540, 540, n=180)
    epix.pen(epix.red(1.6))
    epix.backplot_N(exp_sin, minus_exp_cos, -540, 540, n=180)
    epix.backplot_N(minus_exp_sin, exp_cos, -540, 540, n=180)

    epix.pen(epix.black(0.3))  # coordinate grid
    for i in range(13):
        epix.latitude(lat=90 - 15 * i, long_min=0, long_max=360)
        epix.longitude(lng=30 * i, lat_min=0, lat_max=360)

    epix.bold(epix.blue())  # visible portions
    epix.frontplot_N(exp_cos, exp_sin, -540, 540, n=360)
    epix.frontplot_N(minus_exp_cos, minus_exp_sin, -540, 540, n=360)
    epix.pen(epix.red())
    epix.frontplot_N(exp_sin, minus_exp_cos, -540, 540, n=360)
    epix.frontplot_N(minus_exp_sin, exp_cos, -540, 540, n=360)
fig
