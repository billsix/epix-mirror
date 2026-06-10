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
import epix
from epix import P
from math import sqrt

tau = (1 + sqrt(5)) / 2

# %%
epix.bounding_box(P(0, 0), P(tau, 1))
epix.picture(100 * (1 + tau), 100 * tau)
epix.unitlength("0.0125in")
epix.begin()
epix.dashed()
epix.line(P(0.5, 0), P(0.5, 1))
epix.solid()
epix.line(P(1, 0), P(1, 1))
epix.line(P(0.5, 0), P(1, 1))
epix.arc(P(0.5, 0), tau - 0.5, 0, epix.Atan(2.0))
epix.bold()
epix.rect(P(0, 0), P(tau, 1))
fig = epix.render()
fig
