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
# # twisted_cubic — ported from `samples/twisted_cubic.xp`
# The twisted cubic with its three coordinate-plane projections, as a stereo pair.

# %%
import epix
from epix import P

MAX = 2.5
XMIN = -2


def cubic(t):
    return t * P(1, t, t * t)


def cubic12(t):
    a = cubic(t)
    return P(a.x1(), a.x2(), XMIN)


def cubic13(t):
    a = cubic(t)
    return P(a.x1(), XMIN, a.x3())


def cubic23(t):
    a = cubic(t)
    return P(XMIN, a.x2(), a.x3())


def bars(f1, f2):
    for i in range(21):
        epix.line(f1(-1 + 0.1 * i), f2(-1 + 0.1 * i))


def draw_frame():
    epix.blue()
    epix.bbold()
    epix.plot(cubic13, -1, 1, 60)
    epix.plain()
    bars(cubic13, cubic)
    epix.label(cubic13(1), P(0, 2), "$x$-$z$", epix.LabelPos.t)
    epix.green(0.6)
    epix.bbold()
    epix.plot(cubic23, -1, 1, 60)
    epix.plain()
    bars(cubic23, cubic)
    epix.label(cubic23(1), P(0, 2), "$y$-$z$", epix.LabelPos.t)
    epix.magenta()
    epix.bbold()
    epix.plot(cubic12, -1, 1, 60)
    epix.plain()
    bars(cubic12, cubic)
    epix.label(cubic12(1), P(2, 0), "$x$-$y$", epix.LabelPos.r)
    epix.bold(epix.Red(1.6))
    epix.base(epix.Red(0.6), "2pt")
    epix.plot(cubic, -1, 1, 60)


# %%
with epix.figure(P(-1, 0), P(1.25, 1), "4.5 x 2in") as fig:
    scr_left = epix.screen(P(-MAX, -MAX), P(MAX, MAX))
    epix.activate(scr_left)
    epix.border(epix.Black(), "1pt")
    epix.camera.at(epix.sph(10, 0.8, 0.5))
    epix.camera.look_at(P(0, 0, -0.25))
    draw_frame()
    epix.base(epix.Neutral())
    scr_right = epix.screen(P(-MAX, -MAX), P(MAX, MAX))
    epix.activate(scr_right)
    epix.border(epix.Black(), "1pt")
    epix.camera.at(epix.sph(10, 0.9, 0.5))
    epix.camera.look_at(P(0, 0, -0.25))
    draw_frame()
    epix.inset(scr_left, P(0.25, 0), P(1.25, 1))
    epix.inset(scr_right, P(-1, 0), P(0, 1))
fig
