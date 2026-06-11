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
# # trig — ported from `samples/trig.flx` (animation)
# A wheel rolling generates a sine wave (two synchronized `screen` panels).

# %%
from __future__ import annotations

import epix
from epix import Point


def f(x: float, t: float) -> epix.Point:
    return Point(x=x, y=epix.sin(x + t))


R: epix.Domain = epix.Domain(
    lower_left=Point(x=0, y=0),
    upper_right=Point(x=1, y=1),
    coarse=epix.Mesh(nx=60, ny=24),
    fine=epix.Mesh(nx=120, ny=72),
)


def build() -> None:
    t = epix.tix()
    epix.picture(lower_left=Point(x=0, y=0), upper_right=Point(x=5, y=2), size="5x2in")
    epix.begin()
    epix.revolutions()
    epix.border()
    epix.set_crop()
    rolling: epix.Screen = epix.Screen(
        lower_left=Point(x=-1, y=-1), upper_right=Point(x=1, y=1)
    )
    waving: epix.Screen = epix.Screen(
        lower_left=Point(x=0, y=-1), upper_right=Point(x=1, y=1)
    )
    with epix.activated(rolling):
        wheel_center: epix.Point = epix.polar(radius=0.5, theta=t)
        rim: epix.Point = epix.polar(radius=0.5, theta=t) + epix.polar(
            radius=0.5, theta=-t
        )
        rim_point2: epix.Point = epix.polar(radius=0.5, theta=t) + epix.polar(
            radius=0.5, theta=0.5 - t
        )
        epix.circle(center=wheel_center, radius=0.5)  # small wheel
        epix.bold()
        epix.circle(center=Point(x=0, y=0), radius=1)  # big wheel
        epix.set_green()
        epix.line(tail=Point(x=1, y=0), head=Point(x=-1, y=0))
        epix.set_blue()
        epix.line(tail=Point(x=0, y=1), head=Point(x=0, y=-1))
        epix.set_red()
        epix.line(tail=wheel_center, head=rim)
        epix.line(tail=wheel_center, head=rim_point2)
        epix.ddot(rim)
        epix.ddot(rim_point2)
    with epix.activated(waving):
        epix.set_crop()
        epix.set_blue()
        epix.plot(f, R.slice2(t))
    epix.inset(child=rolling, lower_left=Point(x=0, y=0), upper_right=Point(x=2, y=2))
    epix.inset(child=waving, lower_left=Point(x=3, y=0), upper_right=Point(x=5, y=2))
    epix.dashed()
    epix.pen(epix.blue(1.6), width=0.15)
    epix.line(tail=Point(x=1, y=1 + epix.sin(t)), head=Point(x=3, y=1) + f(0, t))


# %%
anim: epix.Animation = epix.animate(build, count=24)
anim
