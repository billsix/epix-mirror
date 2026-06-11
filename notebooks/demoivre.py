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
# # demoivre — ported from `samples/demoivre.xp`
# De Moivre's theorem: $(1 + i\theta/n)^n \to e^{i\theta}$, via the `pair` type.

# %%
from __future__ import annotations

from math import pi

import epix
from epix import Point

N = 24
theta = 0.875 * pi
alpha = epix.Pair(1, theta / N)


def pair2P(arg: epix.Pair) -> epix.Point:
    return Point(x=arg.x1(), y=arg.x2())


# %%
with epix.figure(
    lower_left=Point(x=-1.5, y=0), upper_right=Point(x=1, y=1.25), size="200 x 100pt"
) as fig:
    power = epix.Pair(1, 0)
    for _ in range(N):
        epix.line(tail=Point(x=0, y=0), head=pair2P(alpha * power))  # two sides
        epix.line(tail=pair2P(power), head=pair2P(alpha * power))  # of triangle
        power = power * alpha
    epix.label(
        pair2P(alpha),
        offset=Point(x=2, y=0),
        text=r"$\alpha=1+\frac{i\theta}{n}$",
        align=epix.LabelPos.r,
    )
    epix.label_angle(theta - pi)
    epix.ddot(
        pair2P(power),
        offset=Point(x=2, y=4),
        text=r"$\alpha^n\approx e^{i\theta}$",
        align=epix.LabelPos.b,
    )
    epix.plain(epix.black(0.3))
    epix.arc(center=Point(x=0, y=0), radius=1, start=0, finish=theta)
    epix.ddot(
        epix.cis(theta), offset=Point(x=0, y=-4), text="$$", align=epix.LabelPos.b
    )
    epix.bold(epix.red())
    epix.triangle(a=Point(x=0, y=0), b=Point(x=1, y=0), c=pair2P(alpha))
fig
