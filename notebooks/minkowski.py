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
# # minkowski — ported from `samples/minkowski.xp`
# The Minkowski "unit sphere" of 2+1 special relativity: the spacelike
# hyperboloid of one sheet, the timelike hyperboloid of two sheets, and the
# light cone between them, assembled as one `scenery` object, with hyperbolic
# lines and labelled unit vectors.

# %%
from __future__ import annotations

import math

import epix
from epix import Point

MAX = 3


# hyperboloid of one sheet / spacelike cone
def Fp(x: float, y: float, z: float) -> epix.Point:
    return Point(
        x=z * math.cosh(x) * epix.cos(y),
        y=z * math.cosh(x) * epix.sin(y),
        z=z * math.sinh(x),
    )


# hyperboloid of two sheets / timelike cone
def Fm(x: float, y: float, z: float) -> epix.Point:
    return Point(
        x=z * math.sinh(x) * epix.cos(y),
        y=z * math.sinh(x) * epix.sin(y),
        z=z * math.cosh(x),
    )


# light cone
def Fz(x: float, y: float) -> epix.Point:
    return abs(math.sinh(x)) * Point(x=epix.cos(y), y=epix.sin(y), z=epix.sgn(x))


# hyperbolic lines
def hyperbolic_line_1(t: float) -> epix.Point:
    return (
        math.cosh(t) * Point(x=-math.sqrt(1.25), y=0, z=1.5) + math.sinh(t) * epix.E_2
    )


def hyperbolic_line_2(t: float) -> epix.Point:
    return math.cosh(t) * Point(x=-1, y=0.5, z=1.5) + (
        math.sqrt(0.4) * math.sinh(t)
    ) * Point(x=0.5, y=1, z=0)


# %%
epix.bounding_box(Point(x=-MAX, y=-MAX), Point(x=MAX, y=MAX))
epix.unitlength("1in")
epix.picture(6, 6)

epix.begin()
epix.camera.at(10, -3, 4)

epix.revolutions()

R: epix.Domain = epix.Domain(
    lower_left=Point(x=-1.5, y=0.25, z=-1),
    upper_right=Point(x=1.5, y=0.75, z=1),
    coarse=epix.Mesh(18, 12, 4),
    fine=epix.Mesh(36, 48, 12),
)

epix.fill(epix.white())

# assemble surfaces as a single object
hyp: epix.Scenery = epix.Scenery(Fp, R.slice3(1))
hyp.add(Fm, R.resize1(0, 1.5).slice3(1))
hyp.add(Fm, R.resize1(-1.5, 0).slice3(-1))
hyp.add(Fz, R.slice3(1))

hyp.draw()
epix.nofill()

# Draw cuts of intersection with (y,z)-plane.
# Simulated 3-D/tube pen: light color drawn over darker shade
epix.base(epix.red(0.6), "3pt")
epix.bold(epix.red())

epix.plot(Fp, R.slice2(0.25).slice3(1))
epix.plot(Fp, R.slice2(0.25).slice3(-1))

epix.plot(Fm, R.slice2(0.25).slice3(1))
epix.plot(Fm, R.slice2(0.25).slice3(-1))

epix.plot(Fz, R.slice2(0.25).slice3(1))
epix.plot(Fz, R.slice2(0.75).slice3(-1))

# to clip lines
epix.clip_box(
    lower_left=Point(x=-10, y=-10, z=-math.cosh(1.5)),
    upper_right=Point(x=10, y=10, z=math.cosh(1.5)),
)

epix.base(epix.green(0.6), "3pt")
epix.bold(epix.green())

epix.plot(hyperbolic_line_1, -1.5, 1.5, n=120)
epix.plot(hyperbolic_line_2, -1.5, 1.5, n=120)

# Labels with "overlay" arrows
epix.font_size("scriptsize")

epix.set_black()
epix.camera.at(Point(x=0, y=0, z=10000))
epix.label(
    Point(x=1.5, y=0), Point(x=4, y=16), "Future timelike unit vectors", epix.LabelPos.r
)
epix.label(
    Point(x=1.5, y=0), Point(x=4, y=0), "Unit spacelike vectors", epix.LabelPos.r
)
epix.label(Point(x=1.5, y=0), Point(x=4, y=-16), "Lightlike vectors", epix.LabelPos.r)
epix.label(
    Point(x=1.5, y=0), Point(x=4, y=-32), "Past timelike unit vectors", epix.LabelPos.r
)

epix.base(epix.white(), "2pt")
epix.bold(epix.black())
epix.arrow(tail=Point(x=1.5, y=0), head=Point(x=1, y=0), scale=0.5)
epix.arrow(tail=Point(x=1.5, y=0.25), head=Point(x=1, y=1.35), scale=0.5)
epix.arrow(tail=Point(x=1.5, y=-0.25), head=Point(x=1, y=-0.75), scale=0.5)
epix.arrow(tail=Point(x=1.5, y=-0.5), head=Point(x=1, y=-1.05), scale=0.5)

epix.pst_format()

fig = epix.render()
fig
