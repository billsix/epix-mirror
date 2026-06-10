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
# # geomsum — ported from `samples/geomsum.xp`
# The geometric series 1/2 + 1/4 + 1/8 + … filling the unit square.

# %%
import epix
from epix import P

N = 8

# %%
with epix.figure(P(0, 0), P(1, 1), "2.5x2.5in") as fig:
    epix.pen(epix.White(), "0.8pt")
    epix.fill(epix.White(0.9))
    epix.rect(P(0, 0), P(1, 1))
    epix.black()
    epix.label(P(1.0 / 4, 1.0 / 2), r"$\frac{1}{2}$")
    epix.label(P(5.0 / 8, 3.0 / 4), r"$\frac{1}{8}$")
    epix.label(P(13.0 / 16, 7.0 / 8), r"$\frac{1}{32}$")
    epix.white()
    epix.fill(epix.Blue())
    t = 0.5
    for _ in range(N):
        epix.rect(P(1 - t, 1 - 2 * t), P(1, 1 - t))
        epix.line(P(1 - t, 1 - 2 * t), P(1 - t, 1))
        t *= 0.5
    epix.label(P(3.0 / 4, 1.0 / 4), r"$\mathbf{\frac{1}{4}}$")
    epix.label(P(7.0 / 8, 5.0 / 8), r"$\mathbf{\frac{1}{16}}$")
    epix.label(P(15.0 / 16, 13.0 / 16), r"$\mathbf{\frac{1}{64}}$")
fig
