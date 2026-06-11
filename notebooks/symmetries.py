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
# # symmetries — ported from `samples/symmetries.xp`
# The dihedral group D₃ acting on a hexagon (`screen` rotate/reflect + `path`).

# %%
from __future__ import annotations

from math import pi

import epix
from epix import Point


def polygon(n: int, th_0: float = 0) -> None:
    d_th = epix.full_turn() / n
    poly = epix.Path()
    for i in range(n):
        th = th_0 + i * d_th
        vtx = epix.cis(th)
        poly.pt(vtx)
        epix.label_angle(th + 0.25 * epix.full_turn())
        epix.dot(vtx, offset=-8 * vtx, text=f"${i}$", align=epix.LabelPos.none)
    poly.close().draw()


# %%
with epix.figure(
    lower_left=Point(x=0, y=0), upper_right=Point(x=2, y=3), size="4 x 6in"
) as fig:
    epix.border()
    panel: epix.Screen = epix.Screen(
        lower_left=Point(x=-1, y=-1), upper_right=Point(x=1, y=1)
    )
    epix.activate(panel)
    polygon(6)
    epix.spot(Point(x=1, y=0))
    epix.set_black(0.3)
    epix.line(tail=Point(x=-0.75, y=0), head=Point(x=0.75, y=0))
    epix.set_black()
    epix.arc_arrow(center=Point(x=0, y=0), radius=0.75, start=0, finish=pi / 3)
    epix.arc_arrow(center=Point(x=0, y=0), radius=0.75, start=2 * pi / 3, finish=pi)
    epix.arc_arrow(
        center=Point(x=0, y=0), radius=0.75, start=4 * pi / 3, finish=5 * pi / 3
    )
    panel.scale(0.9)
    epix.inset(lower_left=Point(x=0, y=2), upper_right=Point(x=1, y=3))
    panel.rotate(2 * pi / 3)
    epix.inset(lower_left=Point(x=0, y=1), upper_right=Point(x=1, y=2))
    panel.rotate(2 * pi / 3)
    epix.inset(lower_left=Point(x=0, y=0), upper_right=Point(x=1, y=1))
    panel.rotate(2 * pi / 3).reflect(0)
    epix.inset(lower_left=Point(x=1, y=2), upper_right=Point(x=2, y=3))
    panel.rotate(2 * pi / 3)
    epix.inset(lower_left=Point(x=1, y=1), upper_right=Point(x=2, y=2))
    panel.rotate(2 * pi / 3)
    epix.inset(lower_left=Point(x=1, y=0), upper_right=Point(x=2, y=1))
    epix.deactivate(panel)
    epix.font_size("LARGE")
    epix.label_angle(0)
    epix.masklabel(Point(x=0.5, y=2.5), "$e$")
    epix.masklabel(Point(x=0.5, y=1.5), r"$\alpha$")
    epix.masklabel(Point(x=0.5, y=0.5), r"$\alpha^2$")
    epix.masklabel(Point(x=1.5, y=2.5), r"$\beta$")
    epix.masklabel(Point(x=1.5, y=1.5), r"$\beta\alpha^2$")
    epix.masklabel(Point(x=1.5, y=0.5), r"$\beta\alpha$")
fig
