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
# # sqrt2 — ported from `samples/sqrt2.xp`
# Both branches of the complex square root, as a self-intersecting surface
# (a `scenery` built from two domain strips).

# %%
import epix
from epix import P

MAX = 1.5


def f(u, v):
    return P(u * u * epix.Cos(v), u * u * epix.Sin(v), u * epix.Cos(0.5 * v))


def sqrt_re(t):
    return f(t, 0)


# %%
with epix.figure(P(-4, -4), P(4, 2), "4x3in") as fig:
    epix.camera.filter(epix.RGB_Neutral())
    epix.backing(epix.Magenta(0.1))
    epix.set_crop()
    epix.revolutions()
    epix.label(
        P(epix.xmin(), epix.ymax()),
        P(2, -2),
        r"Branches of $\sqrt{z}$",
        epix.LabelPos.br,
    )
    epix.camera.at(6, -2, 2)
    epix.camera.range(4)
    R = epix.domain(P(0, 0), P(MAX, 2), epix.mesh(16, 80), epix.mesh(16, 160))
    epix.pen(epix.Green(0.7), "0.15pt")
    surf = epix.scenery(f, R.resize2(1, 2))
    epix.fill(epix.White(0.7))
    epix.pen(epix.Black())
    surf.add(f, R.resize2(0, 1))
    surf.draw()
    epix.nofill()
    epix.bbold(epix.Red())
    epix.plot(sqrt_re, -MAX, 0, 30)
    epix.bold(epix.Blue())
    epix.arrow(P(0, 0, 0), P(2, 0, 0))
    epix.arrow(P(0, 0, 0), P(0, 2, 0))
    epix.label_color(epix.Blue())
    epix.label(P(2, 0, 0), P(0, -4), r"$\mathrm{Re}\,z$", epix.LabelPos.b)
    epix.label(P(0, 2, 0), P(4, 0), r"$\mathrm{Im}\,z$", epix.LabelPos.r)
    epix.bbold(epix.Yellow())
    epix.plot(sqrt_re, 0, MAX, 30)
fig
