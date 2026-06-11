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
# # saddle — ported from `samples/saddle.xp`
# A quadratic saddle cut by a tangent plane, clipped into two half-spaces
# (`clip_face`/`clip_restore`).

# %%
from __future__ import annotations

import epix
from epix import Point


def F(u: float, v: float) -> epix.Point:
    return Point(x=u, y=v, z=0.25 * (u + v) * (u - v))


def TF(u: float, v: float) -> epix.Point:
    return Point(x=u, y=v)


# %%
with epix.figure(
    lower_left=Point(x=-3, y=-2), upper_right=Point(x=3, y=2), size="6x4in"
) as fig:
    epix.backing(epix.black())
    epix.label_color(epix.white())
    epix.label(
        Point(x=0, y=epix.ymax()),
        offset=Point(x=0, y=-4),
        text=r"\textbf{A quadratic saddle cut by a tangent plane}",
        align=epix.LabelPos.b,
    )
    epix.camera.at(Point(x=6, y=12, z=8))
    epix.camera.look_at(Point(x=0, y=0, z=-0.25))
    R: epix.Domain = epix.Domain(
        lower_left=Point(x=-2, y=-2),
        upper_right=Point(x=2, y=2),
        coarse=epix.Mesh(nx=24, ny=24),
        fine=epix.Mesh(nx=48, ny=48),
    )
    epix.clip_face(
        loc=Point(x=0, y=0, z=0), normal=-epix.E_3
    )  # 2nd arg is inward normal
    epix.fill()
    epix.surface(F, R)
    epix.bold()
    epix.set_green()
    epix.plot(F, R)
    epix.label(
        F(0, 2), offset=Point(x=0, y=-4), text=r"$\mathbf{z<0}$", align=epix.LabelPos.b
    )
    epix.set_red()
    epix.plot(TF, R)
    epix.label(
        Point(x=2, y=0, z=0),
        offset=Point(x=-4, y=-2),
        text=r"$\mathbf{z=0}$",
        align=epix.LabelPos.bl,
    )
    epix.clip_restore()
    epix.clip_face(loc=Point(x=0, y=0, z=0), normal=epix.E_3)
    epix.fill(epix.blue())
    epix.surface(F, R)
    epix.pen(epix.red(), width=0.2)
    epix.plot(TF, R)
    epix.bold(epix.blue())
    epix.plot(F, R)
    epix.set_rgb(0.5, 0.5, 1)
    epix.label(
        F(-2, 0), offset=Point(x=4, y=4), text=r"$\mathbf{z>0}$", align=epix.LabelPos.tr
    )
fig
