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
# # shadeplot — ported from `samples/shadeplot.xp`
# A Gaussian bump with three shaded regions illustrating the fundamental theorem
# of calculus ($F(x+dx)-F(x) \approx f(x)\,dx$), captioned by a `legend`.

# %%
from __future__ import annotations

import math

import epix
from epix import Point

k = 4  # change width of hump
dx = 0.05  # width of thin shaded region
x = 1 / math.sqrt(k)  # position of thin shaded region


def f(t: float) -> float:
    return math.sqrt(abs(k) / (2 * math.pi)) * math.exp(-k * t * t)


shade1: epix.Color = epix.black(0.1)
shade2: epix.Color = epix.black(0.4)
shade3: epix.Color = epix.black(0.6)

# %%
with epix.figure(
    lower_left=Point(x=0, y=0), upper_right=Point(x=1, y=1), size="150x150pt"
) as fig:
    L: epix.Legend = epix.Legend()
    L.backing(epix.neutral())

    epix.fill(shade1)
    epix.shadeplot(f, epix.xmin(), x, n=90)
    L.fill_item(r"$=\displaystyle\int_a^x f(t)\,dt$")

    epix.fill(shade2)
    epix.rect(lower_left=Point(x=x, y=0), upper_right=Point(x=x + dx, y=f(x)))
    L.fill_item(r"= $f(x)\,dx\vphantom{\Bigg|}$")

    epix.fill(shade3)
    epix.shadeplot(f, x, x + dx, n=10)
    L.fill_item("= $F(x+dx)-F(x)$")

    epix.bold()
    epix.plot(f, epix.xmin(), epix.xmax(), n=120)

    epix.plain()
    epix.h_axis(4)
    epix.v_axis(4)

    epix.font_size("scriptsize")
    L.draw(loc=epix.canvas().tr(), offset=Point(x=-2, y=-2), align=epix.LabelPos.bl)

    epix.font_size("footnotesize")
    epix.label(
        Point(x=epix.xmin(), y=0),
        offset=Point(x=0, y=-5),
        text="$a$",
        align=epix.LabelPos.b,
    )
    epix.label(
        Point(x=x, y=0), offset=Point(x=0, y=-5), text="$x$", align=epix.LabelPos.b
    )
    epix.label(
        Point(x=x + dx, y=0),
        offset=Point(x=0, y=-2),
        text="$x+dx$",
        align=epix.LabelPos.br,
    )

    epix.pst_format()
fig
