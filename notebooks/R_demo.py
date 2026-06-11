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
# # R_demo — ported from `samples/R_demo.xp`
# A hypothesis-test figure: two density curves with shaded tail regions.

# %%
from __future__ import annotations

from math import exp, pi, sqrt

import epix
from epix import Point

mult: float = 1.0 / sqrt(pi)


def f(x: float) -> float:
    return 0.4 * exp(-mult * x * x)


def g(x: float) -> float:
    return f(x - 3)


PAD_X: float = 0.4
PAD_Y: float = 0.01

# %%
with epix.figure(
    lower_left=Point(x=-3 - PAD_X, y=-PAD_Y),
    upper_right=Point(x=8 + PAD_X, y=0.4 + PAD_Y),
    size="5 x 3.5in",
) as fig:
    epix.border()
    L: epix.Legend = epix.Legend()
    L.border(0)
    epix.fill(epix.black(0.2))
    epix.shadeplot(g, 1.96, epix.xmax(), n=120)
    L.fill_item("$P(|Z|>1.96, H_1) = 0.85$")
    epix.fill(epix.black(0.8))
    epix.shadeplot(f, epix.xmin(), -1.96, n=20)
    epix.shadeplot(f, 1.96, 4, n=40)
    L.fill_item("$P(|Z|>1.96, H_0) = 0.05$")
    epix.plot(f, epix.xmin(), epix.xmax(), n=240)
    epix.plot(g, epix.xmin(), epix.xmax(), n=240)
    epix.font_size("scriptsize")
    L.draw(loc=Point(x=4, y=0.35), offset=Point(x=0, y=0), align=epix.LabelPos.tr)
    epix.label(
        Point(x=0, y=0.15),
        offset=Point(x=0, y=2),
        text=r"$H_0$: $\mu_1=\mu_2$",
        align=epix.LabelPos.t,
    )
    epix.label(
        Point(x=2, y=0.15),
        offset=Point(x=2, y=2),
        text=r"$H_1$: $\mu_1=\mu_2+\delta$",
        align=epix.LabelPos.tr,
    )
    epix.font_size("footnotesize")
    Ax: epix.Axis = epix.Axis(
        Point(x=-2, y=epix.ymin()),
        Point(x=8, y=epix.ymin()),
        n=5,
        offset=Point(x=0, y=-8),
        align=epix.LabelPos.b,
    )
    Ax.align(epix.LabelPos.b).draw()
    epix.label(
        Point(x=2, y=epix.ymin()),
        offset=Point(x=0, y=-18),
        text=r"$Z=\displaystyle\frac{\mu_1-\mu_2}{\sigma/\sqrt{\pi}}$",
        align=epix.LabelPos.b,
    )
    epix.degrees()
    epix.label_angle(90)
    Ay: epix.Axis = epix.Axis(
        Point(x=epix.xmin(), y=0),
        Point(x=epix.xmin(), y=0.4),
        n=4,
        offset=Point(x=-8, y=0),
        align=epix.LabelPos.l,
    )
    Ay.align(epix.LabelPos.l).draw()
    epix.label(
        Point(x=epix.xmin(), y=0.2),
        offset=Point(x=-18, y=0),
        text="Density",
        align=epix.LabelPos.l,
    )
fig
