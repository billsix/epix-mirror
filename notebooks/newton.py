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
# # newton — ported from `samples/newton.xp`
# Newton's method: tangent-line iterates converging to a root.

# %%
from __future__ import annotations

import epix
from epix import Point


def f(t: float) -> float:
    return 1 + t * (-3 + t * t)


def df(t: float) -> float:
    return epix.deriv(f, t).x2()


# %%
with epix.figure(
    lower_left=Point(x=1.5, y=0), upper_right=Point(x=2, y=3), size="2.5 x 2.5in"
) as fig:
    epix.h_axis(2)
    x0: int = 2
    x1: float = x0 - f(x0) / df(x0)
    epix.label(Point(x=x0, y=0), Point(x=0, y=-4), "$x_i$", epix.LabelPos.b)
    for _ in range(3):
        epix.dashed()
        epix.line(tail=Point(x=x0, y=0), head=Point(x=x0, y=f(x0)))
        epix.solid()
        epix.line(tail=Point(x=x0, y=f(x0)), head=Point(x=x0 - f(x0) / df(x0), y=0))
        x0 -= f(x0) / df(x0)
    epix.bold()
    epix.plot(f, epix.xmin(), epix.xmax() + 0.05, n=60)
    epix.label(Point(x=1.75, y=f(1.75)), Point(x=-2, y=2), "$y=f(x)$", epix.LabelPos.tl)
    epix.label(Point(x=x1, y=0), Point(x=0, y=-4), "$x_{i+1}$", epix.LabelPos.b)
    epix.label(
        Point(x=1.75, y=df(2) * (1.75 - x1)),
        Point(x=0, y=-4),
        "Slope $= f'(x_i)$",
        epix.LabelPos.br,
    )
fig
