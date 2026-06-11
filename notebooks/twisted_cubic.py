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
# # twisted_cubic — ported from `samples/twisted_cubic.xp`
# The twisted cubic with its three coordinate-plane projections, as a stereo pair.

# %%
from __future__ import annotations

from collections.abc import Callable

import epix
from epix import Point

MAX = 2.5
XMIN = -2


def cubic(t: float) -> epix.Point:
    return t * Point(x=1, y=t, z=t * t)


def cubic12(t: float) -> epix.Point:
    pt = cubic(t)
    return Point(x=pt.x1(), y=pt.x2(), z=XMIN)


def cubic13(t: float) -> epix.Point:
    pt = cubic(t)
    return Point(x=pt.x1(), y=XMIN, z=pt.x3())


def cubic23(t: float) -> epix.Point:
    pt = cubic(t)
    return Point(x=XMIN, y=pt.x2(), z=pt.x3())


def bars(
    curve1: Callable[[float], epix.Point], curve2: Callable[[float], epix.Point]
) -> None:
    for i in range(21):
        epix.line(tail=curve1(-1 + 0.1 * i), head=curve2(-1 + 0.1 * i))


def draw_frame() -> None:
    epix.set_blue()
    epix.bbold()
    epix.plot(cubic13, -1, 1, n=60)
    epix.plain()
    bars(cubic13, cubic)
    epix.label(
        cubic13(1), offset=Point(x=0, y=2), text="$x$-$z$", align=epix.LabelPos.t
    )
    epix.set_green(0.6)
    epix.bbold()
    epix.plot(cubic23, -1, 1, n=60)
    epix.plain()
    bars(cubic23, cubic)
    epix.label(
        cubic23(1), offset=Point(x=0, y=2), text="$y$-$z$", align=epix.LabelPos.t
    )
    epix.set_magenta()
    epix.bbold()
    epix.plot(cubic12, -1, 1, n=60)
    epix.plain()
    bars(cubic12, cubic)
    epix.label(
        cubic12(1), offset=Point(x=2, y=0), text="$x$-$y$", align=epix.LabelPos.r
    )
    epix.bold(epix.red(1.6))
    epix.base(epix.red(0.6), width="2pt")
    epix.plot(cubic, -1, 1, n=60)


# %%
with epix.figure(
    lower_left=Point(x=-1, y=0), upper_right=Point(x=1.25, y=1), size="4.5 x 2in"
) as fig:
    scr_left: epix.Screen = epix.Screen(
        lower_left=Point(x=-MAX, y=-MAX), upper_right=Point(x=MAX, y=MAX)
    )
    epix.activate(scr_left)
    epix.border(epix.black(), width="1pt")
    epix.camera.at(epix.sph(radius=10, theta=0.8, phi=0.5))
    epix.camera.look_at(Point(x=0, y=0, z=-0.25))
    draw_frame()
    epix.base(epix.neutral())
    scr_right: epix.Screen = epix.Screen(
        lower_left=Point(x=-MAX, y=-MAX), upper_right=Point(x=MAX, y=MAX)
    )
    epix.activate(scr_right)
    epix.border(epix.black(), width="1pt")
    epix.camera.at(epix.sph(radius=10, theta=0.9, phi=0.5))
    epix.camera.look_at(Point(x=0, y=0, z=-0.25))
    draw_frame()
    epix.inset(
        child=scr_left, lower_left=Point(x=0.25, y=0), upper_right=Point(x=1.25, y=1)
    )
    epix.inset(
        child=scr_right, lower_left=Point(x=-1, y=0), upper_right=Point(x=0, y=1)
    )
fig
