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
# # bowl — ported from `samples/bowl.xp`
# The paraboloid z = x² + y², drawn front and back (the `scenery` surface object).

# %%
import epix
from epix import P


def f(u, v):
    return u * P(epix.Cos(v), epix.Sin(v), u)


# %%
with epix.figure(P(-2, -1), P(2, 2), "6x4.5in") as fig:
    epix.backing(epix.Black())
    epix.white()
    epix.label(P(0, epix.ymax()), P(0, -2), r"\LARGE$z=x^2+y^2$", epix.LabelPos.b)
    epix.revolutions()
    epix.camera.at(P(3, 2, 2.5))
    epix.red()
    epix.polar_grid(1, 8, 36)
    epix.arrow(P(0, 0, 0), P(1.25, 0, 0))
    epix.arrow(P(0, 0, 0), P(0, 1.25, 0))
    epix.label(P(1.25, 0, 0), P(-2, -2), "$x$", epix.LabelPos.bl)
    epix.label(P(0, 1.25, 0), P(2, -1), "$y$", epix.LabelPos.br)
    epix.fill()
    epix.rgb(1, 0.8, 0.2)
    R = epix.domain(P(0, 0), P(1, 1), epix.mesh(12, 180), epix.mesh(12, 180))
    my = epix.scenery(f, R)
    my.draw(-1)  # cull back-pointing facets
    epix.red()
    epix.arrow(P(0, 0, 0), P(0, 0, 1))
    epix.pen(epix.Blue(1.6), "0.5pt")
    epix.fill(epix.Blue(1.4))
    my2 = epix.scenery(f, R)
    my2.draw(1)  # cull front-pointing facets
    epix.nofill()
    epix.bold(epix.Green())
    epix.base(epix.Green(0.6), "1.5pt")
    epix.plot(f, R.slice1(1))
fig
