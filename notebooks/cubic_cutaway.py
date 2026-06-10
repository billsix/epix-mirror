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
# # cubic_cutaway — ported from `samples/cubic_cutaway.xp`
# The three branches of the cube root `z^3 = 1 − w²` over the complex `w`-plane,
# assembled as one colored `scenery` and drawn twice (lower-back then upper-front)
# with `clip_box` for manual hidden-surface removal, plus the real cross-sections.

# %%
import math

import epix
from epix import P

ht = 2  # truncation height
sz = 2  # domain size
sq = 12  # domain mesh
eps = 0.0000001


def poly(x, y):
    return epix.Complex(1 - x, -y) * epix.Complex(1 + x, y)  # 1 - (x + iy)^2


# z^3 = 1 - w^2, w = x + iy -- three branches
def f0(x, y):
    return epix.rootC(poly(x, y), 3) + x * epix.E_3


def f1(x, y):
    return epix.rootC(poly(x, y), 3, 1) + x * epix.E_3


def f2(x, y):
    return epix.rootC(poly(x, y), 3, 2) + x * epix.E_3


# real sections
def fp(t):
    return P(t, 0, math.sqrt(1 - t * t * t))


def fm(t):
    return P(t, 0, -math.sqrt(1 - t * t * t))


# coloring
def dens(d):
    return 0.5 * (1 + math.tanh(d))


def color(u, v):
    return P(dens(v), 0.2, 1 - dens(v))


# pen styles; called multiple times, so factored out
def real_section():
    epix.bbold(epix.Magenta(0.6))
    epix.base(epix.RGB(0.8, 0, 1), 2.5)


def mesh_pen():
    epix.plain(epix.RGB(1, 0.8, 0.5))  # surface mesh


def grid_pen():
    epix.plain(epix.Black(0.4))  # coordinate grid


def axis_pen():
    epix.bold(epix.Black())  # axes


# lower and upper half-planes
LHP = epix.domain(
    P(-sz, -sz), P(sz, -eps), epix.mesh(2 * sq, sq), epix.mesh(10 * sq, 5 * sq)
)
UHP = epix.domain(
    P(-sz, eps), P(sz, sz), epix.mesh(2 * sq, sq), epix.mesh(10 * sq, 5 * sq)
)

# %%
with epix.figure(P(-2, -2), P(2, 2), "6 x 6in") as fig:
    epix.camera.at(P(4, 8, 4)).look_at(P(0.5, 0, 0.5))

    epix.fill()
    mesh_pen()

    # three branches of cube root over each half-plane
    sc = epix.scenery(f0, LHP, color)
    sc.add(f1, LHP, color)
    sc.add(f2, LHP, color)

    sc.add(f0, UHP, color)
    sc.add(f1, UHP, color)
    sc.add(f2, UHP, color)

    # Manual object hiding: draw lower back portion
    epix.clip_box(P(-2, -2, -ht), P(2, 0, 0))
    sc.draw()

    # lower portion of real section
    real_section()
    epix.plot(fm, epix.xmin(), 1, 240)

    epix.base(epix.Neutral(), 0)

    # Re w axis and z-grid
    epix.clip_box()
    epix.nofill()
    axis_pen()
    epix.line(P(0, 0, -sz), P(0, 0, 0))

    grid_pen()
    epix.grid(P(-2, -2), P(2, 2), 24, 24)

    # Im z axis
    axis_pen()
    epix.arrow(P(0, -1.1 * sz, 0), P(0, 1.1 * sz, 0))

    # upper back portion
    epix.fill()
    epix.clip_box(P(-2, -2, 0), P(2, 0, ht))
    sc.draw()

    # upper portion of real section
    real_section()
    epix.plot(fp, epix.xmin(), 1, 240)

    epix.base(epix.Neutral(), 0)

    # remaining axes, labels
    epix.clip_box()
    axis_pen()
    epix.arrow(P(-1.1 * sz, 0, 0), P(1.1 * sz, 0, 0))
    epix.arrow(P(0, 0, 0), P(0, 0, sz))

    epix.label(P(1.1 * sz, 0), P(-2, -4), r"$\operatorname{Re} z$", epix.LabelPos.l)
    epix.label(P(0, 1.1 * sz), P(2, -2), r"$\operatorname{Im} z$", epix.LabelPos.br)
    epix.label(
        P(0, 0, sz), P(0, 4), r"$\operatorname{Re} \sqrt{1-z^3}$", epix.LabelPos.t
    )

    epix.pst_format()
fig
