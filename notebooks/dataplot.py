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
import math

import epix
from epix import P


def F(x, y):
    return P((x - y) * (x + y), 2 * x * y)


# %%
with epix.figure(P(-1, 0), P(1, 20), "6x6in") as fig:
    # create a 2-column data file with 121 rows
    DF = epix.data_file(epix.Cos, epix.Sin, 0, math.pi, 120)

    L = epix.legend()
    epix.plain(epix.Black())
    epix.fill(epix.Blue(1.8))

    # act on first two columns; result doubles angles
    DF.transform(F)

    db = epix.data_bins(-1.05, 1.05, 21)  # bins centered on -1, -0.9, ..., 1
    db.read(DF.column(1))  # read col1
    db.bar_chart(db.pop())  # and plot, scaling to population

    L.fill_item("Population count")  # add a filled legend item

    epix.label_color(epix.Blue(1.2))
    epix.v_axis_labels(
        P(epix.xmin(), epix.ymin()),
        P(epix.xmin(), epix.ymax()),
        int(0.5 * epix.ysize()),  # C++ truncates the double 0.5*ysize() to unsigned
        P(-4, 0),
        epix.LabelPos.l,
    )

    # superimpose plot of raw data
    raw = epix.screen(P(-1, -1), P(1, 1))
    epix.activate(raw)
    epix.label_color(epix.Red())
    DF.plot(epix.MarkType.DDOT)  # scatter plot
    L.mark_item(epix.MarkType.DDOT, "Data points")

    epix.nofill()
    epix.plain(epix.Black(0.3))
    epix.grid(1, 10)

    epix.v_axis_labels(P(1, -1), P(1, 1), 4, P(4, 0), epix.LabelPos.r)

    epix.inset(P(-1, 0), P(1, 20))
    epix.deactivate(raw)  # polite but unnecessary

    L.draw(epix.canvas().c(), P(0, 0), epix.LabelPos.c)

    epix.pst_format()
fig
