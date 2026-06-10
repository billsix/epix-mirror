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
# # polar — ported from `samples/polar.xp`
# A polar rose on a polar grid (degrees mode).

# %%
import epix
from epix import P


def f(t):
    return 2 * epix.Cos(3 * t)


# %%
with epix.figure(P(-2, -2), P(2, 2), "2x2in") as fig:
    epix.degrees()
    epix.black(0.3)
    epix.polar_grid(2, 4, 24)  # radius, rings, sectors
    epix.black()
    epix.bottom_axis(4, P(0, -4)).subdivide(2).draw()
    epix.left_axis(4, P(-4, 0)).subdivide(2).draw()
    epix.bold()
    epix.polarplot(f, 0, 180, 120)
fig
