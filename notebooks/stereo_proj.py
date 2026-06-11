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
from __future__ import annotations

import math

import epix
from epix import Point

O: epix.Point = Point(x=0, y=0, z=0)


def circ(t: float) -> None:
    if math.isnan(t):  # fallback, doesn't fill properly
        epix.line(tail=Point(x=-2, y=0), head=Point(x=2, y=0))
    else:
        epix.circle(center=Point(x=0, y=t), radius=math.sqrt(1 + t * t))


def f(u: float, v: float) -> epix.Point:
    sph_point: epix.Point = epix.sph(radius=1, theta=u, phi=v)
    return Point(x=sph_point.x3(), y=sph_point.x1(), z=sph_point.x2())


R: epix.Domain = epix.Domain(
    lower_left=Point(x=0, y=-math.pi / 2),
    upper_right=Point(x=4 * math.pi, y=math.pi / 2),
    coarse=epix.Mesh(nx=144, ny=48),
)


# %%
def build() -> None:
    epix.picture(
        lower_left=Point(x=-2, y=-2), upper_right=Point(x=2, y=2), size="6 x 6in"
    )

    epix.begin()

    epix.camera.at(Point(x=4, y=-6, z=3))

    epix.border()

    epix.set_crop()

    # disk
    epix.plain(epix.black())
    epix.fill(epix.black(0.3))

    if 0.25 <= epix.tix() <= 0.75:
        epix.rect(lower_left=Point(x=-100, y=-100), upper_right=Point(x=100, y=100))
        epix.fill(epix.white())

    circ(epix.tan(2 * math.pi * epix.tix()))

    epix.dot(
        Point(x=-1, y=0), offset=Point(x=4, y=-2), text="$-1$", align=epix.LabelPos.br
    )

    epix.plain(epix.rgb(0.9, 0.7, 1))

    # light purple
    epix.fill(epix.rgb(0.9, 0.7, 1))

    epix.clip_face(loc=O, normal=epix.E_3)
    epix.plot(f, R)

    epix.clip_face(
        loc=O,
        normal=Point(
            x=0,
            y=epix.sin(2 * math.pi * epix.tix()),
            z=-epix.cos(2 * math.pi * epix.tix()),
        ),
    )
    epix.surface(f, R)
    epix.clip_restore()
    epix.dot(
        Point(x=1, y=0), offset=Point(x=2, y=-2), text="$1$", align=epix.LabelPos.br
    )

    epix.dot(
        Point(x=0, y=0, z=1),
        offset=Point(x=0, y=8),
        text=r"$\infty$",
        align=epix.LabelPos.t,
    )

    epix.pst_format()


anim: epix.Animation = epix.animate(build, count=9)
anim
