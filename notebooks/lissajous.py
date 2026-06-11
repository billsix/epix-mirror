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
# # lissajous — ported from `samples/lissajous.xp`
# A Lissajous curve and its tangent field.

# %%
from __future__ import annotations

import epix
from epix import Point


def f(t: float) -> epix.Point:
    return Point(x=epix.sin(3 * t), y=epix.sin(4 * t))


# %%
with epix.figure(
    lower_left=Point(x=-1, y=-1), upper_right=Point(x=1, y=1), size="2 x 2in"
) as fig:
    epix.revolutions()
    epix.bold(epix.red())
    epix.plot(f, 0, 1, n=200)
    epix.arrow_inset(0.2)
    epix.arrow_width(width=1.5)
    epix.plain(epix.blue())
    epix.tan_field(f, 0, 1, n=81)
fig
