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
# # weierstrass — ported from `samples/weierstrass.xp`
# A Weierstrass-type nowhere-differentiable function (sum of scaled `cb` waves).

# %%
from __future__ import annotations

import epix
from epix import Point

N: int = 8  # number of summands


def weierstrass(t: float) -> float:
    y: int = 0
    for i in range(N):
        y += pow(2, -i) * epix.cb(pow(2, i) * t)
    return y


# %%
with epix.figure(
    lower_left=Point(x=-2, y=0), upper_right=Point(x=2, y=1.5), size="3.2 x 1.2in"
) as fig:
    epix.h_axis(int(2 * epix.xsize()))
    epix.v_axis(int(2 * epix.ysize()))
    epix.h_axis_labels(int(epix.xsize()), offset=Point(x=-4, y=-8))
    epix.set_blue()
    epix.plot(
        epix.cb, epix.xmin() - 0.25, epix.xmax() + 0.25, n=int(4 * epix.xsize() + 2)
    )
    epix.bold(epix.black())
    epix.plot(weierstrass, epix.xmin(), epix.xmax(), n=int(pow(2, N)))
    epix.pen(epix.red())
    epix.plot(weierstrass, 0.5, 1.5, n=int(pow(2, N - 2)))
fig
