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
# # line_debug — ported from `samples/line_debug.xp`
# A line/fill style test sheet: all 32 combinations of five boolean attributes
# (line color, line style, base color, base width, fill color), each drawn in its
# own little `screen` panel laid out in a 4×8 grid.

# %%
from __future__ import annotations

import epix
from epix import Point


# test objects
def objs() -> None:
    epix.rect(lower_left=Point(x=0, y=0), upper_right=Point(x=1, y=1))
    epix.line(tail=Point(x=-1, y=-1), head=Point(x=1, y=-1))


# attribute-setting commands on bool flags
def line_color(arg: bool) -> None:
    if arg:
        epix.pen(epix.red(), width=2)
    else:
        epix.pen(epix.neutral(), width=2)


def line_style(arg: bool) -> None:
    if arg:
        epix.dashed()


def base_color(col: bool, wid: int) -> None:
    if col:
        epix.base(epix.blue(1.2), width=wid)
    else:
        epix.base(epix.neutral(), width=wid)


def base_pen(col: bool, wid: bool) -> None:
    if wid:
        base_color(col, 4)
    else:
        base_color(col, 1)


def fill_color(arg: bool) -> None:
    if arg:
        epix.fill(epix.yellow())
    else:
        epix.fill(epix.neutral())


# we'll use 0, 1 as loop indices; convert to bool
def to_bool(i: int) -> bool:
    return i != 0


# where to position the result of a test
def loc(i0: int, i1: int, i2: int, i3: int, i4: int) -> epix.Point:
    horiz, vert = 0, 7
    if to_bool(i0):
        horiz += 1
    if to_bool(i1):
        horiz += 2
    if to_bool(i2):
        vert -= 1
    if to_bool(i3):
        vert -= 2
    if to_bool(i4):
        vert -= 4
    return Point(x=horiz, y=vert)


# %%
with epix.figure(
    lower_left=Point(x=0, y=0), upper_right=Point(x=4, y=8), size="6 x 9in"
) as fig:
    # the tests proper
    for i0 in range(2):
        for i1 in range(2):
            for i2 in range(2):
                for i3 in range(2):
                    for i4 in range(2):
                        panel: epix.Screen = epix.Screen(
                            lower_left=Point(x=-1, y=-1), upper_right=Point(x=1, y=1)
                        )
                        epix.activate(panel)

                        epix.solid()  # may need to reset line style
                        epix.border(epix.green(0.6), width="0.1pt")

                        line_color(to_bool(i0))
                        line_style(to_bool(i1))
                        base_pen(to_bool(i2), to_bool(i3))
                        fill_color(to_bool(i4))

                        objs()

                        panel.scale(factor=0.9)
                        epix.inset(
                            lower_left=loc(i0, i1, i2, i3, i4),
                            upper_right=loc(i0, i1, i2, i3, i4) + Point(x=1, y=1),
                        )
                        epix.deactivate(panel)
fig
