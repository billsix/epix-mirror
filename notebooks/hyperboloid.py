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
# # hyperboloid — ported from `samples/hyperboloid.xp`
# A circular hyperboloid of one sheet (a `surface_rev` of the profile
# `(sinh, cosh)` about a tilted `frame` axis) and its two families of straight-
# line rulings, clipped to the front half.

# %%
import math

import epix
from epix import P

NUM_LINES = 64
dt = 1.0 / NUM_LINES

# %%
with epix.figure(P(-3, -3), P(3, 3), "6x6in") as fig:
    epix.revolutions()

    epix.cam().at(20, 0, 5)

    epix.clip_box(P(4, 4, 2))

    epix.fill(epix.Black(0.8))

    # hyperboloid
    epix.surface_rev(
        math.sinh,
        math.cosh,
        epix.domain(P(-2, 0), P(2, 1), epix.mesh(24, 36)),
        epix.frame(epix.E_3, epix.E_2, epix.E_1),
    )

    # clip to {x>0}
    epix.clip_box(P(0, -4, -2), P(4, 4, 2))

    # rulings
    for i in range(-NUM_LINES // 2, NUM_LINES // 2 + 1):
        t = i * dt
        epix.rgb(0, 0.5, 1)
        epix.Line(
            P(epix.Cos(t), epix.Sin(t), 0),
            P(epix.Cos(t) + epix.Sin(t), epix.Sin(t) - epix.Cos(t), 1),
        )

        epix.red()
        epix.Line(
            P(epix.Cos(t), epix.Sin(t), 0),
            P(epix.Cos(t) - epix.Sin(t), epix.Sin(t) + epix.Cos(t), 1),
        )

    # emphatic rulings
    epix.bbold(epix.RGB(0.1, 0.6, 0.9))
    epix.Line(P(1, 0, 0), P(1, -1, 1))

    epix.pen(epix.Red(1.2))
    epix.Line(P(1, 0, 0), P(1, 1, 1))
    epix.Line(P(0, 1, 0), P(-1, 1, 1))

    epix.clip_box()
    epix.black()
    epix.label(P(1, 0, 0) - P(0, -2, 2), P(2, -2), r"$\ell_0^-$", epix.LabelPos.br)
    epix.label(P(1, 0, 0) + P(0, 2, 2), P(18, 2), r"$\ell_0^+$", epix.LabelPos.t)
    epix.label(P(0, 1, 0) - P(-2, 0, 2), P(0, -4), r"$\ell_{\pi/2}^+$", epix.LabelPos.b)

    epix.label(
        P(0, 0, -3),
        P(0, -24),
        r"$\ell_\theta^-(t)=(\cos\theta, \sin\theta, 0) + t(\sin\theta, -\cos\theta, 1)$",
        epix.LabelPos.b,
    )

    epix.label(
        P(0, 0, -3),
        P(0, -12),
        r"$\ell_\theta^+(t)=(\cos\theta, \sin\theta, 0) + t(-\sin\theta, \cos\theta, 1)$",
        epix.LabelPos.b,
    )

    epix.pst_format()
fig
