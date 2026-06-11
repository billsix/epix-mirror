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
# # dataplot — ported from `samples/dataplot.xp`
# Builds a two-column `data_file` by sampling `(cos, sin)` over a half-turn,
# transforms it by the angle-doubling map `F`, then bins the first column into a
# `data_bins` bar chart — with the raw transformed points scatter-plotted in an
# inset and a `legend`.

# %%
from __future__ import annotations

import math

import epix
from epix import Point


def F(x: float, y: float) -> epix.Point:
    return Point(x=(x - y) * (x + y), y=2 * x * y)


# %%
with epix.figure(
    lower_left=Point(x=-1, y=0), upper_right=Point(x=1, y=20), size="6x6in"
) as fig:
    # create a 2-column data file with 121 rows
    data: epix.DataFile = epix.DataFile(epix.cos, epix.sin, 0, math.pi, 120)

    L: epix.Legend = epix.Legend()
    epix.plain(epix.black())
    epix.fill(epix.blue(1.8))

    # act on first two columns; result doubles angles
    data.transform(F)

    bins: epix.DataBins = epix.DataBins(
        low=-1.05, high=1.05, n=21
    )  # bins centered on -1, -0.9, ..., 1
    bins.read(data.column(1))  # read col1
    bins.bar_chart(bins.pop())  # and plot, scaling to population

    L.fill_item("Population count")  # add a filled legend item

    epix.label_color(epix.blue(1.2))
    epix.v_axis_labels(
        Point(x=epix.xmin(), y=epix.ymin()),
        Point(x=epix.xmin(), y=epix.ymax()),
        n=int(0.5 * epix.ysize()),  # C++ truncates the double 0.5*ysize() to unsigned
        offset=Point(x=-4, y=0),
        align=epix.LabelPos.l,
    )

    # superimpose plot of raw data
    raw: epix.Screen = epix.Screen(
        lower_left=Point(x=-1, y=-1), upper_right=Point(x=1, y=1)
    )
    epix.activate(raw)
    epix.label_color(epix.red())
    data.plot(epix.MarkType.DDOT)  # scatter plot
    L.mark_item(epix.MarkType.DDOT, "Data points")

    epix.nofill()
    epix.plain(epix.black(0.3))
    epix.grid(nx=1, ny=10)

    epix.v_axis_labels(
        Point(x=1, y=-1),
        Point(x=1, y=1),
        n=4,
        offset=Point(x=4, y=0),
        align=epix.LabelPos.r,
    )

    epix.inset(lower_left=Point(x=-1, y=0), upper_right=Point(x=1, y=20))
    epix.deactivate(raw)  # polite but unnecessary

    L.draw(loc=epix.canvas().c(), offset=Point(x=0, y=0), align=epix.LabelPos.c)

    epix.pst_format()
fig
