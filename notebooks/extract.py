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
# # extract — ported from `samples/extract.xp`
# A parabola `φ` and its "evil twin" `ψ` (φ plus a tiny localized bump) that look
# identical at this scale but have very different tangent lines at `β`. A small
# neighborhood is cut out with `canvas().extract_ellipse(...)` and magnified into
# an inset to reveal the difference.

# %%
from __future__ import annotations

import math

import epix
from epix import Point


# a benign function
def phi(x: float) -> float:
    return x * x


# parameters defining an arbitrary line ell
slope = -2
beta = 0.5

ht = phi(beta)
# alpha uses phi's numerical derivative at beta (ePiX `Deriv(phi).eval(beta)`)
alpha = slope - epix.deriv_eval(phi, beta)

eps = 0.02  # size of extraction


# a small perturbation to adjust the derivative
def f(x: float) -> float:
    return x / (1 + 16384 * x * x)


def gn(x: float) -> float:
    return f(alpha * (x - beta))


# phi's Evil Twin
def psi(x: float) -> float:
    return phi(x) + gn(x)


# inset parameters
mag_ctr = Point(x=0.75, y=0.2)
mag_sz = 0.125

# %%
with epix.figure(
    lower_left=Point(x=0, y=0), upper_right=Point(x=1, y=0.75), size="4x3in"
) as fig:
    epix.set_crop()

    # two nearly-equal graphs, and the tangent to one of them
    epix.plain(epix.blue())
    epix.plot(psi, epix.xmin(), beta - eps, n=60)
    epix.plot(psi, beta + eps, epix.xmax(), n=60)
    epix.plot(psi, beta - eps, beta + eps, n=60)

    epix.pen(epix.green(0.6))
    epix.infinite_line(Point(x=beta, y=phi(beta)), slope)

    epix.bold(epix.red())
    epix.plot(phi, epix.xmin(), epix.xmax(), n=60)
    epix.plain()

    # cut out a small neighborhood and magnify it
    mag = epix.canvas().extract_ellipse(
        lower_left=Point(x=beta - eps, y=psi(beta) - eps),
        upper_right=Point(x=beta + eps, y=psi(beta) + eps),
    )
    mag.backing(epix.yellow(0.05))
    mag.border(epix.black(), width="0.4pt")

    epix.inset(
        child=mag,
        lower_left=mag_ctr - mag_sz * Point(x=1, y=1),
        upper_right=mag_ctr + mag_sz * Point(x=1, y=1),
    )

    epix.pen(epix.black())
    # axes
    epix.line(tail=Point(x=epix.xmin(), y=0), head=Point(x=epix.xmax(), y=0))
    epix.line(tail=Point(x=0, y=epix.ymin()), head=Point(x=0, y=epix.ymax()))

    # indicators
    epix.line(
        tail=Point(x=beta, y=0), head=Point(x=beta, y=phi(beta)), expand=-20
    )  # shorten slightly
    epix.line(tail=Point(x=0, y=phi(beta)), head=Point(x=beta, y=phi(beta)), expand=-10)

    # zoom lines
    epix.line(
        tail=Point(x=beta, y=phi(beta)),
        head=mag_ctr + epix.polar(radius=mag_sz, theta=2 * math.pi / 3),
        expand=-20,
    )
    epix.line(
        tail=Point(x=beta, y=phi(beta)),
        head=mag_ctr + epix.polar(radius=mag_sz, theta=5 * (math.pi / 4)),
        expand=-20,
    )

    epix.font_size("footnotesize")
    epix.label(
        Point(x=beta, y=0),
        offset=Point(x=0, y=-4),
        text=r"$\beta$",
        align=epix.LabelPos.b,
    )
    epix.label(
        Point(x=0, y=phi(beta)),
        offset=Point(x=-4, y=0),
        text="$b$",
        align=epix.LabelPos.l,
    )

    # use mag_ctr to place inset labels
    epix.set_red()
    epix.label(
        Point(x=beta + 0.1, y=phi(beta + 0.1)),
        offset=Point(x=-2, y=2),
        text=r"$\varphi$",
        align=epix.LabelPos.tl,
    )
    epix.label(
        mag_ctr, offset=Point(x=0, y=6), text=r"$\varphi$", align=epix.LabelPos.tr
    )

    epix.set_blue()
    epix.label(
        Point(x=beta + 0.1, y=psi(beta + 0.1)),
        offset=Point(x=2, y=-2),
        text=r"$\psi$",
        align=epix.LabelPos.br,
    )
    epix.label(mag_ctr, offset=Point(x=8, y=-2), text=r"$\psi$", align=epix.LabelPos.br)

    epix.set_green(0.6)
    epix.label(
        Point(x=beta - 0.1, y=ht - 0.1 * slope),
        offset=Point(x=2, y=2),
        text=r"$\ell$",
        align=epix.LabelPos.tr,
    )
    epix.label(
        mag_ctr, offset=Point(x=-12, y=20), text=r"$\ell$", align=epix.LabelPos.bl
    )

    epix.set_black()
    # a large label
    epix.label_border(epix.blue(), "0.2pt")
    epix.masklabel(
        Point(x=0.5 * (epix.xmin() + epix.xmax()), y=epix.ymax()),
        offset=Point(x=0, y=0),
        text=r"\begin{minipage}{2.25in}A graph with unexpected tangent line\end{minipage}",
        align=epix.LabelPos.b,
    )
fig
