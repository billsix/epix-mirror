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
# # steiner2 — ported from `samples/steiner2.flx` (animation)
# The Steiner surface sliced into rotating layers (depth-sorted by the camera).

# %%
import epix
from epix import P

MAX = 2.5
rad = 0.8 * MAX
N = 6


def F(u, v):
    x = rad * epix.Cos(u) * epix.Cos(v)
    y = rad * epix.Cos(u) * epix.Sin(v)
    z = rad * epix.Sin(u)
    return P(y * z, x * z, x * y)


def build():
    t = epix.tix()
    epix.picture(P(-MAX, -MAX), P(MAX, MAX), "4x4in")
    epix.begin()
    epix.backing(epix.RGB(0.7, 0.9, 1))
    epix.fill()
    epix.revolutions()
    epix.camera.at(epix.cyl(10, 0.25, 4))
    perp = epix.sph(1, 0.125, t)
    R = epix.domain(P(-0.25, 0), P(0.25, 0.5), epix.mesh(32, 32), epix.mesh(64, 64))
    mult = 1
    if perp.dot(epix.camera.eye()) < 0:
        mult = -1
    for i in range(-mult * N, mult * N, mult):
        epix.clip_slice((MAX * i * 1.0 / N) * perp, perp, 0.1)
        epix.rgb(0.25 + 0.1 * i, 0, -0.1 * i)
        epix.surface(F, R)
        epix.clip_restore()


# %%
anim = epix.animate(build, count=24)
anim
