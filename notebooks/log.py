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
# # log — ported from `samples/log.xp`
# Two sheets of the Riemann surface of `log`, with the image of the unit circle.
# Surface patches and path elements share the scene and are painter-sorted by
# distance (patches "fudged" farther so their edges don't occlude the paths).
# Patches are octagons shaded by a point light; the path is drawn in green.

# %%
from __future__ import annotations

import enum
import math
from dataclasses import dataclass, field

import epix
from epix import Point


# A mesh element is either a (shaded, filled) surface patch or a path segment.
# An enum reads better than the old is_segment / last_was_seg booleans.
class PatchKind(enum.Enum):
    SURFACE = enum.auto()
    SEGMENT = enum.auto()


LIGHT: epix.Point = Point(x=2, y=2, z=0)  # location of light, for shading
VIEWPT: epix.Point = Point(x=15, y=-10, z=6)

# surface and path mesh fineness
N1: int = 18
N2: int = 80
N3: int = 120

du: float = 4.5 / N1
dv: float = 6.0 / N2
dt: float = 4.0 / N3

# "gap size" between surface mesh elements
EPS: int = 0  # (0.002)


# visual styles
def path_color() -> None:
    epix.set_green(0.8)


def label_color() -> None:
    epix.set_yellow(0.5)


def dot_color() -> None:
    epix.set_red()


def path_width() -> None:
    epix.pen(1.5)


# can represent either a surface element or a path element
@dataclass
class MeshElt:
    last_kind = PatchKind.SURFACE  # static: kind of the previously drawn element

    kind: PatchKind
    fudge: float
    center: object = field(init=False, default=None)
    pt1: object = field(init=False, default=None)
    pt2: object = field(init=False, default=None)
    pt3: object = field(init=False, default=None)
    pt4: object = field(init=False, default=None)
    pt5: object = field(init=False, default=None)
    pt6: object = field(init=False, default=None)
    pt7: object = field(init=False, default=None)
    pt8: object = field(init=False, default=None)

    @classmethod
    def surface(cls, f, u0: float, v0: float) -> MeshElt:
        self: MeshElt = cls(
            kind=PatchKind.SURFACE, fudge=0.25
        )  # fudge = dist increment
        self.pt1 = f(u0 + EPS, v0 + EPS)
        self.pt2 = f(u0 + 0.5 * du, v0 + EPS)
        self.pt3 = f(u0 + du - EPS, v0 + EPS)
        self.pt4 = f(u0 + du - EPS, v0 + 0.5 * dv)
        self.pt5 = f(u0 + du - EPS, v0 + dv - EPS)
        self.pt6 = f(u0 + 0.5 * du, v0 + dv - EPS)
        self.pt7 = f(u0 + EPS, v0 + dv - EPS)
        self.pt8 = f(u0 + EPS, v0 + 0.5 * dv)
        self.center = 0.25 * (self.pt1 + (self.pt3 + (self.pt5 + self.pt7)))
        return self

    @classmethod
    def segment(cls, f, t0: float) -> MeshElt:
        self: MeshElt = cls(kind=PatchKind.SEGMENT, fudge=0)
        self.pt1 = f(t0)
        self.pt2 = f(t0 + 0.25 * dt)
        self.pt3 = f(t0 + 0.5 * dt)
        self.pt4 = f(t0 + 0.75 * dt)
        self.pt5 = f(t0 + dt)
        self.center = 0.333 * (self.pt1 + (self.pt3 + self.pt5))
        return self

    def how_far(self) -> float:
        return self.fudge + (self.center - epix.camera.viewpt()).norm()

    def draw(self) -> None:
        if self.kind is PatchKind.SURFACE:
            normal: epix.Point = (self.pt2 - self.pt1) ^ (self.pt4 - self.pt1)
            normal = (1 / normal.norm()) * normal

            dens: float = 0.5 * (1 - ((normal | LIGHT) / LIGHT.norm()))

            if MeshElt.last_kind is PatchKind.SEGMENT:
                MeshElt.last_kind = PatchKind.SURFACE
                epix.plain()  # reset pen width

            epix.set_black()
            epix.set_gray(dens)

            octagon_points: list[epix.Point] = [
                self.pt1,
                self.pt2,
                self.pt3,
                self.pt4,
                self.pt5,
                self.pt6,
                self.pt7,
                self.pt8,
            ]
            patch: epix.Path = epix.Path(
                octagon_points, True, True
            )  # closed and filled
            patch.draw()
        else:  # segment
            if MeshElt.last_kind is not PatchKind.SEGMENT:
                MeshElt.last_kind = PatchKind.SEGMENT
                path_width()
                path_color()

            segment_points: list[epix.Point] = [
                self.pt1,
                self.pt2,
                self.pt3,
                self.pt4,
                self.pt5,
            ]
            curve: epix.Path = epix.Path(segment_points, False, False)
            curve.draw()


# the maps to be plotted
def C_log(u: float, v: float) -> epix.Point:
    return epix.polar(radius=math.exp(u), theta=math.pi * v) + Point(
        x=0, y=0, z=u + math.pi * v
    )


def C_log1(t: float) -> epix.Point:
    return C_log(0, t)


# %%
epix.bounding_box(Point(x=-6, y=-12), Point(x=6, y=12))
epix.unitlength("1in")
epix.picture(Point(x=4, y=8))

epix.begin()
epix.fill()

epix.degrees()
epix.label(
    Point(x=0, y=epix.ymin()),
    Point(x=0, y=4),
    r"$z=\mathrm{Re}\,\log(x+iy) + \mathrm{Im}\,\log(x+iy)$",
    epix.LabelPos.t,
)
epix.radians()

epix.viewpoint(VIEWPT)
epix.camera.range(20)

mesh_data: list[MeshElt] = [
    MeshElt.surface(C_log, -3 + du * i, -3 + dv * j)
    for i in range(N1)
    for j in range(N2)
]
mesh_data += [MeshElt.segment(C_log1, -2 + i * dt) for i in range(N3)]

mesh_data.sort(key=lambda m: m.how_far(), reverse=True)

for m in mesh_data:
    m.draw()

dot_color()
epix.marker(C_log(0, -2), epix.MarkType.BOX)
epix.marker(C_log(0, 0), epix.MarkType.BOX)
epix.marker(C_log(0, 2), epix.MarkType.BOX)

label_color()
epix.label(C_log(0, -2), Point(x=6, y=0), r"$-2\pi i$", epix.LabelPos.r)
epix.label(C_log(0, 0), Point(x=6, y=0), "$0$", epix.LabelPos.r)
epix.label(C_log(0, 2), Point(x=-6, y=0), r"$2\pi i$", epix.LabelPos.l)

fig: epix.Figure = epix.render()
fig
