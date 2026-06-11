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
# # geomsum — ported from `samples/geomsum.xp`
# The geometric series 1/2 + 1/4 + 1/8 + … filling the unit square.

# %%
from __future__ import annotations

import epix
from epix import Point

N: int = 8

# %%
with epix.figure(
    lower_left=Point(x=0, y=0), upper_right=Point(x=1, y=1), size="2.5x2.5in"
) as fig:
    epix.pen(epix.white(), width="0.8pt")
    epix.fill(epix.white(0.9))
    epix.rect(lower_left=Point(x=0, y=0), upper_right=Point(x=1, y=1))
    epix.set_black()
    epix.label(Point(x=1.0 / 4, y=1.0 / 2), r"$\frac{1}{2}$")
    epix.label(Point(x=5.0 / 8, y=3.0 / 4), r"$\frac{1}{8}$")
    epix.label(Point(x=13.0 / 16, y=7.0 / 8), r"$\frac{1}{32}$")
    epix.set_white()
    epix.fill(epix.blue())
    t: float = 0.5
    for _ in range(N):
        epix.rect(
            lower_left=Point(x=1 - t, y=1 - 2 * t), upper_right=Point(x=1, y=1 - t)
        )
        epix.line(tail=Point(x=1 - t, y=1 - 2 * t), head=Point(x=1 - t, y=1))
        t *= 0.5
    epix.label(Point(x=3.0 / 4, y=1.0 / 4), r"$\mathbf{\frac{1}{4}}$")
    epix.label(Point(x=7.0 / 8, y=5.0 / 8), r"$\mathbf{\frac{1}{16}}$")
    epix.label(Point(x=15.0 / 16, y=13.0 / 16), r"$\mathbf{\frac{1}{64}}$")
fig
