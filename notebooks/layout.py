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
# # layout — ported from `samples/layout.xp`
# A 2×3 grid of views of a surface from rotating camera angles (`screen`/`inset`).

# %%
from math import pi

import epix
from epix import P


def f(u, v):
    return P((u - v) * (u + v), 2 * u * v, u)


# %%
with epix.figure(P(0, 0), P(2, 3), "5x7.5in") as fig:
    R = epix.domain(P(-1, -1), P(1, 1), epix.mesh(12, 12), epix.mesh(24, 24))
    for i in range(2):
        for j in range(3):
            my = epix.screen(P(-3, -3), P(3, 3))
            epix.activate(my)
            epix.border(epix.Red(0.6), "1pt")
            epix.camera.at(epix.sph(10, (2 * j + i + 1) * pi / 8, pi / 6))
            epix.plot(f, R)
            epix.arrow(P(0, 0, 0), 2 * epix.E_1)
            epix.arrow(P(0, 0, 0), 2 * epix.E_2)
            epix.inset(P(i + 0.05, 2.05 - j), P(i + 0.95, 2.95 - j))
            epix.deactivate(my)
fig
