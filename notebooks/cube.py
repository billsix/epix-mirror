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
# # cube — ported from `samples/cube.flx` (animation)
#
# A rotating wireframe cube with a "clock" showing the rotation angle. This is an
# **animation** (`.flx`): each frame reads the `tix()` counter (a parameter in
# `[0, 1)`). `epix.animate(frame, count=N)` renders `N` frames and assembles a
# looping gif. Each frame is built in a forked child so ePiX's per-render global
# state starts fresh — mirroring how `flix` runs a new process per frame — so the
# frames are **byte-identical** to the C++ original.

# %%
from __future__ import annotations

import epix
from epix import Point


def frame() -> None:
    t = epix.tix()
    epix.picture(
        lower_left=Point(x=-4, y=-4), upper_right=Point(x=4, y=4), size="5 x 5in"
    )
    epix.begin()
    epix.revolutions()
    epix.grid(nx=1, ny=1)

    ctr: epix.Point = Point(x=3.25, y=-3.25)  # "clock" showing the angle
    epix.bold()
    epix.ellipse(center=ctr, radius=Point(x=0.5, y=0.5))
    epix.line(tail=ctr, head=ctr + epix.polar(radius=0.4, theta=t))
    epix.label_angle(t - 0.25)
    epix.label(ctr + epix.polar(radius=0.6, theta=t), r"$\theta$")

    R: epix.Domain = epix.Domain(
        lower_left=Point(x=-2, y=-2, z=-2),
        upper_right=Point(x=2, y=2, z=2),
        coarse=epix.Mesh(nx=1, ny=1, nz=1),
    )
    epix.camera.at(epix.sph(radius=10, theta=t, phi=-0.05))
    epix.plain(epix.cyan())
    epix.plot(lambda x, y, z: epix.Point(x=x, y=y, z=z), R)  # the cube
    epix.camera.at(epix.sph(radius=10, theta=0.01 + t, phi=-0.05))
    epix.set_red(1.4)
    epix.plot(
        lambda x, y, z: epix.Point(x=x, y=y, z=z), R
    )  # second, slightly-rotated copy


# %%
anim = epix.animate(frame, count=24)
anim
