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
# # conic — ported from `samples/conic.xp`
#
# Projective equivalence of a parabola and a circle: the envelope of tangent
# lines, the line at infinity (the "horizon"), and a perspective camera that
# pulls back to show the closed-up curve. Faithful port — **byte-identical** to
# the C++ original.

# %%
import epix
from epix import P

MAX = 24
x0 = 4
ht = 3          # camera height
INFTY = 100000


def f(t):
    return P(t, t * t, 0)


# %%
with epix.figure(P(-3, -3), P(3, 3), "4x4in") as fig:
    epix.border()
    epix.set_crop()
    epix.viewpoint(0, -ht * ht, ht)
    epix.camera.look_at(P(0, 0, 1))

    epix.pen(epix.Black(0.3), "0.1pt")
    epix.grid(P(-MAX, 0), P(MAX, MAX * MAX), MAX, MAX * MAX // 8)
    epix.grid(P(-MAX, 0, 2 * ht), P(MAX, MAX * MAX, 2 * ht), MAX, MAX * MAX // 8)

    epix.pen(epix.Blue(1.4))
    epix.envelope(f, -MAX / 2, MAX / 2, 20)        # envelope of tangent lines
    epix.bbold(epix.Blue())
    epix.Line(P(-100, INFTY, 0), P(100, INFTY, 0))  # the horizon (line at infinity)

    epix.masklabel(P(0, 0), P(0, 2), "$(0,0)$", epix.LabelPos.t)
    epix.label(P(0, INFTY), P(0, -2), r"$(0,\infty)$", epix.LabelPos.b)
    epix.label(P(-0.25 * INFTY, INFTY), P(0, 2), "Horizon", epix.LabelPos.t)
    epix.masklabel(P(x0, x0 * x0), P(-4, 0), "$y=x^2$", epix.LabelPos.l)
    epix.box(P(0, 0))
    epix.box(P(0, INFTY))

    epix.pen(1)
    epix.plot(f, -MAX, MAX, 240)
    epix.camera.at(0, 0, 1000)                      # pull back to "close up" the curve
    epix.masklabel(P(0, epix.ymin()), P(0, 0),
                   "Projective equivalence of a parabola and a circle", epix.LabelPos.t)
fig
