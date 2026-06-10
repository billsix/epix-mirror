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
# # cropplot — ported from `samples/cropplot.xp`
# Graphs cropped to a viewport; the second curve has poles at sample points
# (note `g`'s `inf` guard to match C++ double division at the pole).

# %%
import epix
from epix import P


def f(t):
    return 2 * t * (1 - t) * (1 - t)


def g(t):
    d = 1 - t * t
    return 1.0 / d if d != 0.0 else float("inf")


# %%
with epix.figure(P(-2, -4), P(2, 4), "200x200pt") as fig:
    epix.set_crop()
    epix.dashed()
    epix.line(P(-1, epix.ymin()), P(-1, epix.ymax()))
    epix.line(P(1, epix.ymin()), P(1, epix.ymax()))
    epix.solid()
    epix.h_axis(8)
    epix.v_axis(8)
    epix.h_axis_labels(4, P(-1, 2), epix.LabelPos.tl)
    epix.v_axis_labels(4, P(-1, 2), epix.LabelPos.tl)
    epix.plot(f, epix.xmin(), epix.xmax(), 80)
    epix.bold()
    epix.plot(g, epix.xmin(), epix.xmax(), 80)
fig
