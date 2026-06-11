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
# # lorenz — ported from `samples/lorenz.xp`
# The Lorenz attractor as a stereo pair (two `screen`s at slightly different
# camera angles), with the flow field and two trajectories.

# %%
from __future__ import annotations

import epix
from epix import Point

sigma: int = 10
rho: int = 28
beta: float = 2.6666
MAX: int = 30
XMAX: int = 50
TMAX: int = 2000


def F(x: float, y: float, z: float) -> epix.Point:
    return 0.01 * Point(x=sigma * (y - x), y=x * (rho - z) - y, z=x * y - beta * z)


R: epix.Domain = epix.Domain(
    lower_left=Point(x=-MAX, y=-MAX, z=0),
    upper_right=Point(x=MAX, y=MAX, z=2 * MAX),
    coarse=epix.Mesh(6, 6, 6),
)


def draw_frame() -> None:
    epix.dart_field(F, R.resize1(0, MAX), scale=0.5)
    epix.bold(epix.red())
    epix.ode_plot(F, Point(x=10, y=0, z=0), 0, TMAX, n=TMAX)
    epix.pen(epix.blue())
    epix.ode_plot(F, Point(x=10.5, y=0, z=0), 0, TMAX, n=TMAX)
    epix.plain(epix.black())
    epix.dart_field(F, R.resize1(-MAX, 0), scale=0.5)


def init_cam(arg: epix.Point) -> None:
    epix.camera.at(arg)
    epix.camera.look_at(Point(x=0, y=0, z=MAX))
    epix.camera.range(500)


# %%
with epix.figure(
    lower_left=Point(x=-1, y=0), upper_right=Point(x=1, y=1), size="6x3in"
) as fig:
    left_eye: epix.Screen = epix.Screen(
        lower_left=Point(x=-XMAX, y=-XMAX), upper_right=Point(x=XMAX, y=XMAX)
    )
    epix.activate(left_eye)
    init_cam(Point(x=-100, y=90, z=63))
    draw_frame()
    right_eye: epix.Screen = epix.Screen(
        lower_left=Point(x=-XMAX, y=-XMAX), upper_right=Point(x=XMAX, y=XMAX)
    )
    epix.activate(right_eye)
    init_cam(Point(x=-100, y=80, z=63))
    draw_frame()
    epix.inset(child=left_eye, lower_left=Point(x=0, y=0), upper_right=Point(x=1, y=1))
    epix.inset(
        child=right_eye, lower_left=Point(x=-1, y=0), upper_right=Point(x=0, y=1)
    )
fig
