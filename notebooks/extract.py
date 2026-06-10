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
import math

import epix
from epix import P


# a benign function
def phi(x):
    return x * x


# parameters defining an arbitrary line ell
slope = -2
beta = 0.5

ht = phi(beta)
# alpha uses phi's numerical derivative at beta (ePiX `Deriv(phi).eval(beta)`)
alpha = slope - epix.deriv_eval(phi, beta)

eps = 0.02  # size of extraction


# a small perturbation to adjust the derivative
def f(x):
    return x / (1 + 16384 * x * x)


def gn(x):
    return f(alpha * (x - beta))


# phi's Evil Twin
def psi(x):
    return phi(x) + gn(x)


# inset parameters
mag_ctr = P(0.75, 0.2)
mag_sz = 0.125

# %%
with epix.figure(P(0, 0), P(1, 0.75), "4x3in") as fig:
    epix.set_crop()

    # two nearly-equal graphs, and the tangent to one of them
    epix.plain(epix.Blue())
    epix.plot(psi, epix.xmin(), beta - eps, 60)
    epix.plot(psi, beta + eps, epix.xmax(), 60)
    epix.plot(psi, beta - eps, beta + eps, 60)

    epix.pen(epix.Green(0.6))
    epix.Line(P(beta, phi(beta)), slope)

    epix.bold(epix.Red())
    epix.plot(phi, epix.xmin(), epix.xmax(), 60)
    epix.plain()

    # cut out a small neighborhood and magnify it
    mag = epix.canvas().extract_ellipse(
        P(beta - eps, psi(beta) - eps), P(beta + eps, psi(beta) + eps)
    )
    mag.backing(epix.Yellow(0.05))
    mag.border(epix.Black(), "0.4pt")

    epix.inset(mag, mag_ctr - mag_sz * P(1, 1), mag_ctr + mag_sz * P(1, 1))

    epix.pen(epix.Black())
    # axes
    epix.line(P(epix.xmin(), 0), P(epix.xmax(), 0))
    epix.line(P(0, epix.ymin()), P(0, epix.ymax()))

    # indicators
    epix.line(P(beta, 0), P(beta, phi(beta)), -20)  # shorten slightly
    epix.line(P(0, phi(beta)), P(beta, phi(beta)), -10)

    # zoom lines
    epix.line(P(beta, phi(beta)), mag_ctr + epix.polar(mag_sz, 2 * math.pi / 3), -20)
    epix.line(P(beta, phi(beta)), mag_ctr + epix.polar(mag_sz, 5 * (math.pi / 4)), -20)

    epix.font_size("footnotesize")
    epix.label(P(beta, 0), P(0, -4), r"$\beta$", epix.LabelPos.b)
    epix.label(P(0, phi(beta)), P(-4, 0), "$b$", epix.LabelPos.l)

    # use mag_ctr to place inset labels
    epix.red()
    epix.label(P(beta + 0.1, phi(beta + 0.1)), P(-2, 2), r"$\varphi$", epix.LabelPos.tl)
    epix.label(mag_ctr, P(0, 6), r"$\varphi$", epix.LabelPos.tr)

    epix.blue()
    epix.label(P(beta + 0.1, psi(beta + 0.1)), P(2, -2), r"$\psi$", epix.LabelPos.br)
    epix.label(mag_ctr, P(8, -2), r"$\psi$", epix.LabelPos.br)

    epix.green(0.6)
    epix.label(P(beta - 0.1, ht - 0.1 * slope), P(2, 2), r"$\ell$", epix.LabelPos.tr)
    epix.label(mag_ctr, P(-12, 20), r"$\ell$", epix.LabelPos.bl)

    epix.black()
    # a large label
    epix.label_border(epix.Blue(), "0.2pt")
    epix.masklabel(
        P(0.5 * (epix.xmin() + epix.xmax()), epix.ymax()),
        P(0, 0),
        r"\begin{minipage}{2.25in}A graph with unexpected tangent line\end{minipage}",
        epix.LabelPos.b,
    )
fig
