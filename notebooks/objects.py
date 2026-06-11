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
# # objects — ported from `samples/objects.xp`
# Basic 2-D objects: triangle, circle, rectangles, a grid.

# %%
from __future__ import annotations

import epix
from epix import Point

# %%
with epix.figure(
    lower_left=Point(x=0, y=0), upper_right=Point(x=5, y=2), size="3.75 x 1.5in"
) as fig:
    epix.triangle(a=Point(x=0.9, y=1), b=Point(x=3.1, y=1), c=Point(x=2, y=1.5))
    epix.circle(center=Point(x=4, y=1.5), radius=0.25)
    epix.rect(lower_left=Point(x=1, y=0), upper_right=Point(x=3, y=1))
    epix.fill(epix.black(0.1))
    epix.rect(lower_left=Point(x=2.3, y=0), upper_right=Point(x=2.7, y=0.8))
    epix.fill(epix.white())
    epix.bold()
    epix.grid(lower_left=Point(x=1.4, y=0.2), upper_right=Point(x=2, y=0.6), nx=2, ny=2)
    epix.line(tail=Point(x=epix.xmin(), y=0), head=Point(x=epix.xmax(), y=0))
fig
