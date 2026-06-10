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
import math

import epix
from epix import P

# mesh intervals for torus
N1 = 36
N2 = 12


# torus
def F(x, y):
    R = 5 - 2 * epix.Sin(math.pi * y / N2)
    return P(
        R * epix.Cos(2 * math.pi * x / N1),
        R * epix.Sin(2 * math.pi * x / N1),
        2 * epix.Cos(math.pi * y / N2),
    )


# --- lighting.h, reimplemented ---
class Spot:
    def __init__(self, col, loc):
        self.m_tint = col
        self.m_loc = loc


class Fog:
    def __init__(self, c, k):
        self.m_tint = c
        self.m_dens = k

    def apply_to(self, col, dist):
        return self.m_tint.blend(col, math.exp(-dist * self.m_dens))


class Chip:
    def __init__(self, p1, p2, p3, p4, s, c):
        self.pt1, self.pt2, self.pt3, self.pt4 = p1, p2, p3, p4
        self.m_shine = s
        self.m_tint = c
        self.m_ctr = 0.25 * (p1 + p2 + p3 + p4)
        n = (p2 - p1) ^ (p4 - p1)
        n = epix.recip(n.norm()) * n
        self.m_perp = n

    def how_far(self):
        return (epix.camera.viewpt() - self.m_ctr).norm()

    def draw(self, fog, spot, ambient):
        epix.fill(self.appearance(epix.camera.viewpt(), fog, spot, ambient))
        epix.quad(self.pt1, self.pt2, self.pt3, self.pt4)

    def appearance(self, viewer, fog, spot, ambient):
        spot_dir = spot.m_loc - self.m_ctr
        spot_dist = spot_dir.norm()
        spot_dir = epix.recip(spot_dist) * spot_dir

        view_dir = viewer - self.m_ctr
        view_dist = view_dir.norm()
        view_dir = epix.recip(view_dist) * view_dir

        # |cos| of normal angle
        I_ambt = abs(self.m_perp | view_dir)

        # reflect spot_dir across m_perp, then take cos(angle to viewer)^2
        refl_dir = -spot_dir + 2 * (spot_dir | self.m_perp) * self.m_perp
        I_spot = math.pow(refl_dir | view_dir, 2)

        # light from spot reaching us and reflected to viewer
        spot_refl = I_spot * fog.apply_to(spot.m_tint, spot_dist)

        # not shiny -> reflect mostly ambient
        refl = I_ambt * ambient.blend(spot_refl, self.m_shine)

        # color we reflect toward viewer
        source = self.m_tint.filter(refl)
        return fog.apply_to(source, view_dist)


# %%
def build():
    epix.picture(P(-10, -10), P(10, 10), "4x4in")

    epix.begin()
    epix.set_crop()

    epix.backing(epix.Black())

    R = epix.domain(
        P(0, N2), P(N1, 2 * N2), epix.mesh(N1, N2), epix.mesh(2 * N1, 2 * N2)
    )

    # manual layering below assumes this camera
    epix.camera.at(P(4, 6, 4))
    epix.camera.range(16)

    # draw grid only
    epix.pen(epix.Green(), 0.15)
    epix.plot(F, R)

    epix.rgb(0.8, 0.4, 0)
    epix.fill()
    chip_color = epix.White()  # true color of torus
    ambient = epix.RGB(1, 0.75, 0)  # ambient light

    fog = Fog(epix.White(), 0.0125)

    # spot light location -- traverses a longitude
    spot_at = epix.sph(10, math.pi / 4, 2 * math.pi * epix.tix())
    spot = Spot(epix.Blue(), spot_at)

    # torus, surface reflectivity .75
    surf_mesh = [
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
    epix.pen(epix.Green(), 0.15)
    epix.plot(F, R.resize1(0, 0.25 * N1))

    # labels
    epix.base(epix.White(), 1)
    epix.line(P(3, 0, 6), F(0, 0.25 * N2))
    epix.line(P(6, 0, 4), F(0, -0.25 * N2))

    epix.label_border(epix.Green(0.6), "0.5pt")
    epix.label_color(epix.Black())
    epix.label_mask(epix.RGB(1, 0.8, 0.2))

    epix.font_size("footnotesize")
    epix.masklabel(P(3, 0, 6), P(0, 0), r"Total curvature $-\pi$", epix.LabelPos.t)
    epix.masklabel(P(6, 0, 4), P(0, 0), r"Total curvature $\pi$", epix.LabelPos.t)

    epix.pst_format()


anim = epix.animate(build, count=24)
anim
