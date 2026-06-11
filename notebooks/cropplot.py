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
# # cropplot — ported from `samples/cropplot.xp`
# Graphs cropped to a viewport; the second curve has poles at sample points
# (note `g`'s `inf` guard to match C++ double division at the pole).

# %%
from __future__ import annotations

import epix
from epix import Point


def f(t: float) -> float:
    return 2 * t * (1 - t) * (1 - t)


def g(t: float) -> float:
    d: float = 1 - t * t
    return 1.0 / d if d != 0.0 else float("inf")


# %%
with epix.figure(
    lower_left=Point(x=-2, y=-4), upper_right=Point(x=2, y=4), size="200x200pt"
) as fig:
    epix.set_crop()
    epix.dashed()
    epix.line(tail=Point(x=-1, y=epix.ymin()), head=Point(x=-1, y=epix.ymax()))
    epix.line(tail=Point(x=1, y=epix.ymin()), head=Point(x=1, y=epix.ymax()))
    epix.solid()
    epix.h_axis(8)
    epix.v_axis(8)
    epix.h_axis_labels(n=4, offset=Point(x=-1, y=2), align=epix.LabelPos.tl)
    epix.v_axis_labels(n=4, offset=Point(x=-1, y=2), align=epix.LabelPos.tl)
    epix.plot(f, epix.xmin(), epix.xmax(), n=80)
    epix.bold()
    epix.plot(g, epix.xmin(), epix.xmax(), n=80)
fig
