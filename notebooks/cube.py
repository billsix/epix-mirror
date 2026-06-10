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
import epix
from epix import P


def frame():
    t = epix.tix()
    epix.picture(P(-4, -4), P(4, 4), "5 x 5in")
    epix.begin()
    epix.revolutions()
    epix.grid(1, 1)

    ctr = P(3.25, -3.25)  # "clock" showing the angle
    epix.bold()
    epix.ellipse(ctr, P(0.5, 0.5))
    epix.line(ctr, ctr + epix.polar(0.4, t))
    epix.label_angle(t - 0.25)
    epix.label(ctr + epix.polar(0.6, t), r"$\theta$")

    R = epix.domain(P(-2, -2, -2), P(2, 2, 2), epix.mesh(1, 1, 1))
    epix.camera.at(epix.sph(10, t, -0.05))
    epix.plain(epix.Cyan())
    epix.plot(lambda x, y, z: epix.P(x, y, z), R)  # the cube
    epix.camera.at(epix.sph(10, 0.01 + t, -0.05))
    epix.red(1.4)
    epix.plot(lambda x, y, z: epix.P(x, y, z), R)  # second, slightly-rotated copy


# %%
anim = epix.animate(frame, count=24)
anim
