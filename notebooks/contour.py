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
# # contour — ported from `samples/contour.xp`
# A keyhole contour for complex integration (built from arcs via the `path` class).

# %%
from __future__ import annotations

import epix
from epix import Point

theta: int = 60
rad: float = 0.25
Rad: float = 4.5
ht: float = 0.0625

# %%
epix.bounding_box(Point(x=-5, y=-5), Point(x=5, y=5))
epix.picture(160, 160)
epix.unitlength("0.35mm")
epix.begin()
epix.degrees()
theta1: float = epix.asin(ht / rad)
theta2: float = epix.asin(ht / Rad)
epix.fill(epix.black(0.1))
contour: epix.Path = epix.Path(
    Point(x=0, y=0), Rad * epix.E_1, Rad * epix.E_2, theta2, 360 - theta2
)
contour += epix.Path(
    epix.polar(radius=Rad, theta=-theta2), epix.polar(radius=rad, theta=-theta1)
)
contour += epix.Path(
    Point(x=0, y=0), rad * epix.E_1, rad * epix.E_2, 360 - theta1, theta1
)
contour += epix.Path(
    epix.polar(radius=rad, theta=theta1), epix.polar(radius=Rad, theta=theta2)
)
contour.close().fill().draw()
epix.dot(Point(x=0, y=0))
epix.arrow_width(width=2)
epix.arrow_inset(0.25)
epix.arrow(tail=Point(x=Rad / 4, y=0.1 * Rad), head=Point(x=3 * Rad / 4, y=0.1 * Rad))
epix.arrow(tail=Point(x=3 * Rad / 4, y=-0.1 * Rad), head=Point(x=Rad / 4, y=-0.1 * Rad))
epix.arc_arrow(
    center=Point(x=0, y=0), radius=0.9 * Rad, start=180 - theta, finish=180 + theta
)
epix.label(
    Point(x=Rad, y=ht),
    offset=Point(x=2, y=4),
    text=r"$R\to\infty$",
    align=epix.LabelPos.tr,
)
epix.label(
    Point(x=0, y=rad),
    offset=Point(x=0, y=4),
    text=r"$\delta\to0$",
    align=epix.LabelPos.tl,
)
epix.label(
    epix.polar(radius=Rad, theta=45),
    offset=Point(x=0, y=0),
    text=r"$\gamma$",
    align=epix.LabelPos.tr,
)
fig: epix.Figure = epix.render()
fig
