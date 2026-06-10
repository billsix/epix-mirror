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
# # butterfly — ported from `samples/butterfly.xp`
# The cusp ("butterfly") catastrophe surface `Phi(u,v)` with the twisted-cubic
# cusp locus and its three coordinate-plane projections, drawn in a cropped 3-D
# scene with background grids and axes.

# %%
import math

import epix
from epix import P

VIEWPT = P(20, 0, 8)

tmin = 2 / math.sqrt(3)

light = epix.Blue(1.6)
dark = epix.Red(0.8)
lt_gray = epix.Black(0.2)
net_color = epix.Yellow(0.1)


def label_tag(loc, head, txt, align):
    epix.dart(loc, head)
    epix.masklabel(loc, P(0, 0), txt, align)


# surface
def Phi(u, v):
    return P(4 * v, 0.75 * (u * u - 2 * v) * u, 2 * u)


def cubic(t):
    return Phi(t, 1.5 * t * t)


# cubic projections to background planes
def Phi12(t):
    temp = cubic(t)
    return P(temp.x1(), temp.x2(), -8)


def Phi13(t):
    temp = cubic(t)
    return P(temp.x1(), -8, temp.x3())


def Phi23(t):
    temp = cubic(t)
    return P(-8, temp.x2(), temp.x3())


R = epix.domain(P(-2, -2), P(2, 2), epix.mesh(20, 20), epix.mesh(40, 40))
edge = epix.domain(P(-2, -2), P(2, 2), epix.mesh(1, 1), epix.mesh(40, 40))

# %%
with epix.figure(P(-14, -15), P(10, 9), "6x6in") as fig:
    epix.set_crop()
    epix.camera.at(20, 0, 8)

    # background grids
    epix.pen(lt_gray)
    epix.grid(P(-8, -8, -8), P(8, 8, -8), 8, 8)
    epix.grid(P(-8, -8, -8), P(8, -8, 6), 7, 8)  # n.b. (z,x) divisions
    epix.grid(P(-8, -8, -8), P(-8, 8, 6), 8, 7)

    # background coordinate axes
    epix.pen(epix.Black())
    epix.line(P(0, -8, -8), P(0, 8, -8))
    epix.line(P(-8, 0, -8), P(8, 0, -8))

    epix.line(P(0, -8, -8), P(0, -8, 6))
    epix.line(P(-8, -8, 0), P(8, -8, 0))

    epix.line(P(-8, 0, -8), P(-8, 0, 6))
    epix.line(P(-8, -8, 0), P(-8, 8, 0))

    # projections of cubic
    epix.bold(dark)
    epix.plot(Phi12, -tmin, tmin, 80)
    epix.plot(Phi13, -tmin, tmin, 80)
    epix.plot(Phi23, -tmin, tmin, 80)

    # surface
    epix.plain(net_color)
    epix.fill(lt_gray)
    epix.surface(Phi, R.resize1(-2, 0))  # bottom half

    # and cubic curve
    epix.bold(dark)
    epix.plot(cubic, -tmin, 0, 80)

    epix.plain(net_color)
    epix.surface(Phi, R.resize1(0, 2))  # top half

    epix.bold(dark)
    epix.plot(cubic, 0, tmin, 80)

    # surface boundary
    epix.bold(epix.Yellow())
    epix.plot(Phi, edge)

    # guide lines to cusp projection
    epix.plain(light)
    epix.dashed()
    epix.line(cubic(tmin), Phi12(tmin))
    epix.line(cubic(-tmin), Phi12(-tmin))

    # fake transparency
    epix.plain(dark)
    epix.plot(Phi12, -tmin, tmin, 20)  # cusp

    epix.label_color(epix.Black())
    epix.label(P(8, 0, -8), P(0, -4), "$x$", epix.LabelPos.b)
    epix.label(P(0, 8, -8), P(4, 0), "$y$", epix.LabelPos.r)

    epix.pen(light)
    epix.label_border(light, "0.4pt")
    epix.solid()
    label_tag(
        Phi(-2, -2) + P(0, 2, 6),
        Phi(-1.75, -1.75),
        r"$y=\frac{3}{32}(z^3-2xz)$",
        epix.LabelPos.t,
    )
    label_tag(
        cubic(-1) + P(0, 2, 3),
        cubic(-1),
        r"$(x,y,z)=(6t^2, -\frac{3}{2}t^3, 2t)$",
        epix.LabelPos.t,
    )

    epix.pst_format()
fig
