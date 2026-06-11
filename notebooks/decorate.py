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
# # decorate — ported from `samples/decorate.xp`
# A torus drawn facet-by-facet as a `mesh_quad` class, each quad shaded by the
# angle between its normal and the view direction and decorated with a diagonal
# "line field". Facets are painter-sorted back-to-front by distance to the eye.
#
# The C++ default (`#define LINEFIELD`, `NORMAL` off) is what's ported here.

# %%
from __future__ import annotations

from dataclasses import dataclass, field

import epix
from epix import Point

n_lat: int = 24  # latitudes
n_long: int = 48  # longitudes

du: float = 1.0 / n_lat
dv: float = 1.0 / n_long

VIEWPT: epix.Point = Point(x=6, y=3, z=4)

r_0: float = 0.95  # minor radius
R_0: int = 2  # major radius


def g(u: float) -> float:
    return R_0 + r_0 * epix.cos(u)


def F(u: float, v: float) -> epix.Point:
    return epix.polar(radius=g(u), theta=v) + Point(x=0, y=0, z=r_0 * epix.sin(u))


# a facet-like class that can be drawn with extra decorations (here, a diagonal
# "line field" and orientation-dependent colors)
@dataclass
class MeshQuad:
    f: object
    u0: float
    v0: float
    pt1: Point = field(init=False)
    pt2: Point = field(init=False)
    pt3: Point = field(init=False)
    pt4: Point = field(init=False)
    center: Point = field(init=False)
    distance: float = field(init=False)

    def __post_init__(self) -> None:
        self.pt1 = self.f(self.u0, self.v0)
        self.pt2 = self.f(self.u0 + du, self.v0)
        self.pt3 = self.f(self.u0 + du, self.v0 + dv)
        self.pt4 = self.f(self.u0, self.v0 + dv)
        self.center = 0.25 * (self.pt1 + self.pt2 + self.pt3 + self.pt4)
        self.distance = (self.center - epix.camera.viewpt()).norm()

    def how_far(self) -> float:
        return self.distance

    def draw(self) -> None:
        direction: epix.Point = self.center - epix.camera.viewpt()
        normal: epix.Point = (self.pt2 - self.pt1) ^ (self.pt4 - self.pt1)
        normal = (1 / normal.norm()) * normal

        epix.set_blue(0.75 * (normal | (epix.recip(self.distance) * direction)))

        epix.fill()
        epix.quad(a=self.pt1, b=self.pt2, c=self.pt3, d=self.pt4)
        epix.fill(False)

        if (normal | direction) > 0:
            epix.bbold(epix.blue(1.8))
        else:
            epix.bold(epix.red())

        epix.line(tail=self.pt1, head=0.5 * (self.pt3 + self.pt4))
        epix.line(tail=0.5 * (self.pt1 + self.pt2), head=self.pt3)


# %%
with epix.figure(
    lower_left=Point(x=-3, y=-3), upper_right=Point(x=3, y=3), size="4x4in"
) as fig:
    epix.revolutions()

    epix.viewpoint(VIEWPT)
    epix.camera.range(10)

    # chop off the front
    epix.clip_face(loc=Point(x=R_0, y=0, z=r_0), normal=Point(x=-0.25, y=-0.25, z=-1))

    # build and draw a torus
    quads: list[MeshQuad] = [
        MeshQuad(F, i * du, j * dv) for i in range(n_lat) for j in range(n_long)
    ]

    quads.sort(key=lambda quad: quad.how_far(), reverse=True)

    for quad in quads:
        quad.draw()

    epix.pst_format()
fig
