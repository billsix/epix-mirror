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
from __future__ import annotations

import epix
from epix import Point

N: int = 20

# %%
with epix.figure(
    lower_left=Point(x=-2, y=-2), upper_right=Point(x=2, y=2), size="6x6in"
) as fig:
    epix.border()
    epix.fill()
    epix.camera.at(Point(x=10, y=-6, z=4))
    epix.clip_box(lower_left=Point(x=-1, y=-1, z=-1), upper_right=Point(x=1, y=1, z=1))
    C: epix.Plane = epix.Plane(Point(x=-1, y=-1, z=-1), Point(x=1, y=1, z=1))
    for i in range(1, N):
        epix.set_rgb(i * 1.0 / N, 0.2, 1 - i * 1.0 / N)
        C.shift((2.0 / N) * Point(x=1, y=1, z=1))
        C.draw()
fig
