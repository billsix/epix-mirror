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
from math import pi, sqrt

import epix
from epix import P

theta0 = -5 * pi / 6
EPS = 0.01  # avoid the singularity of f at theta0
posn0 = P(theta0, 0)
pivot = P(0, 5.5)
ell = 3
K = -epix.Cos(theta0)


def F(u, v):
    return P(v, -epix.Sin(u))


def f(t):
    return 1.0 / sqrt(2 * (K + epix.Cos(t)))


def build():
    t = epix.tix()
    epix.picture(P(-6.5, -2.5), P(6.5, 8.5), "6.5 x 5.5in")
    epix.begin()
    period = 4 * (
        sqrt(2 * EPS / (-epix.Sin(theta0))) + epix.integral_eval(f, -theta0 - EPS)
    )
    epix.border(epix.Black(0.1), "1pt")
    posn = epix.flow(F, posn0, period * t, int(120 * t))  # phase position
    x_t = -epix.Sin(posn.x1())
    y_t = epix.Cos(posn.x1())
    epix.slope_field(F, P(-2 * pi, -2), P(2 * pi, 2), 48, 12)
    epix.bold()
    epix.line(pivot, pivot - ell * P(x_t, y_t))
    epix.red()
    epix.ode_plot(F, posn0, period, 120)
    epix.blue()
    epix.box(posn)
    epix.box(pivot - ell * P(x_t, y_t))


# %%
anim = epix.animate(build, count=24)
anim
