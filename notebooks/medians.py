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
# # medians — ported from `samples/medians.xp`
# A triangle's medians, sides, and altitudes, with the orthocenter circle.

# %%
import epix
from epix import P

pt1 = P(2, -1)
pt2 = P(1, 2)
pt3 = P(-2, 0)


def drop_perp(arg, tail, head):
    return arg - ((arg - tail) % (head - tail))  # % = orthogonalization


q1 = drop_perp(pt1, pt2, pt3)
q2 = drop_perp(pt2, pt3, pt1)
q3 = drop_perp(pt3, pt1, pt2)


def rangle(arg, tail, head):
    direction = head - tail
    loc = arg - ((arg - tail) % direction)
    epix.right_angle(loc, direction, epix.J(direction))


# %%
with epix.figure(P(-3, -3), P(3, 3), "4x4in") as fig:
    epix.set_crop()
    side1 = epix.Segment(pt2, pt3)
    side2 = epix.Segment(pt3, pt1)
    side3 = epix.Segment(pt1, pt2)
    med1 = epix.Segment(pt1, side1.midpoint())
    med2 = epix.Segment(pt2, side2.midpoint())
    med3 = epix.Segment(pt3, side3.midpoint())
    alt1 = epix.Segment(pt1, q1)
    alt2 = epix.Segment(pt2, q2)
    alt3 = epix.Segment(pt3, q3)
    med1.draw()
    med2.draw()
    med3.draw()
    epix.bold()
    side1.draw()
    side2.draw()
    side3.draw()
    epix.plain()
    epix.blue(1.4)
    alt1.draw()
    alt2.draw()
    alt3.draw()
    rangle(q1, pt2, pt3)
    rangle(q2, pt3, pt1)
    rangle(q3, pt1, pt2)
    epix.circle(q1, q2, q3)
    epix.label(q1, P(-2, 4), "$a_1$", epix.LabelPos.t)
    epix.label(q2, P(-2, -4), "$a_2$", epix.LabelPos.b)
    epix.label(q3, P(4, 0), "$a_3$", epix.LabelPos.r)
    epix.black()
    epix.font_size("footnotesize")
    epix.label(pt1, P(2, -2), "$p_1$", epix.LabelPos.br)
    epix.label(pt2, P(2, 2), "$p_2$", epix.LabelPos.tr)
    epix.label(pt3, P(-4, -2), "$p_3$", epix.LabelPos.l)
    epix.label(side1 * med1, P(-4, 2), "$m_1$", epix.LabelPos.tl)
    epix.label(side2 * med2, P(-2, -4), "$m_2$", epix.LabelPos.b)
    epix.label(side3 * med3, P(4, 0), "$m_3$", epix.LabelPos.r)
fig
