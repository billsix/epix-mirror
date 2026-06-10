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
# # std_F — ported from `samples/std_F.xp` (+ its `std_F.cc` / `std_F.h`)
# Draws the image of a letter "F" (the first roman letter with no symmetries)
# under several affine maps, illustrating how an affine transform acts on a
# shape. The sample's `std_F` helper class (a tiny external library) is
# reimplemented here in Python; it stores only the three corner `pair`s and
# regenerates the F at draw time.

# %%
import math

import epix
from epix import P


class StdF:
    def __init__(self):
        # the standard F in the unit square [0,1] x [0,1]
        self.m_loc = epix.pair(0, 0)  # lower left
        self.m_e1 = epix.pair(1, 0)  # lower right
        self.m_e2 = epix.pair(0, 1)  # upper left
        self.m_fore = epix.Black()
        self.m_back = epix.White()
        self.m_edge = epix.Black()
        self.m_edge_width = 0.4

    def map_by(self, af):
        self.m_loc = af(self.m_loc)
        self.m_e1 = af(self.m_e1)
        self.m_e2 = af(self.m_e2)
        return self

    def backing(self, back):
        self.m_back = back
        return self

    def fill(self, fore):
        self.m_fore = fore
        return self

    def border(self, edge, wid):
        self.m_edge = edge
        self.m_edge_width = wid
        return self

    # convert (x, y) in [0,1]^2 to a location
    def pr(self, x, y):
        loc = (1 - x - y) * self.m_loc + x * self.m_e1 + y * self.m_e2
        return P(loc.x1(), loc.x2())

    def draw(self):
        r = 1.0 / 6.0

        F = epix.path()
        # pr converts coords in [0,1]^2 to our coords
        (
            F.pt(self.pr(r, 0.75 * r))
            .pt(self.pr(2 * r, 0.75 * r))
            .pt(self.pr(2 * r, 2.25 * r))
            .pt(self.pr(4 * r, 2.25 * r))
            .pt(self.pr(4 * r, 3.25 * r))
            .pt(self.pr(2 * r, 3.25 * r))
            .pt(self.pr(2 * r, 4.25 * r))
            .pt(self.pr(5 * r, 4.25 * r))
            .pt(self.pr(5 * r, 5.25 * r))
            .pt(self.pr(r, 5.25 * r))
        )
        F.close().fill()

        epix.fill(self.m_back)
        epix.pen(self.m_edge, self.m_edge_width)

        # bounding parallelogram
        epix.quad(self.pr(0, 0), self.pr(1, 0), self.pr(1, 1), self.pr(0, 1))

        epix.fill(self.m_fore)
        F.draw()


# %%
with epix.figure(P(0, 0), P(6, 6), "4x4in") as fig:
    epix.degrees()

    epix.grid(6, 6)

    F1 = StdF()  # the standard F
    af1 = epix.affine()  # the identity map

    F1.draw()

    # stretch along the main diagonal
    af1.rotate(45).v_scale(3).h_scale(0.75).rotate(-45)

    # flip over the main diagonal and translate
    af1.reflect(45).shift(epix.pair(3, 2))

    # apply af, set style, and draw
    F1.map_by(af1).fill(epix.Black(0.6)).backing(epix.Blue(1.8)).draw()

    F2 = StdF()
    af2 = epix.affine()
    af2.rotate(60).scale(1.5).shift(epix.pair(5, 0))
    F2.map_by(af2).backing(epix.RGB(1, 0.8, 0.2)).fill(epix.Green(0.6)).draw()

    F3 = StdF()
    # define af3 by images of e1, e2, origin
    af3 = epix.affine(epix.pair(2 - math.sqrt(3), 6), epix.pair(2, 3), epix.pair(2, 5))
    F3.map_by(af3).backing(epix.Black(0.4)).fill(epix.White()).draw()

    epix.pst_format()
fig
