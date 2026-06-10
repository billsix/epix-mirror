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
import epix
from epix import P

gap = 0.15
GRAY = epix.Black(0.5)

# %%
with epix.figure(P(0, 0), P(2 + gap, 2 + gap), "4x4in") as fig:
    epix.plain(GRAY)
    epix.axis_break(P(1, 0), P(1 + gap, 0))
    epix.axis_break(P(0, 1), P(0, 1 + gap))

    scr1 = epix.screen(P(0, 0), P(4, 4))
    epix.activate(scr1)
    epix.set_crop()
    epix.grid(8, 8)
    epix.h_axis_labels(4, P(0, -4), epix.LabelPos.b)
    epix.v_axis_labels(4, P(-4, 0), epix.LabelPos.l)
    epix.bold(epix.Red())
    epix.plot(epix.recip, 0, 4, 120)
    epix.inset(P(0, 0), P(1, 1))

    scr2 = epix.screen(P(10, 0), P(14, 4))
    epix.activate(scr2)
    epix.set_crop()
    epix.plain(GRAY)
    epix.grid(8, 8)
    epix.h_axis_labels(4, P(0, -4), epix.LabelPos.b)
    epix.bold(epix.Red())
    epix.plot(epix.recip, 10, 14, 20)
    epix.inset(P(1 + gap, 0), P(2 + gap, 1))

    scr3 = epix.screen(P(0, 10), P(4, 14))
    epix.activate(scr3)
    epix.set_crop()
    epix.plain(GRAY)
    epix.grid(8, 8)
    epix.v_axis_labels(4, P(-4, 0), epix.LabelPos.l)
    epix.bold(epix.Red())
    epix.plot(epix.recip, 0.05, 0.1, 10)
    epix.inset(P(0, 1 + gap), P(1, 2 + gap))
fig
