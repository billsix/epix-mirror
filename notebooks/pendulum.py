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
# # pendulum — ported from `samples/pendulum.flx` (animation)
# A pendulum swinging, with its trajectory in the phase plane (slope field).

# %%
from __future__ import annotations

from math import pi, sqrt

import epix
from epix import Point

theta0 = -5 * pi / 6
EPS = 0.01  # avoid the singularity of f at theta0
posn0: epix.Point = Point(x=theta0, y=0)
pivot: epix.Point = Point(x=0, y=5.5)
ell = 3
K = -epix.cos(theta0)


def F(u: float, v: float) -> epix.Point:
    return Point(x=v, y=-epix.sin(u))


def f(t: float) -> float:
    return 1.0 / sqrt(2 * (K + epix.cos(t)))


def build() -> None:
    t = epix.tix()
    epix.picture(
        lower_left=Point(x=-6.5, y=-2.5),
        upper_right=Point(x=6.5, y=8.5),
        size="6.5 x 5.5in",
    )
    epix.begin()
    period = 4 * (
        sqrt(2 * EPS / (-epix.sin(theta0))) + epix.integral_eval(f, -theta0 - EPS)
    )
    epix.border(epix.black(0.1), width="1pt")
    posn: epix.Point = epix.flow(F, posn0, period * t, int(120 * t))  # phase position
    x_t = -epix.sin(posn.x1())
    y_t = epix.cos(posn.x1())
    epix.slope_field(
        F,
        lower_left=Point(x=-2 * pi, y=-2),
        upper_right=Point(x=2 * pi, y=2),
        nx=48,
        ny=12,
    )
    epix.bold()
    epix.line(tail=pivot, head=pivot - ell * Point(x=x_t, y=y_t))
    epix.set_red()
    epix.ode_plot(F, posn0, period, n=120)
    epix.set_blue()
    epix.box(posn)
    epix.box(pivot - ell * Point(x=x_t, y=y_t))


# %%
anim = epix.animate(build, count=24)
anim
