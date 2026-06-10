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
import math

import epix
from epix import P

k = 4  # change width of hump
dx = 0.05  # width of thin shaded region
x = 1 / math.sqrt(k)  # position of thin shaded region


def f(t):
    return math.sqrt(abs(k) / (2 * math.pi)) * math.exp(-k * t * t)


shade1 = epix.Black(0.1)
shade2 = epix.Black(0.4)
shade3 = epix.Black(0.6)

# %%
with epix.figure(P(0, 0), P(1, 1), "150x150pt") as fig:
    L = epix.legend()
    L.backing(epix.Neutral())

    epix.fill(shade1)
    epix.shadeplot(f, epix.xmin(), x, 90)
    L.fill_item(r"$=\displaystyle\int_a^x f(t)\,dt$")

    epix.fill(shade2)
    epix.rect(P(x, 0), P(x + dx, f(x)))
    L.fill_item(r"= $f(x)\,dx\vphantom{\Bigg|}$")

    epix.fill(shade3)
    epix.shadeplot(f, x, x + dx, 10)
    L.fill_item("= $F(x+dx)-F(x)$")

    epix.bold()
    epix.plot(f, epix.xmin(), epix.xmax(), 120)

    epix.plain()
    epix.h_axis(4)
    epix.v_axis(4)

    epix.font_size("scriptsize")
    L.draw(epix.canvas().tr(), P(-2, -2), epix.LabelPos.bl)

    epix.font_size("footnotesize")
    epix.label(P(epix.xmin(), 0), P(0, -5), "$a$", epix.LabelPos.b)
    epix.label(P(x, 0), P(0, -5), "$x$", epix.LabelPos.b)
    epix.label(P(x + dx, 0), P(0, -2), "$x+dx$", epix.LabelPos.br)

    epix.pst_format()
fig
