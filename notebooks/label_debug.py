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
# # label_debug — ported from `samples/label_debug.xp`
# A font/label style test sheet: all 32 combinations of five boolean label
# attributes (color, mask, border color, border width, pad), each drawn in its
# own little `screen` panel over a faint grid, laid out in a 4×8 grid.

# %%
from __future__ import annotations

import epix
from epix import Point


# test objects
def objs() -> None:
    epix.label(Point(x=-0.5, y=0), "The")
    epix.masklabel(Point(x=0.5, y=0), "The")


# attribute-setting commands on bool flags
def label_color(arg: bool) -> None:
    if arg:
        epix.label_color(epix.red())
    else:
        epix.label_color(epix.neutral())


def label_mask(arg: bool) -> None:
    if arg:
        epix.label_mask(epix.yellow(0.3))
    else:
        epix.label_mask(epix.neutral())


def set_border(col: bool, length: str) -> None:
    if col:
        epix.label_border(epix.blue(1.2), length)
    else:
        epix.label_border(epix.neutral(), length)


def label_border(col: bool, wid: bool) -> None:
    if wid:
        set_border(col, "1pt")
    else:
        set_border(col, "0pt")


def pad(arg: bool) -> None:
    if arg:
        epix.label_pad("6pt")
    else:
        epix.label_pad("0pt")


# we'll use 0, 1 as loop indices; convert to bool
def tf(i: int) -> bool:
    return i != 0


# where to position the result of a test
def loc(i0: int, i1: int, i2: int, i3: int, i4: int) -> epix.Point:
    horiz, vert = 0, 7
    if tf(i0):
        horiz += 1
    if tf(i1):
        horiz += 2
    if tf(i2):
        vert -= 1
    if tf(i3):
        vert -= 2
    if tf(i4):
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
                        with epix.activated(panel):
                            epix.solid()  # may need to reset line style
                            epix.border(epix.green(0.6), width="0.1pt")

                            epix.backing(epix.black(0.1))
                            epix.pen(epix.black(0.3))
                            epix.grid(nx=8, ny=8)

                            label_color(tf(i0))
                            label_mask(tf(i1))
                            label_border(tf(i2), tf(i3))
                            pad(tf(i4))

                            objs()

                            panel.scale(factor=0.9)
                            epix.inset(
                                lower_left=loc(i0, i1, i2, i3, i4),
                                upper_right=loc(i0, i1, i2, i3, i4) + Point(x=1, y=1),
                            )
fig
