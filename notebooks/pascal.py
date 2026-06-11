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
# # pascal — ported from `samples/pascal.xp`
# Pascal's theorem: the three intersection points are collinear (`Segment` × `Segment`).

# %%
from __future__ import annotations

import epix
from epix import Point

# %%
with epix.figure(
    lower_left=Point(x=-2, y=-2), upper_right=Point(x=2, y=2), size="3 x 3in"
) as fig:
    epix.set_crop()
    v_dir: epix.Point = Point(x=2, y=-0.25)
    w_dir: epix.Point = Point(x=3, y=1)
    P1: epix.Point = Point(x=-1.5, y=-1)
    P2: epix.Point = P1 + v_dir
    P3: epix.Point = P1 + 1.5 * v_dir
    Q1: epix.Point = Point(x=-1, y=1)
    Q2: epix.Point = Q1 + 0.5 * w_dir
    Q3: epix.Point = Q1 + w_dir
    L12: epix.Segment = epix.Segment(tail=P1, head=Q2)
    L13: epix.Segment = epix.Segment(tail=P1, head=Q3)
    L21: epix.Segment = epix.Segment(tail=P2, head=Q1)
    L23: epix.Segment = epix.Segment(tail=P2, head=Q3)
    L31: epix.Segment = epix.Segment(tail=P3, head=Q1)
    L32: epix.Segment = epix.Segment(tail=P3, head=Q2)
    intersection_1: epix.Point = L12 * L21
    intersection_2: epix.Point = L13 * L31
    intersection_3: epix.Point = L32 * L23
    epix.dot(P1, Point(x=0, y=-2), "$P_1$", epix.LabelPos.b)
    epix.dot(P2, Point(x=0, y=-2), "$P_2$", epix.LabelPos.b)
    epix.dot(P3, Point(x=0, y=-2), "$P_3$", epix.LabelPos.b)
    epix.dot(Q1, Point(x=0, y=2), "$Q_1$", epix.LabelPos.t)
    epix.dot(Q2, Point(x=0, y=2), "$Q_2$", epix.LabelPos.t)
    epix.dot(Q3, Point(x=0, y=2), "$Q_3$", epix.LabelPos.t)
    epix.set_red()
    L12.draw()
    L21.draw()
    L13.draw()
    L31.draw()
    L32.draw()
    L23.draw()
    epix.set_green()
    epix.infinite_line(tail=P1, head=P3)
    epix.infinite_line(tail=Q1, head=Q3)
    epix.set_blue()
    epix.dot(intersection_1, Point(x=4, y=2), "$p_1$", epix.LabelPos.r)
    epix.dot(intersection_2, Point(x=4, y=2), "$p_2$", epix.LabelPos.r)
    epix.dot(intersection_3, Point(x=4, y=2), "$p_3$", epix.LabelPos.r)
    epix.dashed()
    epix.infinite_line(tail=intersection_1, head=intersection_3)
fig
