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
import epix
from epix import P

theta = 60
rad = 0.25
Rad = 4.5
ht = 0.0625

# %%
epix.bounding_box(P(-5, -5), P(5, 5))
epix.picture(160, 160)
epix.unitlength("0.35mm")
epix.begin()
epix.degrees()
theta1 = epix.Asin(ht / rad)
theta2 = epix.Asin(ht / Rad)
epix.fill(epix.Black(0.1))
contour = epix.path(P(0, 0), Rad * epix.E_1, Rad * epix.E_2, theta2, 360 - theta2)
contour += epix.path(epix.polar(Rad, -theta2), epix.polar(rad, -theta1))
contour += epix.path(P(0, 0), rad * epix.E_1, rad * epix.E_2, 360 - theta1, theta1)
contour += epix.path(epix.polar(rad, theta1), epix.polar(Rad, theta2))
contour.close().fill().draw()
epix.dot(P(0, 0))
epix.arrow_width(2)
epix.arrow_inset(0.25)
epix.arrow(P(Rad / 4, 0.1 * Rad), P(3 * Rad / 4, 0.1 * Rad))
epix.arrow(P(3 * Rad / 4, -0.1 * Rad), P(Rad / 4, -0.1 * Rad))
epix.arc_arrow(P(0, 0), 0.9 * Rad, 180 - theta, 180 + theta)
epix.label(P(Rad, ht), P(2, 4), r"$R\to\infty$", epix.LabelPos.tr)
epix.label(P(0, rad), P(0, 4), r"$\delta\to0$", epix.LabelPos.tl)
epix.label(epix.polar(Rad, 45), P(0, 0), r"$\gamma$", epix.LabelPos.tr)
fig = epix.render()
fig
