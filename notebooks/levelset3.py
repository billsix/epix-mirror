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
# # levelset3 — ported from `samples/levelset3.xp`
# A Gaussian-modulated saddle `0.9·MAX·e^{-(x²+y²)/2}·xy` drawn as height-banded
# level sets via a stack of thin `clip_slice` planes, each tinted by height and
# captioned in one of three `legend`s (low / middle / high bands).

# %%
import math

import epix
from epix import P

MESH = 12  # number of coordinate grid squares
MAX = 3  # maximum coordinate

sqrt3 = math.sqrt(3)


# function to be graphed
def f(x, y):
    return P(x, y, 0.9 * MAX * math.exp(-0.5 * (x * x + y * y)) * x * y)


def color(u, v, w):
    return P(-0.5 * w, 0.25 * w, 0.5 * w)


Bk = epix.Green(0.7)

# %%
with epix.figure(P(-MAX, -MAX), P(MAX, MAX), "6 x 6in") as fig:
    epix.backing(Bk)

    epix.camera.at(0, -10, 4)

    epix.border(epix.Green(0.6), "1pt")

    epix.clip_box(P(MAX, MAX, MAX))

    # the C++ 3rd ctor arg is the comma-expression `(6*MESH, 6*MESH)` == 72,
    # which converts to mesh(72); reproduced here as epix.mesh(6 * MESH).
    R = epix.domain(
        P(-MAX, -MAX), P(MAX, MAX), epix.mesh(6 * MESH), epix.mesh(6 * MESH, 6 * MESH)
    )

    L1 = epix.legend()
    L2 = epix.legend()
    L3 = epix.legend()
    L1.backing(Bk)
    L2.backing(Bk)
    L3.backing(Bk)

    for i in range(-10, 11):
        epix.clip_slice(P(0, 0, 0.1 * i), P(0, 0, 1), 0.1)
        epix.rgb(0.25 + 0.1 * i, 0, -0.1 * i)

        buf = rf"${0.1 * i - 0.05:g}\leq z \leq {0.1 * i + 0.05:g}$"

        if i < -3:
            L1.fill_item(buf)
        elif i < 4:
            L2.fill_item(buf)
        else:
            L3.fill_item(buf)

        epix.nofill()
        epix.surface(f, R)
        epix.clip_restore()

    epix.camera.at(0, 0, 1000)
    epix.font_size("scriptsize")
    epix.label_color(epix.Black())
    L1.border(0)
    L2.border(0)
    L3.border(0)

    L1.draw(epix.canvas().bl(), P(2, 2), epix.LabelPos.tr)
    L2.draw(epix.canvas().bl() + P(2, 0), P(2, 2), epix.LabelPos.tr)
    L3.draw(epix.canvas().bl() + P(4, 0), P(2, 2), epix.LabelPos.tr)

    epix.pst_format()
fig
