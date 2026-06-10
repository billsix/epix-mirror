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
import math

import epix
from epix import P

MAX = 3


# hyperboloid of one sheet / spacelike cone
def Fp(x, y, z):
    return P(
        z * math.cosh(x) * epix.Cos(y), z * math.cosh(x) * epix.Sin(y), z * math.sinh(x)
    )


# hyperboloid of two sheets / timelike cone
def Fm(x, y, z):
    return P(
        z * math.sinh(x) * epix.Cos(y), z * math.sinh(x) * epix.Sin(y), z * math.cosh(x)
    )


# light cone
def Fz(x, y):
    return abs(math.sinh(x)) * P(epix.Cos(y), epix.Sin(y), epix.sgn(x))


# hyperbolic lines
def hl1(t):
    return math.cosh(t) * P(-math.sqrt(1.25), 0, 1.5) + math.sinh(t) * epix.E_2


def hl2(t):
    return math.cosh(t) * P(-1, 0.5, 1.5) + (math.sqrt(0.4) * math.sinh(t)) * P(
        0.5, 1, 0
    )


# %%
epix.bounding_box(P(-MAX, -MAX), P(MAX, MAX))
epix.unitlength("1in")
epix.picture(6, 6)

epix.begin()
epix.camera.at(10, -3, 4)

epix.revolutions()

R = epix.domain(
    P(-1.5, 0.25, -1), P(1.5, 0.75, 1), epix.mesh(18, 12, 4), epix.mesh(36, 48, 12)
)

epix.fill(epix.White())

# assemble surfaces as a single object
hyp = epix.scenery(Fp, R.slice3(1))
hyp.add(Fm, R.resize1(0, 1.5).slice3(1))
hyp.add(Fm, R.resize1(-1.5, 0).slice3(-1))
hyp.add(Fz, R.slice3(1))

hyp.draw()
epix.nofill()

# Draw cuts of intersection with (y,z)-plane.
# Simulated 3-D/tube pen: light color drawn over darker shade
epix.base(epix.Red(0.6), "3pt")
epix.bold(epix.Red())

epix.plot(Fp, R.slice2(0.25).slice3(1))
epix.plot(Fp, R.slice2(0.25).slice3(-1))

epix.plot(Fm, R.slice2(0.25).slice3(1))
epix.plot(Fm, R.slice2(0.25).slice3(-1))

epix.plot(Fz, R.slice2(0.25).slice3(1))
epix.plot(Fz, R.slice2(0.75).slice3(-1))

# to clip lines
epix.clip_box(P(-10, -10, -math.cosh(1.5)), P(10, 10, math.cosh(1.5)))

epix.base(epix.Green(0.6), "3pt")
epix.bold(epix.Green())

epix.plot(hl1, -1.5, 1.5, 120)
epix.plot(hl2, -1.5, 1.5, 120)

# Labels with "overlay" arrows
epix.font_size("scriptsize")

epix.black()
epix.camera.at(P(0, 0, 10000))
epix.label(P(1.5, 0), P(4, 16), "Future timelike unit vectors", epix.LabelPos.r)
epix.label(P(1.5, 0), P(4, 0), "Unit spacelike vectors", epix.LabelPos.r)
epix.label(P(1.5, 0), P(4, -16), "Lightlike vectors", epix.LabelPos.r)
epix.label(P(1.5, 0), P(4, -32), "Past timelike unit vectors", epix.LabelPos.r)

epix.base(epix.White(), "2pt")
epix.bold(epix.Black())
epix.arrow(P(1.5, 0), P(1, 0), 0.5)
epix.arrow(P(1.5, 0.25), P(1, 1.35), 0.5)
epix.arrow(P(1.5, -0.25), P(1, -0.75), 0.5)
epix.arrow(P(1.5, -0.5), P(1, -1.05), 0.5)

epix.pst_format()

fig = epix.render()
fig
