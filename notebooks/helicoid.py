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
from __future__ import annotations

import math
from dataclasses import dataclass, field

import epix
from epix import Point

# number of meridians and latitudes
n_merid: int = 72
n_lat: int = 24

VIEWPT: epix.Point = Point(x=4, y=3, z=4)

# locations of "lights"
LIGHT_R: epix.Point = Point(x=10, y=0, z=10)
LIGHT_G: epix.Point = Point(x=0, y=10, z=10)
LIGHT_B: epix.Point = Point(x=0, y=-10, z=10)

# internal constants
facet_shrink: float = 0.0  # facet shrink factor
du: float = 2.0 / n_merid
dv: float = 4.0 / n_lat


# parametrized surfaces
def helicoid(u: float, v: float) -> epix.Point:
    return Point(
        math.sinh(v) * epix.cos(math.pi * u),
        math.sinh(v) * epix.sin(math.pi * u),
        2 * math.pi * u,
    )


def catenoid(u: float, v: float) -> epix.Point:
    return Point(
        x=math.cosh(v) * epix.sin(math.pi * u),
        y=-math.cosh(v) * epix.cos(math.pi * u),
        z=-v,
    )


def morph(u: float, v: float) -> epix.Point:
    return epix.cos(2 * math.pi * epix.tix()) * helicoid(u, v) + epix.sin(
        2 * math.pi * epix.tix()
    ) * catenoid(u, v)


# facet-like class with light reflection
@dataclass
class Element:
    f: object
    u0: float
    v0: float
    pt1: Point = field(init=False)
    pt2: Point = field(init=False)
    pt3: Point = field(init=False)
    pt4: Point = field(init=False)
    distance: float = field(init=False)

    def __post_init__(self) -> None:
        self.pt1 = self.f(self.u0 + facet_shrink, self.v0 + facet_shrink)
        self.pt2 = self.f(self.u0 + du - facet_shrink, self.v0 + facet_shrink)
        self.pt3 = self.f(self.u0 + du - facet_shrink, self.v0 + dv - facet_shrink)
        self.pt4 = self.f(self.u0 + facet_shrink, self.v0 + dv - facet_shrink)
        center: epix.Point = 0.25 * (self.pt1 + (self.pt2 + (self.pt3 + self.pt4)))
        self.distance = (center - epix.camera.viewpt()).norm()

    def how_far(self) -> float:
        return self.distance

    def draw(self) -> None:
        normal: epix.Point = (self.pt2 - self.pt1) ^ (self.pt4 - self.pt1)
        normal = (1 / normal.norm()) * normal

        dens_r: float = 0.75 * (math.pow(normal | LIGHT_R, 2) / (LIGHT_R | LIGHT_R))
        dens_g: float = 0.75 * (math.pow(normal | LIGHT_G, 2) / (LIGHT_G | LIGHT_G))
        dens_b: float = 0.75 * (math.pow(normal | LIGHT_B, 2) / (LIGHT_B | LIGHT_B))

        epix.fill(epix.rgb(dens_r, dens_g, dens_b))
        epix.quad(a=self.pt1, b=self.pt2, c=self.pt3, d=self.pt4)


# %%
def build() -> None:
    MAX: int = 8
    epix.picture(
        lower_left=Point(x=-MAX, y=-MAX), upper_right=Point(x=MAX, y=MAX), size="5x5in"
    )

    epix.begin()
    epix.pen(epix.neutral(), width=0)  # no grid lines

    # draw bounding square for uniform frame size
    epix.backing(epix.black())
    epix.viewpoint(VIEWPT)

    epix.camera.range(20)

    # build surface
    mesh: list[Element] = [
        Element(morph, -1 + du * i, -2 + dv * j)
        for i in range(n_merid)
        for j in range(n_lat)
    ]

    mesh.sort(key=lambda e: e.how_far(), reverse=True)

    for e in mesh:
        e.draw()

    epix.pst_format()


anim: epix.Animation = epix.animate(build, count=24)
anim
