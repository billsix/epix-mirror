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
from __future__ import annotations

import epix
from epix import Point

MAX: float = 2.5
rad: float = 0.8 * MAX


def F(u: float, v: float) -> epix.Point:
    x: float = rad * epix.cos(u) * epix.cos(v)
    y: float = rad * epix.cos(u) * epix.sin(v)
    z: float = rad * epix.sin(u)
    return Point(x=y * z, y=x * z, z=x * y)


def color(x: float, y: float) -> epix.Point:
    return Point(x=1, y=1, z=1)  # white (COLORSHADE off)


def build() -> None:
    t = epix.tix()
    epix.picture(
        lower_left=Point(x=-MAX, y=-MAX), upper_right=Point(x=MAX, y=MAX), size="4x4in"
    )
    epix.begin()
    epix.backing(epix.rgb(0.7, 0.9, 1))
    epix.fill()
    epix.revolutions()
    epix.camera.at(epix.cyl(radius=10, theta=0.5 * t, z=2))  # exploit 2-fold symmetry
    epix.surface(
        F,
        epix.Domain(
            lower_left=Point(x=-0.25, y=0),
            upper_right=Point(x=0.25, y=0.5),
            coarse=epix.Mesh(nx=32, ny=32),
            fine=epix.Mesh(nx=64, ny=64),
        ),
        color,
    )


# %%
anim: epix.Animation = epix.animate(build, count=24)
anim
