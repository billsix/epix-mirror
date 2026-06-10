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
from math import exp, pi, sqrt

import epix
from epix import P

mult = 1.0 / sqrt(pi)


def f(x):
    return 0.4 * exp(-mult * x * x)


def g(x):
    return f(x - 3)


PAD_X = 0.4
PAD_Y = 0.01

# %%
with epix.figure(P(-3 - PAD_X, -PAD_Y), P(8 + PAD_X, 0.4 + PAD_Y), "5 x 3.5in") as fig:
    epix.border()
    L = epix.legend()
    L.border(0)
    epix.fill(epix.Black(0.2))
    epix.shadeplot(g, 1.96, epix.xmax(), 120)
    L.fill_item("$P(|Z|>1.96, H_1) = 0.85$")
    epix.fill(epix.Black(0.8))
    epix.shadeplot(f, epix.xmin(), -1.96, 20)
    epix.shadeplot(f, 1.96, 4, 40)
    L.fill_item("$P(|Z|>1.96, H_0) = 0.05$")
    epix.plot(f, epix.xmin(), epix.xmax(), 240)
    epix.plot(g, epix.xmin(), epix.xmax(), 240)
    epix.font_size("scriptsize")
    L.draw(P(4, 0.35), P(0, 0), epix.LabelPos.tr)
    epix.label(P(0, 0.15), P(0, 2), r"$H_0$: $\mu_1=\mu_2$", epix.LabelPos.t)
    epix.label(P(2, 0.15), P(2, 2), r"$H_1$: $\mu_1=\mu_2+\delta$", epix.LabelPos.tr)
    epix.font_size("footnotesize")
    Ax = epix.axis(P(-2, epix.ymin()), P(8, epix.ymin()), 5, P(0, -8), epix.LabelPos.b)
    Ax.align(epix.LabelPos.b).draw()
    epix.label(
        P(2, epix.ymin()),
        P(0, -18),
        r"$Z=\displaystyle\frac{\mu_1-\mu_2}{\sigma/\sqrt{\pi}}$",
        epix.LabelPos.b,
    )
    epix.degrees()
    epix.label_angle(90)
    Ay = epix.axis(P(epix.xmin(), 0), P(epix.xmin(), 0.4), 4, P(-8, 0), epix.LabelPos.l)
    Ay.align(epix.LabelPos.l).draw()
    epix.label(P(epix.xmin(), 0.2), P(-18, 0), "Density", epix.LabelPos.l)
fig
