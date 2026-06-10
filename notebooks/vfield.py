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
# # vfield — ported from `samples/vfield.xp`
# A 3-D vector field with several integral curves (`ode_plot`) and a dart field.

# %%
from math import pi

import epix
from epix import P


def F(x, y, z):
    return -0.25 * P(0.5 * x + y, 0.5 * y - x, 1 + z)


R = epix.domain(P(-1, -1, -1), P(1, 1, 1), epix.mesh(6, 6, 4), epix.mesh())

# %%
with epix.figure(P(-2, -2), P(2, 2), "4x4in") as fig:
    epix.border()
    epix.label(
        P(0, epix.ymax()), P(0, -4), "$F(x,y,z)=-(x+2y, y-2x, 1+z)$", epix.LabelPos.b
    )
    epix.camera.at(P(11, 8, 5))
    GRIDS = 6
    epix.plain(epix.Black(0.3))
    epix.grid(P(-1, -1, -1), P(-1, 1, 1), GRIDS, GRIDS)
    epix.grid(P(-1, -1, -1), P(1, -1, 1), GRIDS, GRIDS)
    epix.grid(P(-1, -1, -1), P(1, 1, -1), GRIDS, GRIDS)
    epix.bold()
    for i in range(12):
        t = pi / 2 + i * pi / 6
        epix.pen(epix.RGB(0.25 * (3 + epix.Sin(t)), 0.25, 0.25 * (3 + epix.Cos(t))))
        epix.ode_plot(F, epix.cyl(1.4, t, 1), 0, 10, 60)
    epix.plain(epix.Black())
    epix.base(epix.White(), "2pt")
    epix.dart_field(F, R, 0.5)  # darts at 50% of default length
    DX = 0.125
    epix.font_size("scriptsize")
    epix.degrees()
    epix.label_angle(30)
    epix.label(P(1 + DX, -1, -1), P(-4, -2), "$y=-1$", epix.LabelPos.bl)
    epix.label(P(1 + DX, 0, -1), P(-4, -2), "$y=0$", epix.LabelPos.bl)
    epix.label(P(1 + DX, 1, -1), P(-4, -2), "$y=1$", epix.LabelPos.bl)
    epix.label_angle(-15)
    epix.label(P(-1, 1 + DX, -1), P(4, 0), "$x=-1$", epix.LabelPos.br)
    epix.label(P(0, 1 + DX, -1), P(4, 0), "$x=0$", epix.LabelPos.br)
    epix.label(P(1, 1 + DX, -1), P(4, 0), "$x=1$", epix.LabelPos.br)
    epix.label(P(1, -1 - DX, -1), P(-4, 0), "$z=-1$", epix.LabelPos.tl)
    epix.label(P(1, -1 - DX, 0), P(-4, 0), "$z=0$", epix.LabelPos.tl)
    epix.label(P(1, -1 - DX, 1), P(-4, 0), "$z=1$", epix.LabelPos.tl)
fig
