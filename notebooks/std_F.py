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
from __future__ import annotations

import math
from dataclasses import dataclass, field

import epix
from epix import Point


@dataclass
class StdF:
    # the standard F in the unit square [0,1] x [0,1]
    origin: epix.Pair = field(default_factory=lambda: epix.Pair(0, 0))  # lower left
    e1: epix.Pair = field(default_factory=lambda: epix.Pair(1, 0))  # lower right
    e2: epix.Pair = field(default_factory=lambda: epix.Pair(0, 1))  # upper left
    fill_color: epix.Color = field(default_factory=epix.black)
    back_color: epix.Color = field(default_factory=epix.white)
    edge_color: epix.Color = field(default_factory=epix.black)
    edge_width: float = 0.4

    def map_by(self, af: epix.Affine) -> StdF:
        self.origin = af(self.origin)
        self.e1 = af(self.e1)
        self.e2 = af(self.e2)
        return self

    def backing(self, back: epix.Color) -> StdF:
        self.back_color = back
        return self

    def fill(self, fore: epix.Color) -> StdF:
        self.fill_color = fore
        return self

    def border(self, edge: epix.Color, wid: float) -> StdF:
        self.edge_color = edge
        self.edge_width = wid
        return self

    # convert (x, y) in [0,1]^2 to a location
    def pr(self, x: float, y: float) -> epix.Point:
        loc: epix.Pair = (1 - x - y) * self.origin + x * self.e1 + y * self.e2
        return Point(x=loc.x1(), y=loc.x2())

    def draw(self) -> None:
        r: float = 1.0 / 6.0

        F: epix.Path = epix.Path()
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

        epix.fill(self.back_color)
        epix.pen(self.edge_color, self.edge_width)

        # bounding parallelogram
        epix.quad(a=self.pr(0, 0), b=self.pr(1, 0), c=self.pr(1, 1), d=self.pr(0, 1))

        epix.fill(self.fill_color)
        F.draw()


# %%
with epix.figure(
    lower_left=Point(x=0, y=0), upper_right=Point(x=6, y=6), size="4x4in"
) as fig:
    epix.degrees()

    epix.grid(nx=6, ny=6)

    F1: StdF = StdF()  # the standard F
    af1: epix.Affine = epix.Affine()  # the identity map

    F1.draw()

    # stretch along the main diagonal
    af1.rotate(45).v_scale(factor=3).h_scale(factor=0.75).rotate(-45)

    # flip over the main diagonal and translate
    af1.reflect(45).shift(epix.Pair(3, 2))

    # apply af, set style, and draw
    F1.map_by(af1).fill(epix.black(0.6)).backing(epix.blue(1.8)).draw()

    F2: StdF = StdF()
    af2: epix.Affine = epix.Affine()
    af2.rotate(60).scale(factor=1.5).shift(epix.Pair(5, 0))
    F2.map_by(af2).backing(epix.rgb(1, 0.8, 0.2)).fill(epix.green(0.6)).draw()

    F3: StdF = StdF()
    # define af3 by images of e1, e2, origin
    af3: epix.Affine = epix.Affine(
        epix.Pair(2 - math.sqrt(3), 6), epix.Pair(2, 3), epix.Pair(2, 5)
    )
    F3.map_by(af3).backing(epix.black(0.4)).fill(epix.white()).draw()

    epix.pst_format()
fig
