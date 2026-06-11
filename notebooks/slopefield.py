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
# # slopefield — ported from `samples/slopefield.xp`
# A vector field with several integral curves (`ode_plot`).

# %%
from __future__ import annotations

import epix
from epix import Point


def F(s: float, t: float) -> epix.Point:
    return Point(x=0.1 * s, y=0.025 * t) + (1 / (0.01 + s * s + t * t)) * Point(
        x=-t, y=s
    )


# %%
with epix.figure(
    lower_left=Point(x=-4, y=-3), upper_right=Point(x=2, y=2), size="3x2.5in"
) as fig:
    epix.set_blue(1.8)
    epix.dart_field(
        F,
        lower_left=Point(x=epix.xmin(), y=epix.ymin()),
        upper_right=Point(x=epix.xmax(), y=epix.ymax()),
        nx=int(4 * epix.xsize()),
        ny=int(4 * epix.ysize()),
    )
    epix.set_crop()
    epix.bold()
    for i in range(7):
        epix.set_rgb(0.05 * (7 - i), 1 - 0.1 * i, 0.15 * i)
        epix.ode_plot(F, Point(x=-0.9 - 0.025 * i, y=0), 20, n=200)
fig
