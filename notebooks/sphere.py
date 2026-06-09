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
# is **byte-identical** to the C++ original. Note: `epix.Cos`/`epix.Sin` honour
# the `degrees()` mode (don't use `math.cos` here).

# %%
import epix
from epix import P
from math import pi, sqrt, exp

k = 2 * pi / (360 * sqrt(3))   # loxodrome pitch (degrees mode)


def exp_cos(t): return exp(k * t) * epix.Cos(t)
def exp_sin(t): return exp(k * t) * epix.Sin(t)
def minus_exp_cos(t): return -exp_cos(t)
def minus_exp_sin(t): return -exp_sin(t)


# %%
with epix.figure(P(-1, -1), P(1, 1), "2.5 x 2.5in") as fig:
    epix.degrees()
    epix.camera.at(P(1, 2.5, 3))
    epix.sphere()                         # unit sphere's horizon

    epix.pen(epix.Blue(1.6))              # hidden portions
    epix.backplot_N(exp_cos, exp_sin, -540, 540, 180)
    epix.backplot_N(minus_exp_cos, minus_exp_sin, -540, 540, 180)
    epix.pen(epix.Red(1.6))
    epix.backplot_N(exp_sin, minus_exp_cos, -540, 540, 180)
    epix.backplot_N(minus_exp_sin, exp_cos, -540, 540, 180)

    epix.pen(epix.Black(0.3))             # coordinate grid
    for i in range(13):
        epix.latitude(90 - 15 * i, 0, 360)
        epix.longitude(30 * i, 0, 360)

    epix.bold(epix.Blue())               # visible portions
    epix.frontplot_N(exp_cos, exp_sin, -540, 540, 360)
    epix.frontplot_N(minus_exp_cos, minus_exp_sin, -540, 540, 360)
    epix.pen(epix.Red())
    epix.frontplot_N(exp_sin, minus_exp_cos, -540, 540, 360)
    epix.frontplot_N(minus_exp_sin, exp_cos, -540, 540, 360)
fig
