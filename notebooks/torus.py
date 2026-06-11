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
# # torus — ported from `samples/torus.xp`
# A torus drawn as a wireframe with cut-away slices (`domain` resize/slice).

# %%
from __future__ import annotations

import epix
from epix import Point

N = 8


def F(t1: float, t2: float) -> epix.Point:
    return epix.polar(radius=2 + epix.sin(t2), theta=t1) + epix.cos(t2) * epix.E_3


R: epix.Domain = epix.Domain(
    lower_left=Point(x=0, y=0),
    upper_right=Point(x=1, y=1),
    coarse=epix.Mesh(nx=4 * N, ny=2 * N),
    fine=epix.Mesh(nx=12 * N, ny=6 * N),
)

# %%
with epix.figure(
    lower_left=Point(x=-3, y=-3), upper_right=Point(x=3, y=3), size="2x2in"
) as fig:
    epix.revolutions()
    epix.camera.at(1.7, -1, 0.75)
    epix.camera.range(8)
    domain_body: epix.Domain = R.resize1(0, 0.875)
    domain_cutaway: epix.Domain = R.resize2(-0.375, 0).resize1(0, 0.125)
    epix.set_rgb(0.2, 0.5, 0.7)
    epix.plot(F, domain_body)
    epix.bold()
    epix.set_rgb(0.5, 0.3, 0.9)
    epix.plot(F, domain_cutaway)
    epix.set_red()
    epix.dashed()
    epix.plot(F, R.slice1(0))
    epix.plot(F, R.slice2(0))
fig
