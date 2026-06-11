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
# # artifacts — ported from `samples/artifacts.xp`
# A sphere drawn at coarse/fine mesh, flat/smooth shading — four panels.

# %%
from __future__ import annotations

import epix
from epix import Point


def F(u: float, v: float) -> epix.Point:
    return epix.sph(radius=1, theta=u, phi=v)


# %%
with epix.figure(
    lower_left=Point(x=0, y=0), upper_right=Point(x=5, y=1), size="5 x 1in"
) as fig:
    coarse: epix.Domain = epix.Domain(
        lower_left=Point(x=0, y=0),
        upper_right=Point(x=0.5, y=1),
        coarse=epix.Mesh(nx=6, ny=12),
        fine=epix.Mesh(nx=60, ny=60),
    )
    fine: epix.Domain = epix.Domain(
        lower_left=Point(x=0, y=0),
        upper_right=Point(x=0.5, y=1),
        coarse=epix.Mesh(nx=15, ny=30),
        fine=epix.Mesh(nx=60, ny=60),
    )
    epix.revolutions()
    epix.camera.at(Point(x=5, y=4, z=3))
    epix.fill(epix.white())
    epix.nofill()  # fill color set -> flat shading
    screen_coarse_flat: epix.Screen = epix.Screen(
        lower_left=Point(x=-1, y=-1), upper_right=Point(x=1, y=1)
    )
    epix.activate(screen_coarse_flat)
    epix.surface(F, coarse)
    screen_fine_flat: epix.Screen = epix.Screen(
        lower_left=Point(x=-1, y=-1), upper_right=Point(x=1, y=1)
    )
    epix.activate(screen_fine_flat)
    epix.surface(F, fine)
    epix.fill()
    screen_coarse_smooth: epix.Screen = epix.Screen(
        lower_left=Point(x=-1, y=-1), upper_right=Point(x=1, y=1)
    )
    epix.activate(screen_coarse_smooth)
    epix.surface(F, coarse)
    screen_fine_smooth: epix.Screen = epix.Screen(
        lower_left=Point(x=-1, y=-1), upper_right=Point(x=1, y=1)
    )
    epix.activate(screen_fine_smooth)
    epix.surface(F, fine)
    epix.inset(
        child=screen_coarse_flat,
        lower_left=Point(x=0, y=0),
        upper_right=Point(x=1, y=1),
    )
    epix.inset(
        child=screen_fine_flat,
        lower_left=Point(x=1.25, y=0),
        upper_right=Point(x=2.25, y=1),
    )
    epix.inset(
        child=screen_coarse_smooth,
        lower_left=Point(x=2.5, y=0),
        upper_right=Point(x=3.5, y=1),
    )
    epix.inset(
        child=screen_fine_smooth,
        lower_left=Point(x=3.75, y=0),
        upper_right=Point(x=4.75, y=1),
    )
fig
