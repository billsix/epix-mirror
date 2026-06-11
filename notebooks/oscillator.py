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
from __future__ import annotations

from math import exp, pi

import epix
from epix import Point


def f(t: float, a: float) -> float:
    return 2 * exp(-0.5 * t) * epix.sin(a * t)


def f1(t: float) -> float:
    return f(t, 2)


def f2(t: float) -> float:
    return f1(t) + 0.1 * epix.cos(3 * t)


def err(t: float) -> float:
    return 0.125 * (1 + abs(f1(t)))


samples: int = 8
N: int = 2

# %%
with epix.figure(
    lower_left=Point(x=0, y=-N), upper_right=Point(x=2 * N, y=N), size="2.5 x 2.5in"
) as fig:
    delta: float = 0.0625 * epix.xsize()
    epix.pen(epix.black(0.25))
    epix.grid(nx=5 * N * int(epix.xsize()), ny=int(5 * N * epix.ysize()))
    epix.pen(epix.black())
    epix.grid(nx=N * int(epix.xsize()), ny=N * int(epix.ysize()))
    epix.bold()
    epix.dart(tail=Point(x=epix.xmin(), y=0), head=Point(x=epix.xmax() + delta, y=0))
    epix.dart(tail=Point(x=0, y=epix.ymin()), head=Point(x=0, y=epix.ymax() + delta))
    epix.font_size("scriptsize")
    epix.h_axis_masklabels(
        Point(x=epix.xmin() + 1, y=0),
        Point(x=epix.xmax(), y=0),
        n=int(epix.xsize()) - 1,
        offset=Point(x=0, y=2),
        align=epix.LabelPos.c,
    )
    epix.v_axis_labels(
        Point(x=0, y=epix.ymin()),
        Point(x=0, y=epix.ymax()),
        n=int(epix.ysize()),
        offset=Point(x=-2, y=0),
        align=epix.LabelPos.l,
    )
    epix.font_size("footnotesize")
    epix.label(
        Point(x=epix.xmax() + delta, y=0),
        Point(x=2, y=0),
        "Time (sec)",
        epix.LabelPos.r,
    )
    epix.label_angle(pi / 2)
    epix.label(Point(x=0, y=0), Point(x=-8, y=0), "Displacement (cm)", epix.LabelPos.l)
    epix.label_angle(0)
    L: epix.Legend = epix.Legend()
    L.item_border(0)
    epix.pen(epix.rgb(0.2, 0.7, 0.8))
    epix.plot(f1, epix.xmin(), epix.xmax(), n=120)
    L.path_item("Predicted")
    epix.set_red()
    L.mark_item(epix.MarkType.CIRC, "Measured")
    for i in range(1, samples):
        t: float = epix.xmin() + i * epix.xsize() / samples
        epix.v_error_bar(
            loc=Point(x=t, y=f2(t)), error=err(t), kind=epix.MarkType.CIRC, width=4
        )
    epix.set_black()
    L.draw(loc=epix.canvas().br(), offset=Point(x=-2, y=2), align=epix.LabelPos.tl)
    epix.font_face("sc")
    epix.label(
        Point(x=0.5 * (epix.xmin() + epix.xmax()), y=epix.ymax()),
        Point(x=0, y=2),
        "A damped harmonic oscillator",
        epix.LabelPos.t,
    )
fig
