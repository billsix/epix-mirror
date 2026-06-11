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
from __future__ import annotations

import epix
from epix import Point

MAX = 2.5
rad = 0.8 * MAX
N = 6


def F(u: float, v: float) -> epix.Point:
    x = rad * epix.cos(u) * epix.cos(v)
    y = rad * epix.cos(u) * epix.sin(v)
    z = rad * epix.sin(u)
    return Point(x=y * z, y=x * z, z=x * y)


def build() -> None:
    t = epix.tix()
    epix.picture(
        lower_left=Point(x=-MAX, y=-MAX), upper_right=Point(x=MAX, y=MAX), size="4x4in"
    )
    epix.begin()
    epix.backing(epix.rgb(0.7, 0.9, 1))
    epix.fill()
    epix.revolutions()
    epix.camera.at(epix.cyl(radius=10, theta=0.25, z=4))
    cut_plane_normal = epix.sph(radius=1, theta=0.125, phi=t)
    R: epix.Domain = epix.Domain(
        lower_left=Point(x=-0.25, y=0),
        upper_right=Point(x=0.25, y=0.5),
        coarse=epix.Mesh(nx=32, ny=32),
        fine=epix.Mesh(nx=64, ny=64),
    )
    direction = 1
    if cut_plane_normal.dot(epix.camera.eye()) < 0:
        direction = -1
    for i in range(-direction * N, direction * N, direction):
        epix.clip_slice(
            loc=(MAX * i * 1.0 / N) * cut_plane_normal,
            normal=cut_plane_normal,
            thickness=0.1,
        )
        epix.set_rgb(0.25 + 0.1 * i, 0, -0.1 * i)
        epix.surface(F, R)
        epix.clip_restore()


# %%
anim = epix.animate(build, count=24)
anim
