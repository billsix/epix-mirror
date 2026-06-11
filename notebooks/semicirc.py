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
# # semicirc — ported from `samples/semicirc.xp`
# A semicircle with an inscribed rectangle.

# %%
from __future__ import annotations

from math import pi, sqrt

import epix
from epix import Point


def f(x: float) -> float:
    return sqrt(1 - x * x)


width: float = 0.6
height: float = f(width)
posn: float = 0.5
y_posn: float = f(posn)

# %%
with epix.figure(
    lower_left=Point(x=-1, y=0), upper_right=Point(x=1, y=1), size="2.5 x 1.25in"
) as fig:
    epix.h_axis(4)
    epix.v_axis(2)
    epix.h_axis_labels(
        int(epix.xsize()), offset=Point(x=0, y=-4), align=epix.LabelPos.b
    )
    epix.arc(center=Point(x=0, y=0), radius=1, start=0, finish=pi)
    epix.bold()
    epix.rect(lower_left=Point(x=-width, y=0), upper_right=Point(x=width, y=height))
    epix.label(
        Point(x=posn, y=y_posn),
        offset=Point(x=2, y=4),
        text=r"$y=\sqrt{1-x^2}$",
        align=epix.LabelPos.tr,
    )
fig
