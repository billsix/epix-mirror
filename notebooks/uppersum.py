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
from __future__ import annotations

import epix
from epix import Point

N = 12  # number of rectangles


def f(t: float) -> float:
    return epix.sin(t)


# %%
with epix.figure(
    lower_left=Point(x=0, y=0), upper_right=Point(x=3, y=1), size="3x1in"
) as fig:
    epix.riemann_sum(f, a=epix.xmin(), b=epix.xmax(), n=N, type=epix.IntegralType.UPPER)
    epix.fill(epix.black(0.1))
    epix.riemann_sum(f, a=epix.xmin(), b=epix.xmax(), n=N, type=epix.IntegralType.LOWER)
    epix.nofill()
    epix.h_axis(int(epix.xsize()))
    epix.v_axis(int(2 * epix.ysize()))
    epix.h_axis_labels(
        int(epix.xsize()), offset=Point(x=0, y=-4), align=epix.LabelPos.b
    )
    epix.label(
        Point(x=2, y=f(2)),
        offset=Point(x=4, y=2),
        text=r"$y=\sin x$",
        align=epix.LabelPos.tr,
    )
    epix.bold(epix.blue())
    epix.plot(f, epix.xmin(), epix.xmax(), n=40)
fig
