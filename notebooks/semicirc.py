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
from math import pi, sqrt

import epix
from epix import P


def f(x):
    return sqrt(1 - x * x)


width = 0.6
height = f(width)
posn = 0.5
y_posn = f(posn)

# %%
with epix.figure(P(-1, 0), P(1, 1), "2.5 x 1.25in") as fig:
    epix.h_axis(4)
    epix.v_axis(2)
    epix.h_axis_labels(int(epix.xsize()), P(0, -4), epix.LabelPos.b)
    epix.arc(P(0, 0), 1, 0, pi)
    epix.bold()
    epix.rect(P(-width, 0), P(width, height))
    epix.label(P(posn, y_posn), P(2, 4), r"$y=\sqrt{1-x^2}$", epix.LabelPos.tr)
fig
