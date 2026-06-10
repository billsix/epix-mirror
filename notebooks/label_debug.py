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
import epix
from epix import P


# test objects
def objs():
    epix.label(P(-0.5, 0), "The")
    epix.masklabel(P(0.5, 0), "The")


# attribute-setting commands on bool flags
def label_color(arg):
    if arg:
        epix.label_color(epix.Red())
    else:
        epix.label_color(epix.Neutral())


def label_mask(arg):
    if arg:
        epix.label_mask(epix.Yellow(0.3))
    else:
        epix.label_mask(epix.Neutral())


def set_border(col, length):
    if col:
        epix.label_border(epix.Blue(1.2), length)
    else:
        epix.label_border(epix.Neutral(), length)


def label_border(col, wid):
    if wid:
        set_border(col, "1pt")
    else:
        set_border(col, "0pt")


def pad(arg):
    if arg:
        epix.label_pad("6pt")
    else:
        epix.label_pad("0pt")


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

                        epix.backing(epix.Black(0.1))
                        epix.pen(epix.Black(0.3))
                        epix.grid(8, 8)

                        label_color(tf(i0))
                        label_mask(tf(i1))
                        label_border(tf(i2), tf(i3))
                        pad(tf(i4))

                        objs()

                        scr.scale(0.9)
                        epix.inset(
                            loc(i0, i1, i2, i3, i4), loc(i0, i1, i2, i3, i4) + P(1, 1)
                        )
                        epix.deactivate(scr)
fig
