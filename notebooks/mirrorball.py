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
from __future__ import annotations

import math

import epix
from epix import Point

r_0 = 1  # 0.75
golden = 0.5 * (1 + math.sqrt(5))

S0: epix.Sphere = epix.Sphere(center=Point(x=0, y=0, z=0), radius=math.sqrt(3))

S1: epix.Sphere = epix.Sphere(center=Point(x=1, y=golden, z=0), radius=r_0)
S2: epix.Sphere = epix.Sphere(center=Point(x=-1, y=golden, z=0), radius=r_0)
S3: epix.Sphere = epix.Sphere(center=Point(x=1, y=-golden, z=0), radius=r_0)
S4: epix.Sphere = epix.Sphere(center=Point(x=-1, y=-golden, z=0), radius=r_0)

S5: epix.Sphere = epix.Sphere(center=Point(x=0, y=1, z=golden), radius=r_0)
S6: epix.Sphere = epix.Sphere(center=Point(x=0, y=-1, z=golden), radius=r_0)
S7: epix.Sphere = epix.Sphere(center=Point(x=0, y=1, z=-golden), radius=r_0)
S8: epix.Sphere = epix.Sphere(center=Point(x=0, y=-1, z=-golden), radius=r_0)

S9: epix.Sphere = epix.Sphere(center=Point(x=golden, y=0, z=1), radius=r_0)
Sa: epix.Sphere = epix.Sphere(center=Point(x=golden, y=0, z=-1), radius=r_0)
Sb: epix.Sphere = epix.Sphere(center=Point(x=-golden, y=0, z=1), radius=r_0)
Sc: epix.Sphere = epix.Sphere(center=Point(x=-golden, y=0, z=-1), radius=r_0)

# each circle is the intersection of the big sphere with a vertex sphere
circles: list[epix.Circle] = [
    S0 * S for S in (S1, S2, S3, S4, S5, S6, S7, S8, S9, Sa, Sb, Sc)
]


# bold circle if facing camera, otherwise plain
def sphere_draw(C: epix.Circle) -> None:
    if (C.center() | epix.camera.viewpt()) >= 0:
        epix.bold()
    else:
        epix.plain()
    C.draw()


def icosa() -> None:
    for C in circles:
        sphere_draw(C)


def build() -> None:
    epix.picture(
        lower_left=Point(x=-2, y=-2), upper_right=Point(x=2, y=2), size="6x6in"
    )
    epix.begin()
    epix.set_crop()
    epix.revolutions()

    # draw frames
    epix.camera.at(epix.sph(radius=12, theta=0.5 * epix.tix(), phi=0.05))
    epix.set_red(-1.4)  # nearly cyan
    icosa()

    epix.camera.at(epix.sph(radius=12, theta=0.01 + 0.5 * epix.tix(), phi=0.05))
    epix.set_red(1.4)
    icosa()


# %%
anim = epix.animate(build, count=24)
anim
