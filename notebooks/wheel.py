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
# # wheel — ported from `samples/wheel.xp`
# A rolling wheel tracing a cycloid, shown as a stack of panels (`screen`/`inset`).

# %%
from math import ceil, pi

import epix
from epix import P


def F(t, r):
    return P(t - r * epix.Sin(t), 1 - r * epix.Cos(t))  # a cycloid


# %%
with epix.figure(P(0, 0), P(15, 27), "4 x 7.2in") as fig:
    dt = 5 * pi / 11
    t = dt
    for i in range(9):
        scr = epix.screen(P(-1, 0), P(15, 2))  # drawing area
        epix.activate(scr)
        epix.plain(epix.Black())
        epix.line(P(-1, 0), P(15, 0))  # the ground
        epix.circle(P(t, 1), 1)  # the wheel
        R = epix.domain(
            P(0, 0), P(t, 1), epix.mesh(10 * i, 5), epix.mesh(int(ceil(1 + 4 * t)), 5)
        )
        epix.bold()
        for j in range(6):
            epix.pen(epix.RGB(1 - 0.125 * j, 0.125 * j, 0.5 + 0.125 * j))
            epix.plot(F, R.slice2(0.2 * j))
        epix.bold(epix.Green())
        epix.line(P(t, 1), F(t, 1))
        epix.inset(P(0, 25 - 3 * i), P(15, 27 - 3 * i))  # page layout
        epix.deactivate(scr)
        t += dt
fig
