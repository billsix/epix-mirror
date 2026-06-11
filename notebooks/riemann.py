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
# # riemann — ported from `samples/riemann.flx` (animation)
# The Riemann surface of Re √(x+iy), sliced and rotated through the family.

# %%
from __future__ import annotations

import epix
from epix import Point


def F(t: float, r: float, theta: float) -> epix.Point:
    return Point(
        r * r * epix.cos(2 * theta),
        r * r * epix.cos(t) * epix.sin(2 * theta) + r * epix.sin(t) * epix.sin(theta),
        r * epix.cos(theta),
    )


R: epix.Domain = epix.Domain(
    lower_left=Point(x=0, y=0, z=0),
    upper_right=Point(x=1, y=1.5, z=1),
    coarse=epix.Mesh(nx=24, ny=8, nz=32),
    fine=epix.Mesh(nx=24, ny=40, nz=80),
)


def build() -> None:
    t = epix.tix()
    R1: epix.Domain = R.slice1(t)
    epix.picture(
        lower_left=Point(x=-4, y=-4), upper_right=Point(x=4, y=4), size="5x5in"
    )
    epix.begin()
    epix.revolutions()
    epix.grid(nx=1, ny=1)
    ctr: epix.Point = Point(x=3.25, y=-3.25)  # "clock" center
    epix.bold()
    epix.circle(center=ctr, radius=0.5)
    epix.line(tail=ctr, head=ctr + epix.polar(radius=0.4, theta=t))
    epix.label(ctr + epix.polar(radius=0.6, theta=t), "$y$")
    epix.line(tail=ctr, head=ctr + epix.polar(radius=0.4, theta=t + 0.25))
    epix.label(ctr + epix.polar(radius=0.6, theta=t + 0.25), "$w$")
    epix.viewpoint(4, 5, 3)
    epix.camera.range(20)
    epix.line(tail=Point(x=0, y=0, z=0), head=Point(x=3, y=0, z=0))
    epix.line(tail=Point(x=0, y=0, z=0), head=Point(x=0, y=3, z=0))
    epix.line(tail=Point(x=0, y=0, z=0), head=Point(x=0, y=0, z=2))
    epix.plain(epix.red())
    epix.plot(F, R1.resize3(0, 0.5))
    epix.set_blue()
    epix.plot(F, R1.resize3(0.5, 1))
    epix.bold(epix.magenta())
    epix.plot(F, R.slice3(0))
    epix.plot(F, R.slice3(0.5))
    epix.set_black()
    epix.masklabel(Point(x=3, y=0, z=0), "$x$")
    epix.masklabel(Point(x=0, y=3, z=0), "$y$")
    epix.masklabel(
        Point(x=0, y=0, z=2),
        offset=Point(x=-4, y=0),
        text=r"$z=\textrm{Re}\,\sqrt{x+iy}$",
        align=epix.LabelPos.tr,
    )


# %%
anim = epix.animate(build, count=24)
anim
