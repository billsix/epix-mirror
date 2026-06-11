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
# # coord_tricks — ported from `samples/coord_tricks.xp`
# Graphing 1/x across broken axes (three `screen` panels with `axis_break`).

# %%
from __future__ import annotations

import epix
from epix import Point

gap = 0.15
GRAY = epix.black(0.5)

# %%
with epix.figure(
    lower_left=Point(x=0, y=0), upper_right=Point(x=2 + gap, y=2 + gap), size="4x4in"
) as fig:
    epix.plain(GRAY)
    epix.axis_break(tail=Point(x=1, y=0), head=Point(x=1 + gap, y=0))
    epix.axis_break(tail=Point(x=0, y=1), head=Point(x=0, y=1 + gap))

    screen_origin: epix.Screen = epix.Screen(
        lower_left=Point(x=0, y=0), upper_right=Point(x=4, y=4)
    )
    epix.activate(screen_origin)
    epix.set_crop()
    epix.grid(nx=8, ny=8)
    epix.h_axis_labels(n=4, offset=Point(x=0, y=-4), align=epix.LabelPos.b)
    epix.v_axis_labels(n=4, offset=Point(x=-4, y=0), align=epix.LabelPos.l)
    epix.bold(epix.red())
    epix.plot(epix.recip, 0, 4, n=120)
    epix.inset(lower_left=Point(x=0, y=0), upper_right=Point(x=1, y=1))

    screen_large_x: epix.Screen = epix.Screen(
        lower_left=Point(x=10, y=0), upper_right=Point(x=14, y=4)
    )
    epix.activate(screen_large_x)
    epix.set_crop()
    epix.plain(GRAY)
    epix.grid(nx=8, ny=8)
    epix.h_axis_labels(n=4, offset=Point(x=0, y=-4), align=epix.LabelPos.b)
    epix.bold(epix.red())
    epix.plot(epix.recip, 10, 14, n=20)
    epix.inset(lower_left=Point(x=1 + gap, y=0), upper_right=Point(x=2 + gap, y=1))

    screen_large_y: epix.Screen = epix.Screen(
        lower_left=Point(x=0, y=10), upper_right=Point(x=4, y=14)
    )
    epix.activate(screen_large_y)
    epix.set_crop()
    epix.plain(GRAY)
    epix.grid(nx=8, ny=8)
    epix.v_axis_labels(n=4, offset=Point(x=-4, y=0), align=epix.LabelPos.l)
    epix.bold(epix.red())
    epix.plot(epix.recip, 0.05, 0.1, n=10)
    epix.inset(lower_left=Point(x=0, y=1 + gap), upper_right=Point(x=1, y=2 + gap))
fig
