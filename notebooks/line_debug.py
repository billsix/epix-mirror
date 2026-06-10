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
import epix
from epix import P


# test objects
def objs():
    epix.rect(P(0, 0), P(1, 1))
    epix.line(P(-1, -1), P(1, -1))


# attribute-setting commands on bool flags
def line_color(arg):
    if arg:
        epix.pen(epix.Red(), 2)
    else:
        epix.pen(epix.Neutral(), 2)


def line_style(arg):
    if arg:
        epix.dashed()


def base_color(col, wid):
    if col:
        epix.base(epix.Blue(1.2), wid)
    else:
        epix.base(epix.Neutral(), wid)


def base_pen(col, wid):
    if wid:
        base_color(col, 4)
    else:
        base_color(col, 1)


def fill_color(arg):
    if arg:
        epix.fill(epix.Yellow())
    else:
        epix.fill(epix.Neutral())


# we'll use 0, 1 as loop indices; convert to bool
def tf(i):
    return i != 0


# where to position the result of a test
def loc(i0, i1, i2, i3, i4):
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
    return P(horiz, vert)


# %%
with epix.figure(P(0, 0), P(4, 8), "6 x 9in") as fig:
    # the tests proper
    for i0 in range(2):
        for i1 in range(2):
            for i2 in range(2):
                for i3 in range(2):
                    for i4 in range(2):
                        scr = epix.screen(P(-1, -1), P(1, 1))
                        epix.activate(scr)

                        epix.solid()  # may need to reset line style
                        epix.border(epix.Green(0.6), "0.1pt")

                        line_color(tf(i0))
                        line_style(tf(i1))
                        base_pen(tf(i2), tf(i3))
                        fill_color(tf(i4))

                        objs()

                        scr.scale(0.9)
                        epix.inset(
                            loc(i0, i1, i2, i3, i4), loc(i0, i1, i2, i3, i4) + P(1, 1)
                        )
                        epix.deactivate(scr)
fig
