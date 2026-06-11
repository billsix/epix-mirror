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
from __future__ import annotations

import epix
from epix import Point

MAX: int = 24
x0: int = 4
ht: int = 3  # camera height
INFTY: int = 100000


def f(t: float) -> epix.Point:
    return Point(x=t, y=t * t, z=0)


# %%
with epix.figure(
    lower_left=Point(x=-3, y=-3), upper_right=Point(x=3, y=3), size="4x4in"
) as fig:
    epix.border()
    epix.set_crop()
    epix.viewpoint(0, -ht * ht, ht)
    epix.camera.look_at(Point(x=0, y=0, z=1))

    epix.pen(epix.black(0.3), width="0.1pt")
    epix.grid(
        lower_left=Point(x=-MAX, y=0),
        upper_right=Point(x=MAX, y=MAX * MAX),
        nx=MAX,
        ny=MAX * MAX // 8,
    )
    epix.grid(
        lower_left=Point(x=-MAX, y=0, z=2 * ht),
        upper_right=Point(x=MAX, y=MAX * MAX, z=2 * ht),
        nx=MAX,
        ny=MAX * MAX // 8,
    )

    epix.pen(epix.blue(1.4))
    epix.envelope(f, -MAX / 2, MAX / 2, n=20)  # envelope of tangent lines
    epix.bbold(epix.blue())
    epix.infinite_line(
        tail=Point(x=-100, y=INFTY, z=0), head=Point(x=100, y=INFTY, z=0)
    )  # the horizon (line at infinity)

    epix.masklabel(
        Point(x=0, y=0), offset=Point(x=0, y=2), text="$(0,0)$", align=epix.LabelPos.t
    )
    epix.label(
        Point(x=0, y=INFTY),
        offset=Point(x=0, y=-2),
        text=r"$(0,\infty)$",
        align=epix.LabelPos.b,
    )
    epix.label(
        Point(x=-0.25 * INFTY, y=INFTY),
        offset=Point(x=0, y=2),
        text="Horizon",
        align=epix.LabelPos.t,
    )
    epix.masklabel(
        Point(x=x0, y=x0 * x0),
        offset=Point(x=-4, y=0),
        text="$y=x^2$",
        align=epix.LabelPos.l,
    )
    epix.box(Point(x=0, y=0))
    epix.box(Point(x=0, y=INFTY))

    epix.pen(1)
    epix.plot(f, -MAX, MAX, n=240)
    epix.camera.at(0, 0, 1000)  # pull back to "close up" the curve
    epix.masklabel(
        Point(x=0, y=epix.ymin()),
        offset=Point(x=0, y=0),
        text="Projective equivalence of a parabola and a circle",
        align=epix.LabelPos.t,
    )
fig
