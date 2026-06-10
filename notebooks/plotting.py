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
# # plotting — ported from `samples/plotting.xp`
# Graphing a function, with custom axis tick labels (radian mode).

# %%
import epix
from epix import P


def f(x):
    return 0.75 * epix.Sin(x) - 0.25 * epix.Sin(2 * x)


# %%
with epix.figure(P(-2, -1), P(2, 1), "5x1in") as fig:
    epix.h_axis(16)
    epix.v_axis(4)
    epix.h_axis_labels(4, P(0, -4), epix.LabelPos.b)
    epix.revolutions()
    epix.red()
    epix.plot(f, epix.xmin(), epix.xmax(), 120)
fig
