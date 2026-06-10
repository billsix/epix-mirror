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
# # stereo_proj — ported from `samples/stereo_proj.flx`
# The stereographic "shadow" in ℂ of a hemisphere rotating about the real axis:
# the moving disk/half-plane image and the hemisphere itself (clipped to the
# rotating cut), animated over a turn. An **odd** frame count avoids the frame
# where the boundary passes through the point at infinity.

# %%
import math

import epix
from epix import P

O = P(0, 0, 0)


def circ(t):
    if math.isnan(t):  # fallback, doesn't fill properly
        epix.line(P(-2, 0), P(2, 0))
    else:
        epix.circle(P(0, t), math.sqrt(1 + t * t))


def f(u, v):
    tmp = epix.sph(1, u, v)
    return P(tmp.x3(), tmp.x1(), tmp.x2())


R = epix.domain(P(0, -math.pi / 2), P(4 * math.pi, math.pi / 2), epix.mesh(144, 48))


# %%
def build():
    epix.picture(P(-2, -2), P(2, 2), "6 x 6in")

    epix.begin()

    epix.camera.at(P(4, -6, 3))

    epix.border()

    epix.set_crop()

    # disk
    epix.plain(epix.Black())
    epix.fill(epix.Black(0.3))

    if 0.25 <= epix.tix() <= 0.75:
        epix.rect(P(-100, -100), P(100, 100))
        epix.fill(epix.White())

    circ(epix.Tan(2 * math.pi * epix.tix()))

    epix.dot(P(-1, 0), P(4, -2), "$-1$", epix.LabelPos.br)

    epix.plain(epix.RGB(0.9, 0.7, 1))

    # light purple
    epix.fill(epix.RGB(0.9, 0.7, 1))

    epix.clip_face(O, epix.E_3)
    epix.plot(f, R)

    epix.clip_face(
        O, P(0, epix.Sin(2 * math.pi * epix.tix()), -epix.Cos(2 * math.pi * epix.tix()))
    )
    epix.surface(f, R)
    epix.clip_restore()
    epix.dot(P(1, 0), P(2, -2), "$1$", epix.LabelPos.br)

    epix.dot(P(0, 0, 1), P(0, 8), r"$\infty$", epix.LabelPos.t)

    epix.pst_format()


anim = epix.animate(build, count=9)
anim
