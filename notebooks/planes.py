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
# # planes — ported from `samples/planes.xp`
# A stack of clipped, color-graded planes (the `Plane` class).

# %%
import epix
from epix import P

N = 20

# %%
with epix.figure(P(-2, -2), P(2, 2), "6x6in") as fig:
    epix.border()
    epix.fill()
    epix.camera.at(P(10, -6, 4))
    epix.clip_box(P(-1, -1, -1), P(1, 1, 1))
    C = epix.Plane(P(-1, -1, -1), P(1, 1, 1))
    for i in range(1, N):
        epix.rgb(i * 1.0 / N, 0.2, 1 - i * 1.0 / N)
        C.shift((2.0 / N) * P(1, 1, 1))
        C.draw()
fig
