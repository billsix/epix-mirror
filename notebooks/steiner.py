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
# # steiner — ported from `samples/steiner.flx` (animation)
# The Steiner Roman surface, rotating (a `.flx` animation; 2-var color function).

# %%
import epix
from epix import P

MAX = 2.5
rad = 0.8 * MAX


def F(u, v):
    x = rad * epix.Cos(u) * epix.Cos(v)
    y = rad * epix.Cos(u) * epix.Sin(v)
    z = rad * epix.Sin(u)
    return P(y * z, x * z, x * y)


def color(x, y):
    return P(1, 1, 1)  # white (COLORSHADE off)


def build():
    t = epix.tix()
    epix.picture(P(-MAX, -MAX), P(MAX, MAX), "4x4in")
    epix.begin()
    epix.backing(epix.RGB(0.7, 0.9, 1))
    epix.fill()
    epix.revolutions()
    epix.camera.at(epix.cyl(10, 0.5 * t, 2))  # exploit 2-fold symmetry
    epix.surface(
        F,
        epix.domain(P(-0.25, 0), P(0.25, 0.5), epix.mesh(32, 32), epix.mesh(64, 64)),
        color,
    )


# %%
anim = epix.animate(build, count=24)
anim
