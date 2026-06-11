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
from __future__ import annotations

import epix
from epix import Point

pt1: epix.Point = Point(x=2, y=-1)
pt2: epix.Point = Point(x=1, y=2)
pt3: epix.Point = Point(x=-2, y=0)


def drop_perp(arg: epix.Point, tail: epix.Point, head: epix.Point) -> epix.Point:
    return arg - ((arg - tail) % (head - tail))  # % = orthogonalization


altitude_foot_1: epix.Point = drop_perp(pt1, pt2, pt3)
altitude_foot_2: epix.Point = drop_perp(pt2, pt3, pt1)
altitude_foot_3: epix.Point = drop_perp(pt3, pt1, pt2)


def rangle(arg: epix.Point, tail: epix.Point, head: epix.Point) -> None:
    direction: epix.Point = head - tail
    loc: epix.Point = arg - ((arg - tail) % direction)
    epix.right_angle(loc=loc, leg1=direction, leg2=epix.quarter_turn(direction))


# %%
with epix.figure(
    lower_left=Point(x=-3, y=-3), upper_right=Point(x=3, y=3), size="4x4in"
) as fig:
    epix.set_crop()
    side1: epix.Segment = epix.Segment(tail=pt2, head=pt3)
    side2: epix.Segment = epix.Segment(tail=pt3, head=pt1)
    side3: epix.Segment = epix.Segment(tail=pt1, head=pt2)
    med1: epix.Segment = epix.Segment(tail=pt1, head=side1.midpoint())
    med2: epix.Segment = epix.Segment(tail=pt2, head=side2.midpoint())
    med3: epix.Segment = epix.Segment(tail=pt3, head=side3.midpoint())
    alt1: epix.Segment = epix.Segment(tail=pt1, head=altitude_foot_1)
    alt2: epix.Segment = epix.Segment(tail=pt2, head=altitude_foot_2)
    alt3: epix.Segment = epix.Segment(tail=pt3, head=altitude_foot_3)
    med1.draw()
    med2.draw()
    med3.draw()
    epix.bold()
    side1.draw()
    side2.draw()
    side3.draw()
    epix.plain()
    epix.set_blue(1.4)
    alt1.draw()
    alt2.draw()
    alt3.draw()
    rangle(altitude_foot_1, pt2, pt3)
    rangle(altitude_foot_2, pt3, pt1)
    rangle(altitude_foot_3, pt1, pt2)
    epix.circle(altitude_foot_1, altitude_foot_2, altitude_foot_3)
    epix.label(altitude_foot_1, Point(x=-2, y=4), "$a_1$", epix.LabelPos.t)
    epix.label(altitude_foot_2, Point(x=-2, y=-4), "$a_2$", epix.LabelPos.b)
    epix.label(altitude_foot_3, Point(x=4, y=0), "$a_3$", epix.LabelPos.r)
    epix.set_black()
    epix.font_size("footnotesize")
    epix.label(pt1, Point(x=2, y=-2), "$p_1$", epix.LabelPos.br)
    epix.label(pt2, Point(x=2, y=2), "$p_2$", epix.LabelPos.tr)
    epix.label(pt3, Point(x=-4, y=-2), "$p_3$", epix.LabelPos.l)
    epix.label(side1 * med1, Point(x=-4, y=2), "$m_1$", epix.LabelPos.tl)
    epix.label(side2 * med2, Point(x=-2, y=-4), "$m_2$", epix.LabelPos.b)
    epix.label(side3 * med3, Point(x=4, y=0), "$m_3$", epix.LabelPos.r)
fig
