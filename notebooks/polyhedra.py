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
import epix
from epix import P

MAX = 1.25

# %%
with epix.figure(P(-MAX, -MAX), P(MAX, MAX), "6 x 6in") as fig:
    epix.grid()
    epix.camera.at(P(8, 3, 2))
    epix.camera.range(20)

    S = epix.Sphere()
    f = epix.frame()
    f2 = epix.frame(epix.E_2, epix.E_1, epix.E_3)

    # hidden (back) edges
    epix.rgb(0.7, 1, 1)
    epix.back_dodeca(S, f)

    epix.rgb(1, 1, 0.7)
    epix.back_icosa(S, f)

    epix.bold()

    # visible (front) edges
    epix.rgb(0, 0.7, 0.7)
    epix.front_dodeca(S, f)

    epix.rgb(1, 0.9, 0)
    epix.front_icosa(S, f)

    epix.green(0.8)
    S.draw()
fig
