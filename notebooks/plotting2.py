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
# # plotting2 — ported from `samples/plotting2.xp`
# A parametric surface over a polar domain, with coordinate arrows.

# %%
import epix
from epix import P
from math import pi


def f(r, th):
    return P(r * epix.Cos(th), r * epix.Sin(th), pow(r, 3) * epix.Cos(3 * th))


# %%
with epix.figure(P(-1, -1), P(1, 1), "2x1in") as fig:
    R = epix.domain(P(0, 0), P(1, 2 * pi), epix.mesh(8, 40), epix.mesh(16, 120))
    epix.camera.at(3, 1, 2)
    epix.arrow(P(0, 0, 0), P(1.25, 0, 0))
    epix.arrow(P(0, 0, 0), P(0, 1.25, 0))
    epix.plain(epix.Blue(1.2))
    epix.fill(epix.Yellow())
    epix.surface(f, R)
fig
