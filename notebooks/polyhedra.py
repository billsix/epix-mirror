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
# # polyhedra — ported from `samples/polyhedra.xp`
# Platonic solids inscribed in a sphere, drawn with hidden (`back_*`) and visible
# (`front_*`) edges. The C++ default build (`#define A5`, no `-DALL`) draws the
# two icosahedral-symmetry solids: the dodecahedron and the icosahedron.

# %%
from __future__ import annotations

import epix
from epix import Point

MAX: float = 1.25

# %%
with epix.figure(
    lower_left=Point(x=-MAX, y=-MAX), upper_right=Point(x=MAX, y=MAX), size="6 x 6in"
) as fig:
    epix.grid()
    epix.camera.at(Point(x=8, y=3, z=2))
    epix.camera.range(20)

    S: epix.Sphere = epix.Sphere()
    frame1: epix.Frame = epix.Frame()
    frame2: epix.Frame = epix.Frame(epix.E_2, epix.E_1, epix.E_3)

    # hidden (back) edges
    epix.set_rgb(0.7, 1, 1)
    epix.back_dodeca(S, frame1)

    epix.set_rgb(1, 1, 0.7)
    epix.back_icosa(S, frame1)

    epix.bold()

    # visible (front) edges
    epix.set_rgb(0, 0.7, 0.7)
    epix.front_dodeca(S, frame1)

    epix.set_rgb(1, 0.9, 0)
    epix.front_icosa(S, frame1)

    epix.set_green(0.8)
    S.draw()
fig
