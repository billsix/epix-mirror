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
# # house — ported from `samples/house.flx` (animation)
# A little house at sunset — sky/ground gradients animate with `tix()`.

# %%
from __future__ import annotations

import epix
from epix import Point

DY = 0.1
N = 200
dN = 1.0 / N


def build() -> None:
    t = epix.tix()
    epix.picture(
        lower_left=Point(x=0, y=0), upper_right=Point(x=5, y=3), size="5 x 3in"
    )
    epix.begin()
    epix.set_crop()
    epix.bbold()
    for i in range(2 * N):
        epix.set_rgb(
            (1 - 0.6 * t) * (0.5 - 0.25 * dN * i),
            (1 - 0.6 * t) * (0.5 - 0.25 * dN * i),
            (1 - 0.6 * t) * (1 - 0.2 * dN * i),
        )
        epix.line(tail=Point(x=0, y=1 + i * dN), head=Point(x=5, y=1 + i * dN))
    epix.pen(4)
    epix.revolutions()
    for i in range(1, 11):
        epix.set_rgb((1 - 0.25 * t) * (1 - 0.01 * i), (1 - t) * (1 - 0.015 * i), 0)
        epix.circle(center=Point(x=3 + 0.5 * t, y=2.5 - 2 * t), radius=0.025 * i)
    epix.set_red(0.6 * (1 - 0.6 * t))
    epix.line(tail=Point(x=0.9, y=1.5), head=Point(x=1.5, y=1.8))
    epix.line(tail=Point(x=2.1, y=1.5), head=Point(x=1.5, y=1.8))
    epix.bbold()
    for i in range(N):
        epix.set_green((1 - 0.6 * t) * 0.4 * (1 + i * dN))
        epix.line(tail=Point(x=0, y=i * dN), head=Point(x=5, y=i * dN))
    epix.plain()
    epix.fill()
    epix.set_gray(0.2 + 0.8 * t)
    house: epix.Path = epix.Path()
    house.pt(Point(x=1, y=1 - DY)).pt(Point(x=2, y=1 - DY)).pt(Point(x=2, y=1.5)).pt(
        Point(x=1.5, y=1.75)
    ).pt(Point(x=1, y=1.5))
    house.close().fill()
    house.draw()
    epix.set_gray(0.1 + 0.4 * t)
    epix.rect(lower_left=Point(x=1.2, y=1.1), upper_right=Point(x=1.4, y=1.3))  # window
    epix.pen(2)
    epix.dot_size(diameter=1)
    epix.set_rgb(0.8 * (1 - t), 0.8 * (1 - t), 0)
    epix.ring(Point(x=1.65, y=1.15))  # doorknob
    epix.fill(False)
    epix.plain(epix.black())
    epix.rect(
        lower_left=Point(x=1.6, y=1 - DY), upper_right=Point(x=1.85, y=1.4)
    )  # door frame
    epix.bold(epix.rgb((1 - t) * 0.6, (1 - t) * 0.4, (1 - t) * 0.2))
    epix.grid(
        lower_left=Point(x=1.2, y=1.1), upper_right=Point(x=1.4, y=1.3), nx=2, ny=2
    )  # panes


# %%
anim = epix.animate(build, count=24)
anim
