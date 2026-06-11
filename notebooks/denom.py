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
# # denom — ported from `samples/denom.xp`
# Thomae's function: a dot at each rational p/q of height 1/q.

# %%
from __future__ import annotations

from math import gcd

import epix
from epix import Point

N: int = 30  # maximum denominator plotted

# %%
with epix.figure(
    lower_left=Point(x=-2, y=0), upper_right=Point(x=2, y=1), size="4x1in"
) as fig:
    Ax: epix.Axis = epix.Axis(
        Point(x=-2, y=0),
        Point(x=2, y=0),
        n=8,
        offset=Point(x=0, y=-6),
        align=epix.LabelPos.b,
    )
    Ax.subdivide(n=6).frac().draw()
    epix.v_axis(n=2)
    epix.dot_size(diameter=2)
    for i in range(1, N):
        for j in range(int(i * epix.xmin()), int(i * epix.xmax()) + 1):
            if gcd(i, j) == 1:
                epix.ddot(Point(x=j * 1.0 / i, y=1.0 / i))
    epix.font_size("scriptsize")
    buf: str = (
        "$f(x)=\\begin{cases}%\n"
        "  \\frac{1}{q} & \\text{ $x=\\frac{p}{q}$ in lowest terms} \\\\\n"
        "  0 & \\text{ $x$ irrational}%\n"
        "\\end{cases}$"
    )
    epix.label(
        Point(x=0, y=0.75), offset=Point(x=2, y=0), text=buf, align=epix.LabelPos.r
    )
fig
