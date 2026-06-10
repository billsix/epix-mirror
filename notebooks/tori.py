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
# # tori — ported from `samples/tori.xp`
# A torus shown three ways (solid, cropped, boxed) across three `screen` panels.

# %%
import epix
from epix import P


def F(u, v):
    return epix.polar(2.25 + epix.Cos(u), v) + P(0, 0, epix.Sin(u))


# %%
with epix.figure(P(0, 0), P(4, 1), "6 x 1.5in") as fig:
    R = epix.domain(P(0, 0), P(1, 1), epix.mesh(12, 30), epix.mesh(24, 60))
    epix.revolutions()
    epix.camera.at(10, 7, 5)

    scr1 = epix.screen(P(-3, -3), P(3, 3))
    epix.activate(scr1)
    epix.fill(epix.White())
    epix.surface(F, R)
    epix.inset(P(0, 0), P(1, 1))
    epix.deactivate(scr1)

    scr2 = epix.screen(P(-3, -3), P(3, 3))
    epix.activate(scr2)
    epix.set_crop()
    epix.border()
    epix.surface(F, R)
    epix.inset(P(1.5, 0), P(2.5, 1))
    epix.deactivate(scr2)

    scr3 = epix.screen(P(-3, -3), P(3, 3))
    epix.activate(scr3)
    epix.clip_box(P(3.5, 2, 1.5))
    epix.nofill()
    epix.rect(P(-3.5, -2, -1.5), P(-3.5, 2, 1.5))
    epix.rect(P(-3.5, -2, -1.5), P(3.5, -2, 1.5))
    epix.fill()
    epix.surface(F, R)
    epix.nofill()
    epix.rect(P(3.5, -2, -1.5), P(3.5, 2, 1.5))
    epix.rect(P(-3.5, 2, -1.5), P(3.5, 2, 1.5))
    epix.inset(P(3, 0), P(4, 1))
    epix.deactivate(scr3)
fig
