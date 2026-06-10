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
from math import gcd

import epix
from epix import P

N = 30  # maximum denominator plotted

# %%
with epix.figure(P(-2, 0), P(2, 1), "4x1in") as fig:
    Ax = epix.axis(P(-2, 0), P(2, 0), 8, P(0, -6), epix.LabelPos.b)
    Ax.subdivide(6).frac().draw()
    epix.v_axis(2)
    epix.dot_size(2)
    for i in range(1, N):
        for j in range(int(i * epix.xmin()), int(i * epix.xmax()) + 1):
            if gcd(i, j) == 1:
                epix.ddot(P(j * 1.0 / i, 1.0 / i))
    epix.font_size("scriptsize")
    buf = (
        "$f(x)=\\begin{cases}%\n"
        "  \\frac{1}{q} & \\text{ $x=\\frac{p}{q}$ in lowest terms} \\\\\n"
        "  0 & \\text{ $x$ irrational}%\n"
        "\\end{cases}$"
    )
    epix.label(P(0, 0.75), P(2, 0), buf, epix.LabelPos.r)
fig
