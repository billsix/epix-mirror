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
# # demoivre — ported from `samples/demoivre.xp`
# De Moivre's theorem: $(1 + i\theta/n)^n \to e^{i\theta}$, via the `pair` type.

# %%
from math import pi

import epix
from epix import P

N = 24
theta = 0.875 * pi
alpha = epix.pair(1, theta / N)


def pair2P(arg):
    return P(arg.x1(), arg.x2())


# %%
with epix.figure(P(-1.5, 0), P(1, 1.25), "200 x 100pt") as fig:
    power = epix.pair(1, 0)
    for _ in range(N):
        epix.line(P(0, 0), pair2P(alpha * power))  # two sides
        epix.line(pair2P(power), pair2P(alpha * power))  # of triangle
        power = power * alpha
    epix.label(pair2P(alpha), P(2, 0), r"$\alpha=1+\frac{i\theta}{n}$", epix.LabelPos.r)
    epix.label_angle(theta - pi)
    epix.ddot(pair2P(power), P(2, 4), r"$\alpha^n\approx e^{i\theta}$", epix.LabelPos.b)
    epix.plain(epix.Black(0.3))
    epix.arc(P(0, 0), 1, 0, theta)
    epix.ddot(epix.cis(theta), P(0, -4), "$$", epix.LabelPos.b)
    epix.bold(epix.Red())
    epix.triangle(P(0, 0), P(1, 0), pair2P(alpha))
fig
