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
# # layout2 — ported from `samples/layout2.xp`
# A 3×2 grid of a vector field with integral curves at varying parameters.

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
    lower_left=Point(x=0, y=0), upper_right=Point(x=2, y=3), size="5x7in"
) as fig:
    for k in range(3):
        for j in range(2):
            panel: epix.Screen = epix.Screen(
                lower_left=Point(x=-4, y=-3), upper_right=Point(x=2, y=2)
            )
            epix.activate(panel)
            epix.border(epix.red(0.6), width="1pt")
            epix.set_crop()
            epix.plain(epix.blue(1.8))
            epix.dart_field(
                F,
                lower_left=Point(x=-4, y=-3),
                upper_right=Point(x=2, y=2),
                nx=12,
                ny=10,
            )
            epix.bold()
            for i in range(7):
                epix.pen(epix.rgb(0.05 * (7 - i), 1 - 0.1 * i, 0.15 * i))
                epix.ode_plot(
                    F,
                    Point(x=-0.9 - 0.025 * i, y=0),
                    4 * (3 * j + k + 0.5),
                    n=int(30 * (3 * j + k + 0.5)),
                )
            epix.inset(
                lower_left=Point(x=j + 0.05, y=1.05 - k),
                upper_right=Point(x=j + 0.95, y=1.95 - k),
            )
            epix.deactivate(panel)
fig
