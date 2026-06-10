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
# # lissajous — ported from `samples/lissajous.xp`
# A Lissajous curve and its tangent field.

# %%
import epix
from epix import P


def f(t):
    return P(epix.Sin(3 * t), epix.Sin(4 * t))


# %%
with epix.figure(P(-1, -1), P(1, 1), "2 x 2in") as fig:
    epix.revolutions()
    epix.bold(epix.Red())
    epix.plot(f, 0, 1, 200)
    epix.arrow_inset(0.2)
    epix.arrow_width(1.5)
    epix.plain(epix.Blue())
    epix.tan_field(f, 0, 1, 81)
fig
