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
import epix
from epix import P

# %%
with epix.figure(P(0, 0), P(5, 2), "3.75 x 1.5in") as fig:
    epix.triangle(P(0.9, 1), P(3.1, 1), P(2, 1.5))
    epix.circle(P(4, 1.5), 0.25)
    epix.rect(P(1, 0), P(3, 1))
    epix.fill(epix.Black(0.1))
    epix.rect(P(2.3, 0), P(2.7, 0.8))
    epix.fill(epix.White())
    epix.bold()
    epix.grid(P(1.4, 0.2), P(2, 0.6), 2, 2)
    epix.line(P(epix.xmin(), 0), P(epix.xmax(), 0))
fig
