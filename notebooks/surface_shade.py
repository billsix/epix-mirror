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
# # surface_shade — ported from `samples/surface_shade.xp`
# A monkey-saddle surface shaded by a position-dependent color function.

# %%
from math import tanh

import epix
from epix import P


def f(u, v):
    return P(u, v, 0.5 * u * v * (u - v) * (u + v))


def color(x, y, z):
    return P(-tanh(z), 0, tanh(z))  # quasi-geographic colors


# %%
with epix.figure(P(-3, -3), P(3, 3), "6x6in") as fig:
    epix.revolutions()
    epix.set_crop()
    epix.fill()
    epix.backing(epix.Black())
    epix.plain(epix.Yellow())
    epix.camera.at(P(8, -8, 4))
    R = epix.domain(P(-2, -2), P(2, 2), epix.mesh(36, 36), epix.mesh(72, 72))
    epix.surface(f, R, color)
fig
