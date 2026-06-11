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
# # lighting — ported from `samples/lighting.flx` (+ its `lighting.h`)
# A shaded torus lit by a blue spotlight that traverses a longitude, with fog
# and ambient light. The sample's `lighting.h` shading library (`Spot`, `Fog`,
# `Chip` facet) is reimplemented here in Python; each facet computes its color
# from the spot/ambient/fog model and the facets are painter-sorted.

# %%
from __future__ import annotations

import math
from dataclasses import dataclass, field

import epix
from epix import Point

# mesh intervals for torus
N1 = 36
N2 = 12


# torus
def F(x: float, y: float) -> epix.Point:
    R = 5 - 2 * epix.sin(math.pi * y / N2)
    return Point(
        R * epix.cos(2 * math.pi * x / N1),
        R * epix.sin(2 * math.pi * x / N1),
        2 * epix.cos(math.pi * y / N2),
    )


# --- lighting.h, reimplemented ---
@dataclass
class Spot:
    tint: object
    loc: object


@dataclass
class Fog:
    tint: object
    dens: float

    def apply_to(self, col: epix.Color, dist: float) -> epix.Color:
        return self.tint.blend(col, math.exp(-dist * self.dens))


@dataclass
class Chip:
    pt1: object
    pt2: object
    pt3: object
    pt4: object
    shine: float
    tint: object
    center: object = field(init=False)
    perp: object = field(init=False)

    def __post_init__(self) -> None:
        self.center = 0.25 * (self.pt1 + self.pt2 + self.pt3 + self.pt4)
        n = (self.pt2 - self.pt1) ^ (self.pt4 - self.pt1)
        n = epix.recip(n.norm()) * n
        self.perp = n

    def how_far(self) -> float:
        return (epix.camera.viewpt() - self.center).norm()

    def draw(self, fog: Fog, spot: Spot, ambient: epix.Color) -> None:
        epix.fill(self.appearance(epix.camera.viewpt(), fog, spot, ambient))
        epix.quad(a=self.pt1, b=self.pt2, c=self.pt3, d=self.pt4)

    def appearance(
        self, viewer: epix.Point, fog: Fog, spot: Spot, ambient: epix.Color
    ) -> epix.Color:
        spot_dir = spot.loc - self.center
        spot_dist = spot_dir.norm()
        spot_dir = epix.recip(spot_dist) * spot_dir

        view_dir = viewer - self.center
        view_dist = view_dir.norm()
        view_dir = epix.recip(view_dist) * view_dir

        # |cos| of normal angle
        I_ambt = abs(self.perp | view_dir)

        # reflect spot_dir across perp, then take cos(angle to viewer)^2
        refl_dir = -spot_dir + 2 * (spot_dir | self.perp) * self.perp
        I_spot = math.pow(refl_dir | view_dir, 2)

        # light from spot reaching us and reflected to viewer
        spot_refl = I_spot * fog.apply_to(spot.tint, spot_dist)

        # not shiny -> reflect mostly ambient
        refl = I_ambt * ambient.blend(spot_refl, self.shine)

        # color we reflect toward viewer
        source = self.tint.filter(refl)
        return fog.apply_to(source, view_dist)


# %%
def build() -> None:
    epix.picture(
        lower_left=Point(x=-10, y=-10), upper_right=Point(x=10, y=10), size="4x4in"
    )

    epix.begin()
    epix.set_crop()

    epix.backing(epix.black())

    R: epix.Domain = epix.Domain(
        lower_left=Point(x=0, y=N2),
        upper_right=Point(x=N1, y=2 * N2),
        coarse=epix.Mesh(N1, N2),
        fine=epix.Mesh(2 * N1, 2 * N2),
    )

    # manual layering below assumes this camera
    epix.camera.at(Point(x=4, y=6, z=4))
    epix.camera.range(16)

    # draw grid only
    epix.pen(epix.green(), width=0.15)
    epix.plot(F, R)

    epix.set_rgb(0.8, 0.4, 0)
    epix.fill()
    chip_color: epix.Color = epix.white()  # true color of torus
    ambient: epix.Color = epix.rgb(1, 0.75, 0)  # ambient light

    fog: Fog = Fog(epix.white(), 0.0125)

    # spot light location -- traverses a longitude
    spot_at: epix.Point = epix.sph(
        radius=10, theta=math.pi / 4, phi=2 * math.pi * epix.tix()
    )
    spot: Spot = Spot(epix.blue(), spot_at)

    # torus, surface reflectivity .75
    surf_mesh: list[Chip] = [
        Chip(F(i, j), F(i + 1, j), F(i + 1, j + 1), F(i, j + 1), 0.75, chip_color)
        for i in range(N1)
        for j in range(N2)
    ]

    surf_mesh.sort(key=lambda c: c.how_far(), reverse=True)

    # draw shaded surface mesh
    for c in surf_mesh:
        c.draw(fog, spot, ambient)

    # redraw front portion of mesh partially covered by solid surface
    epix.fill(False)
    epix.pen(epix.green(), width=0.15)
    epix.plot(F, R.resize1(0, 0.25 * N1))

    # labels
    epix.base(epix.white(), width=1)
    epix.line(tail=Point(x=3, y=0, z=6), head=F(0, 0.25 * N2))
    epix.line(tail=Point(x=6, y=0, z=4), head=F(0, -0.25 * N2))

    epix.label_border(epix.green(0.6), "0.5pt")
    epix.label_color(epix.black())
    epix.label_mask(epix.rgb(1, 0.8, 0.2))

    epix.font_size("footnotesize")
    epix.masklabel(
        Point(x=3, y=0, z=6),
        Point(x=0, y=0),
        r"Total curvature $-\pi$",
        epix.LabelPos.t,
    )
    epix.masklabel(
        Point(x=6, y=0, z=4), Point(x=0, y=0), r"Total curvature $\pi$", epix.LabelPos.t
    )

    epix.pst_format()


anim = epix.animate(build, count=24)
anim
