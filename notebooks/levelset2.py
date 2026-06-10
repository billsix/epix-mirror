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
# # levelset2 — ported from `samples/levelset2.xp`
# A monkey-saddle surface drawn as level bands: the surface is re-drawn inside a
# stack of thin horizontal `clip_slice` planes, each shaded by height, over a
# wire-mesh graph and a 3-D coordinate frame. (C++ default — `FLATFILL` undefined
# — uses the position-dependent `color` shading.)

# %%
import math

import epix
from epix import P

MAX = 1

sqrt3 = math.sqrt(3)


# function to be graphed
def f(x, y):
    return P(x, y, 0.75 * y * (y - sqrt3 * x) * (y + sqrt3 * x))


def color(u, v, w):
    return P(-0.5 * w, 0.25 * w, 0.95 * w)


# In the C++ the 3rd ctor arg is the comma-expression `(24, 24)` == 24, which
# converts to mesh(24); reproduced here as epix.mesh(24).
R = epix.domain(P(-MAX, -MAX), P(MAX, MAX), epix.mesh(24), epix.mesh(72, 72))

# %%
with epix.figure(P(-2 * MAX, -2 * MAX), P(2 * MAX, 2 * MAX), "6 x 6in") as fig:
    epix.border(epix.Green(0.6), "1pt")
    epix.backing(epix.Black())

    epix.camera.at(epix.sph(4 * MAX, math.pi / 6, math.pi / 6))

    # positioned for viewpt in first orthant
    epix.yellow()
    epix.grid(P(-MAX, -MAX, -MAX), P(MAX, MAX, -MAX), 4, 4)
    epix.grid(P(-MAX, -MAX, -MAX), P(MAX, -MAX, MAX), 4, 4)
    epix.grid(P(-MAX, -MAX, -MAX), P(-MAX, MAX, MAX), 4, 4)

    Ax = epix.axis(P(-MAX, -MAX, MAX), P(MAX, -MAX, MAX), 4, P(0, 6), epix.LabelPos.t)
    Ay = epix.axis(
        P(MAX, -MAX, -MAX), P(MAX, MAX, -MAX), 4, P(-2, -2), epix.LabelPos.bl
    )
    Az = epix.axis(
        P(MAX, -MAX, -MAX), P(MAX, -MAX, MAX), 4, P(-2, -2), epix.LabelPos.bl
    )

    Ax.frac().draw()
    Ay.frac().draw()
    Az.frac().draw()

    epix.clip_box(P(MAX, MAX, MAX))

    # wire mesh surface
    epix.plain(epix.Green())
    epix.plot(f, R)

    # level bands
    epix.plain(epix.Red())

    for i in range(-5, 6):
        epix.clip_slice(P(0, 0, 0.2 * i), P(0, 0, 1), 0.1)
        epix.surface(f, R, color)  # domain- or position-dependent coloring
        epix.clip_restore()  # remove temporary slicing planes, keep clip box

    epix.pst_format()
fig
