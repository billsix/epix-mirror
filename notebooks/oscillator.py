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
# # oscillator — ported from `samples/oscillator.xp`
# Predicted vs measured displacement of a damped harmonic oscillator (error bars + legend).

# %%
from math import exp, pi

import epix
from epix import P


def f(t, a):
    return 2 * exp(-0.5 * t) * epix.Sin(a * t)


def f1(t):
    return f(t, 2)


def f2(t):
    return f1(t) + 0.1 * epix.Cos(3 * t)


def err(t):
    return 0.125 * (1 + abs(f1(t)))


samples = 8
N = 2

# %%
with epix.figure(P(0, -N), P(2 * N, N), "2.5 x 2.5in") as fig:
    delta = 0.0625 * epix.xsize()
    epix.pen(epix.Black(0.25))
    epix.grid(5 * N * int(epix.xsize()), int(5 * N * epix.ysize()))
    epix.pen(epix.Black())
    epix.grid(N * int(epix.xsize()), N * int(epix.ysize()))
    epix.bold()
    epix.dart(P(epix.xmin(), 0), P(epix.xmax() + delta, 0))
    epix.dart(P(0, epix.ymin()), P(0, epix.ymax() + delta))
    epix.font_size("scriptsize")
    epix.h_axis_masklabels(
        P(epix.xmin() + 1, 0),
        P(epix.xmax(), 0),
        int(epix.xsize()) - 1,
        P(0, 2),
        epix.LabelPos.c,
    )
    epix.v_axis_labels(
        P(0, epix.ymin()),
        P(0, epix.ymax()),
        int(epix.ysize()),
        P(-2, 0),
        epix.LabelPos.l,
    )
    epix.font_size("footnotesize")
    epix.label(P(epix.xmax() + delta, 0), P(2, 0), "Time (sec)", epix.LabelPos.r)
    epix.label_angle(pi / 2)
    epix.label(P(0, 0), P(-8, 0), "Displacement (cm)", epix.LabelPos.l)
    epix.label_angle(0)
    L = epix.legend()
    L.item_border(0)
    epix.pen(epix.RGB(0.2, 0.7, 0.8))
    epix.plot(f1, epix.xmin(), epix.xmax(), 120)
    L.path_item("Predicted")
    epix.red()
    L.mark_item(epix.MarkType.CIRC, "Measured")
    for i in range(1, samples):
        t = epix.xmin() + i * epix.xsize() / samples
        epix.v_error_bar(P(t, f2(t)), err(t), epix.MarkType.CIRC, 4)
    epix.black()
    L.draw(epix.canvas().br(), P(-2, 2), epix.LabelPos.tl)
    epix.font_face("sc")
    epix.label(
        P(0.5 * (epix.xmin() + epix.xmax()), epix.ymax()),
        P(0, 2),
        "A damped harmonic oscillator",
        epix.LabelPos.t,
    )
fig
