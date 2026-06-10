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
# # levelset — ported from `samples/levelset.xp`
# Level sets of the saddle `z = k(x−y)(x+y)`: the contour plot of the scalar
# field over the base plane (`plot(f, p1, p2, …)`), plus the two families of
# hyperbolic level curves lifted onto the surface, drawn over each slice of a
# domain via `domain.slices2()`.

# %%
import math

import epix
from epix import P

MAX = 3
k = 0.25


def Fp(u, v):
    r = math.sqrt(abs(v))
    if v >= 0:
        return P(r * math.cosh(u), r * math.sinh(u), k * v)
    else:
        return P(-r * math.sinh(u), -r * math.cosh(u), k * v)


def Fm(u, v):
    r = math.sqrt(abs(v))
    if v >= 0:
        return P(-r * math.cosh(u), r * math.sinh(u), k * v)
    else:
        return P(-r * math.sinh(u), r * math.cosh(u), k * v)


def f(x, y):
    return k * (x - y) * (x + y)


# %%
with epix.figure(P(-MAX, -MAX), P(MAX, MAX), "2x2in") as fig:
    epix.camera.at(P(1, -3, 5))
    epix.camera.range(40)

    epix.clip_box(P(MAX, MAX, 2 * MAX))
    epix.clip()

    epix.red()
    epix.plot(f, P(-MAX, -MAX), P(MAX, MAX), epix.mesh(4, 4), epix.mesh(40, 40))

    R = epix.domain(P(-4, -9), P(4, 9), epix.mesh(1, 12), epix.mesh(90, 1))

    epix.bold()
    epix.rgb(0.5, 0.5, 1)
    epix.line(P(MAX, -MAX, 0), P(-MAX, MAX, 0))
    epix.line(P(MAX, MAX, 0), P(-MAX, -MAX, 0))
    epix.plot(Fp, R.slices2())
    epix.plot(Fm, R.slices2())
fig
