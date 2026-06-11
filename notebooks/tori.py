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
# # tori — ported from `samples/tori.xp`
# A torus shown three ways (solid, cropped, boxed) across three `screen` panels.

# %%
from __future__ import annotations

import epix
from epix import Point


def F(u: float, v: float) -> epix.Point:
    return epix.polar(radius=2.25 + epix.cos(u), theta=v) + Point(
        x=0, y=0, z=epix.sin(u)
    )


# %%
with epix.figure(
    lower_left=Point(x=0, y=0), upper_right=Point(x=4, y=1), size="6 x 1.5in"
) as fig:
    R: epix.Domain = epix.Domain(
        lower_left=Point(x=0, y=0),
        upper_right=Point(x=1, y=1),
        coarse=epix.Mesh(nx=12, ny=30),
        fine=epix.Mesh(nx=24, ny=60),
    )
    epix.revolutions()
    epix.camera.at(10, 7, 5)

    screen_solid: epix.Screen = epix.Screen(
        lower_left=Point(x=-3, y=-3), upper_right=Point(x=3, y=3)
    )
    with epix.activated(screen_solid):
        epix.fill(epix.white())
        epix.surface(F, R)
        epix.inset(lower_left=Point(x=0, y=0), upper_right=Point(x=1, y=1))

    screen_cropped: epix.Screen = epix.Screen(
        lower_left=Point(x=-3, y=-3), upper_right=Point(x=3, y=3)
    )
    with epix.activated(screen_cropped):
        epix.set_crop()
        epix.border()
        epix.surface(F, R)
        epix.inset(lower_left=Point(x=1.5, y=0), upper_right=Point(x=2.5, y=1))

    screen_boxed: epix.Screen = epix.Screen(
        lower_left=Point(x=-3, y=-3), upper_right=Point(x=3, y=3)
    )
    with epix.activated(screen_boxed):
        epix.clip_box(Point(x=3.5, y=2, z=1.5))
        epix.nofill()
        epix.rect(
            lower_left=Point(x=-3.5, y=-2, z=-1.5),
            upper_right=Point(x=-3.5, y=2, z=1.5),
        )
        epix.rect(
            lower_left=Point(x=-3.5, y=-2, z=-1.5),
            upper_right=Point(x=3.5, y=-2, z=1.5),
        )
        epix.fill()
        epix.surface(F, R)
        epix.nofill()
        epix.rect(
            lower_left=Point(x=3.5, y=-2, z=-1.5), upper_right=Point(x=3.5, y=2, z=1.5)
        )
        epix.rect(
            lower_left=Point(x=-3.5, y=2, z=-1.5), upper_right=Point(x=3.5, y=2, z=1.5)
        )
        epix.inset(lower_left=Point(x=3, y=0), upper_right=Point(x=4, y=1))
fig
