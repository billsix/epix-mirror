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
import epix
from epix import P

# %%
with epix.figure(P(-2, -2), P(2, 2), "3 x 3in") as fig:
    epix.set_crop()
    V = P(2, -0.25)
    W = P(3, 1)
    P1 = P(-1.5, -1)
    P2 = P1 + V
    P3 = P1 + 1.5 * V
    Q1 = P(-1, 1)
    Q2 = Q1 + 0.5 * W
    Q3 = Q1 + W
    L12 = epix.Segment(P1, Q2)
    L13 = epix.Segment(P1, Q3)
    L21 = epix.Segment(P2, Q1)
    L23 = epix.Segment(P2, Q3)
    L31 = epix.Segment(P3, Q1)
    L32 = epix.Segment(P3, Q2)
    R1 = L12 * L21
    R2 = L13 * L31
    R3 = L32 * L23
    epix.dot(P1, P(0, -2), "$P_1$", epix.LabelPos.b)
    epix.dot(P2, P(0, -2), "$P_2$", epix.LabelPos.b)
    epix.dot(P3, P(0, -2), "$P_3$", epix.LabelPos.b)
    epix.dot(Q1, P(0, 2), "$Q_1$", epix.LabelPos.t)
    epix.dot(Q2, P(0, 2), "$Q_2$", epix.LabelPos.t)
    epix.dot(Q3, P(0, 2), "$Q_3$", epix.LabelPos.t)
    epix.red()
    L12.draw()
    L21.draw()
    L13.draw()
    L31.draw()
    L32.draw()
    L23.draw()
    epix.green()
    epix.Line(P1, P3)
    epix.Line(Q1, Q3)
    epix.blue()
    epix.dot(R1, P(4, 2), "$p_1$", epix.LabelPos.r)
    epix.dot(R2, P(4, 2), "$p_2$", epix.LabelPos.r)
    epix.dot(R3, P(4, 2), "$p_3$", epix.LabelPos.r)
    epix.dashed()
    epix.Line(R1, R3)
fig
