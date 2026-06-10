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
from math import pi

import epix
from epix import P


def polygon(n, th_0=0):
    d_th = epix.full_turn() / n
    poly = epix.path()
    for i in range(n):
        th = th_0 + i * d_th
        vtx = epix.cis(th)
        poly.pt(vtx)
        epix.label_angle(th + 0.25 * epix.full_turn())
        epix.dot(vtx, -8 * vtx, f"${i}$", epix.LabelPos.none)
    poly.close().draw()


# %%
with epix.figure(P(0, 0), P(2, 3), "4 x 6in") as fig:
    epix.border()
    scr = epix.screen(P(-1, -1), P(1, 1))
    epix.activate(scr)
    polygon(6)
    epix.spot(P(1, 0))
    epix.black(0.3)
    epix.line(P(-0.75, 0), P(0.75, 0))
    epix.black()
    epix.arc_arrow(P(0, 0), 0.75, 0, pi / 3)
    epix.arc_arrow(P(0, 0), 0.75, 2 * pi / 3, pi)
    epix.arc_arrow(P(0, 0), 0.75, 4 * pi / 3, 5 * pi / 3)
    scr.scale(0.9)
    epix.inset(P(0, 2), P(1, 3))
    scr.rotate(2 * pi / 3)
    epix.inset(P(0, 1), P(1, 2))
    scr.rotate(2 * pi / 3)
    epix.inset(P(0, 0), P(1, 1))
    scr.rotate(2 * pi / 3).reflect(0)
    epix.inset(P(1, 2), P(2, 3))
    scr.rotate(2 * pi / 3)
    epix.inset(P(1, 1), P(2, 2))
    scr.rotate(2 * pi / 3)
    epix.inset(P(1, 0), P(2, 1))
    epix.deactivate(scr)
    epix.font_size("LARGE")
    epix.label_angle(0)
    epix.masklabel(P(0.5, 2.5), "$e$")
    epix.masklabel(P(0.5, 1.5), r"$\alpha$")
    epix.masklabel(P(0.5, 0.5), r"$\alpha^2$")
    epix.masklabel(P(1.5, 2.5), r"$\beta$")
    epix.masklabel(P(1.5, 1.5), r"$\beta\alpha^2$")
    epix.masklabel(P(1.5, 0.5), r"$\beta\alpha$")
fig
