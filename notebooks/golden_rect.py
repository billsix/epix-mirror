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
# # golden_rect — ported from `samples/golden_rect.xp`
# The golden rectangle. Uses `bounding_box` + `picture(w, h)` + `unitlength`
# (not the `figure()` context manager, which is for `picture(sw, ne, size)`).

# %%
from __future__ import annotations

from math import sqrt

import epix
from epix import Point

tau: float = (1 + sqrt(5)) / 2

# %%
epix.bounding_box(lower_left=Point(x=0, y=0), upper_right=Point(x=tau, y=1))
epix.picture(100 * (1 + tau), 100 * tau)
epix.unitlength("0.0125in")
epix.begin()
epix.dashed()
epix.line(tail=Point(x=0.5, y=0), head=Point(x=0.5, y=1))
epix.solid()
epix.line(tail=Point(x=1, y=0), head=Point(x=1, y=1))
epix.line(tail=Point(x=0.5, y=0), head=Point(x=1, y=1))
epix.arc(center=Point(x=0.5, y=0), radius=tau - 0.5, start=0, finish=epix.atan(2.0))
epix.bold()
epix.rect(lower_left=Point(x=0, y=0), upper_right=Point(x=tau, y=1))
fig: epix.Figure = epix.render()
fig
