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
# # mirrorball — ported from `samples/mirrorball.flx`
# Twelve unit spheres centered at the icosahedron's vertices, intersected with a
# bounding sphere of radius √3 (`S0 * Si` → a `Circle`); the resulting circles
# tile the big sphere like a mirror ball. Animated as a slowly-rotating
# red/cyan stereo pair (`tix()` drives the camera angle).

# %%
import math

import epix
from epix import P

r_0 = 1  # 0.75
golden = 0.5 * (1 + math.sqrt(5))

S0 = epix.Sphere(P(0, 0, 0), math.sqrt(3))

S1 = epix.Sphere(P(1, golden, 0), r_0)
S2 = epix.Sphere(P(-1, golden, 0), r_0)
S3 = epix.Sphere(P(1, -golden, 0), r_0)
S4 = epix.Sphere(P(-1, -golden, 0), r_0)

S5 = epix.Sphere(P(0, 1, golden), r_0)
S6 = epix.Sphere(P(0, -1, golden), r_0)
S7 = epix.Sphere(P(0, 1, -golden), r_0)
S8 = epix.Sphere(P(0, -1, -golden), r_0)

S9 = epix.Sphere(P(golden, 0, 1), r_0)
Sa = epix.Sphere(P(golden, 0, -1), r_0)
Sb = epix.Sphere(P(-golden, 0, 1), r_0)
Sc = epix.Sphere(P(-golden, 0, -1), r_0)

# each circle is the intersection of the big sphere with a vertex sphere
circles = [S0 * S for S in (S1, S2, S3, S4, S5, S6, S7, S8, S9, Sa, Sb, Sc)]


# bold circle if facing camera, otherwise plain
def sphere_draw(C):
    if (C.center() | epix.camera.viewpt()) >= 0:
        epix.bold()
    else:
        epix.plain()
    C.draw()


def icosa():
    for C in circles:
        sphere_draw(C)


def build():
    epix.picture(P(-2, -2), P(2, 2), "6x6in")
    epix.begin()
    epix.set_crop()
    epix.revolutions()

    # draw frames
    epix.camera.at(epix.sph(12, 0.5 * epix.tix(), 0.05))
    epix.red(-1.4)  # nearly cyan
    icosa()

    epix.camera.at(epix.sph(12, 0.01 + 0.5 * epix.tix(), 0.05))
    epix.red(1.4)
    icosa()


# %%
anim = epix.animate(build, count=24)
anim
