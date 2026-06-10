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
# # uppersum — ported from `samples/uppersum.xp`
# Upper and lower Riemann sums of sin x.

# %%
import epix
from epix import P

N = 12  # number of rectangles


def f(t):
    return epix.Sin(t)


# %%
with epix.figure(P(0, 0), P(3, 1), "3x1in") as fig:
    epix.riemann_sum(f, epix.xmin(), epix.xmax(), N, epix.IntegralType.UPPER)
    epix.fill(epix.Black(0.1))
    epix.riemann_sum(f, epix.xmin(), epix.xmax(), N, epix.IntegralType.LOWER)
    epix.nofill()
    epix.h_axis(int(epix.xsize()))
    epix.v_axis(int(2 * epix.ysize()))
    epix.h_axis_labels(int(epix.xsize()), P(0, -4), epix.LabelPos.b)
    epix.label(P(2, f(2)), P(4, 2), r"$y=\sin x$", epix.LabelPos.tr)
    epix.bold(epix.Blue())
    epix.plot(f, epix.xmin(), epix.xmax(), 40)
fig
