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
# # sqrt — ported from `samples/sqrt.xp`
# The Riemann surface of the complex square root (uses the `pair` type as ℂ).

# %%
import epix
from epix import P


def f(x, y):
    z = epix.pair(x, y)
    temp = z * z
    return P(temp.x1(), temp.x2(), x)


def F(r, t):
    return f(r * epix.Cos(t), r * epix.Sin(t))


def g(t):
    return t * P(t, 0, 1)


R = epix.domain(P(0, 0), P(1.25, 0.5), epix.mesh(6, 24), epix.mesh(12, 60))

# %%
with epix.figure(P(-1.5, -1.5), P(1.5, 1.5), "2.5x2.5in") as fig:
    epix.revolutions()
    epix.camera.at(P(4, -2, 3))
    epix.pen(epix.Blue(1.7))
    epix.plot(F, R)
    epix.pen(epix.Blue())
    epix.plot(F, R.resize2(0.5, 1))
    epix.pen(epix.Black())
    epix.arrow(P(0, 0, 0), 2 * epix.E_1)
    epix.arrow(P(0, 0, 0), 2 * epix.E_2)
    epix.arrow(P(0, 0, 0), 1.5 * epix.E_3)
    epix.masklabel(2 * epix.E_1, P(0, 0), r"$\mathrm{Re}\,z$", epix.LabelPos.r)
    epix.label(2 * epix.E_2, P(0, 0), r"$\mathrm{Im}\,z$", epix.LabelPos.r)
    epix.label(1.5 * epix.E_3, P(2, 0), r"$\mathrm{Re}\,\sqrt{z}$", epix.LabelPos.r)
    epix.bold(epix.Red())
    epix.plot(g, -1.25, 1.25, 40)
fig
