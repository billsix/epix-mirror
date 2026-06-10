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
import epix
from epix import P

N = 8


def F(t1, t2):
    return epix.polar(2 + epix.Sin(t2), t1) + epix.Cos(t2) * epix.E_3


R = epix.domain(P(0, 0), P(1, 1), epix.mesh(4 * N, 2 * N), epix.mesh(12 * N, 6 * N))

# %%
with epix.figure(P(-3, -3), P(3, 3), "2x2in") as fig:
    epix.revolutions()
    epix.camera.at(1.7, -1, 0.75)
    epix.camera.range(8)
    epix.rgb(0.2, 0.5, 0.7)
    epix.plot(F, R.resize1(0, 0.875))
    epix.bold()
    epix.rgb(0.5, 0.3, 0.9)
    epix.plot(F, R.resize2(-0.375, 0).resize1(0, 0.125))
    epix.red()
    epix.dashed()
    epix.plot(F, R.slice1(0))
    epix.plot(F, R.slice2(0))
fig
