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
# # helicoid — ported from `samples/helicoid.flx`
# The local isometric deformation of a catenoid into a helicoid through a family
# of minimal immersions (`morph = cos·helicoid + sin·catenoid`), animated as the
# parameter sweeps a full turn. Each facet is RGB-shaded by three colored lights
# and painter-sorted back-to-front.

# %%
import math

import epix
from epix import P

# number of meridians and latitudes
N1 = 72
N2 = 24

VIEWPT = P(4, 3, 4)

# locations of "lights"
LIGHT_R = P(10, 0, 10)
LIGHT_G = P(0, 10, 10)
LIGHT_B = P(0, -10, 10)

# internal constants
EPS = 0.0  # facet shrink factor
du = 2.0 / N1
dv = 4.0 / N2


# parametrized surfaces
def helicoid(u, v):
    return P(
        math.sinh(v) * epix.Cos(math.pi * u),
        math.sinh(v) * epix.Sin(math.pi * u),
        2 * math.pi * u,
    )


def catenoid(u, v):
    return P(
        math.cosh(v) * epix.Sin(math.pi * u), -math.cosh(v) * epix.Cos(math.pi * u), -v
    )


def morph(u, v):
    return epix.Cos(2 * math.pi * epix.tix()) * helicoid(u, v) + epix.Sin(
        2 * math.pi * epix.tix()
    ) * catenoid(u, v)


# facet-like class with light reflection
class Element:
    def __init__(self, f, u0, v0):
        self.pt1 = f(u0 + EPS, v0 + EPS)
        self.pt2 = f(u0 + du - EPS, v0 + EPS)
        self.pt3 = f(u0 + du - EPS, v0 + dv - EPS)
        self.pt4 = f(u0 + EPS, v0 + dv - EPS)
        center = 0.25 * (self.pt1 + (self.pt2 + (self.pt3 + self.pt4)))
        self.distance = (center - epix.camera.viewpt()).norm()

    def how_far(self):
        return self.distance

    def draw(self):
        normal = (self.pt2 - self.pt1) ^ (self.pt4 - self.pt1)
        normal = (1 / normal.norm()) * normal

        dens_r = 0.75 * (math.pow(normal | LIGHT_R, 2) / (LIGHT_R | LIGHT_R))
        dens_g = 0.75 * (math.pow(normal | LIGHT_G, 2) / (LIGHT_G | LIGHT_G))
        dens_b = 0.75 * (math.pow(normal | LIGHT_B, 2) / (LIGHT_B | LIGHT_B))

        epix.fill(epix.RGB(dens_r, dens_g, dens_b))
        epix.quad(self.pt1, self.pt2, self.pt3, self.pt4)


# %%
def build():
    MAX = 8
    epix.picture(P(-MAX, -MAX), P(MAX, MAX), "5x5in")

    epix.begin()
    epix.pen(epix.Neutral(), 0)  # no grid lines

    # draw bounding square for uniform frame size
    epix.backing(epix.Black())
    epix.viewpoint(VIEWPT)

    epix.camera.range(20)

    # build surface
    mesh = [
        Element(morph, -1 + du * i, -2 + dv * j) for i in range(N1) for j in range(N2)
    ]

    mesh.sort(key=lambda e: e.how_far(), reverse=True)

    for e in mesh:
        e.draw()

    epix.pst_format()


anim = epix.animate(build, count=24)
anim
