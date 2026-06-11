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
# # plotting — ported from `samples/plotting.xp`
# Graphing a function, with custom axis tick labels (radian mode).

# %%
from __future__ import annotations

import epix
from epix import Point


def f(x: float) -> float:
    return 0.75 * epix.sin(x) - 0.25 * epix.sin(2 * x)


# %%
with epix.figure(
    lower_left=Point(x=-2, y=-1), upper_right=Point(x=2, y=1), size="5x1in"
) as fig:
    epix.h_axis(16)
    epix.v_axis(4)
    epix.h_axis_labels(4, offset=Point(x=0, y=-4), align=epix.LabelPos.b)
    epix.revolutions()
    epix.set_red()
    epix.plot(f, epix.xmin(), epix.xmax(), n=120)
fig
