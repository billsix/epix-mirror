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
# # hyperboloid — ported from `samples/hyperboloid.xp`
# A circular hyperboloid of one sheet (a `surface_rev` of the profile
# `(sinh, cosh)` about a tilted `frame` axis) and its two families of straight-
# line rulings, clipped to the front half.

# %%
from __future__ import annotations

import math

import epix
from epix import Point

NUM_LINES: int = 64
dt: float = 1.0 / NUM_LINES

# %%
with epix.figure(
    lower_left=Point(x=-3, y=-3), upper_right=Point(x=3, y=3), size="6x6in"
) as fig:
    epix.revolutions()

    epix.cam().at(20, 0, 5)

    epix.clip_box(Point(x=4, y=4, z=2))

    epix.fill(epix.black(0.8))

    # hyperboloid
    epix.surface_rev(
        math.sinh,
        math.cosh,
        domain=epix.Domain(
            lower_left=Point(x=-2, y=0),
            upper_right=Point(x=2, y=1),
            coarse=epix.Mesh(nx=24, ny=36),
        ),
        coords=epix.Frame(epix.E_3, epix.E_2, epix.E_1),
    )

    # clip to {x>0}
    epix.clip_box(lower_left=Point(x=0, y=-4, z=-2), upper_right=Point(x=4, y=4, z=2))

    # rulings
    for i in range(-NUM_LINES // 2, NUM_LINES // 2 + 1):
        t: float = i * dt
        epix.set_rgb(0, 0.5, 1)
        epix.infinite_line(
            tail=Point(x=epix.cos(t), y=epix.sin(t), z=0),
            head=Point(x=epix.cos(t) + epix.sin(t), y=epix.sin(t) - epix.cos(t), z=1),
        )

        epix.set_red()
        epix.infinite_line(
            tail=Point(x=epix.cos(t), y=epix.sin(t), z=0),
            head=Point(x=epix.cos(t) - epix.sin(t), y=epix.sin(t) + epix.cos(t), z=1),
        )

    # emphatic rulings
    epix.bbold(epix.rgb(0.1, 0.6, 0.9))
    epix.infinite_line(tail=Point(x=1, y=0, z=0), head=Point(x=1, y=-1, z=1))

    epix.pen(epix.red(1.2))
    epix.infinite_line(tail=Point(x=1, y=0, z=0), head=Point(x=1, y=1, z=1))
    epix.infinite_line(tail=Point(x=0, y=1, z=0), head=Point(x=-1, y=1, z=1))

    epix.clip_box()
    epix.set_black()
    epix.label(
        Point(x=1, y=0, z=0) - Point(x=0, y=-2, z=2),
        offset=Point(x=2, y=-2),
        text=r"$\ell_0^-$",
        align=epix.LabelPos.br,
    )
    epix.label(
        Point(x=1, y=0, z=0) + Point(x=0, y=2, z=2),
        offset=Point(x=18, y=2),
        text=r"$\ell_0^+$",
        align=epix.LabelPos.t,
    )
    epix.label(
        Point(x=0, y=1, z=0) - Point(x=-2, y=0, z=2),
        offset=Point(x=0, y=-4),
        text=r"$\ell_{\pi/2}^+$",
        align=epix.LabelPos.b,
    )

    epix.label(
        Point(x=0, y=0, z=-3),
        offset=Point(x=0, y=-24),
        text=r"$\ell_\theta^-(t)=(\cos\theta, \sin\theta, 0) + t(\sin\theta, -\cos\theta, 1)$",
        align=epix.LabelPos.b,
    )

    epix.label(
        Point(x=0, y=0, z=-3),
        offset=Point(x=0, y=-12),
        text=r"$\ell_\theta^+(t)=(\cos\theta, \sin\theta, 0) + t(-\sin\theta, \cos\theta, 1)$",
        align=epix.LabelPos.b,
    )

    epix.pst_format()
fig
