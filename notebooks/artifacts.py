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
# # artifacts — ported from `samples/artifacts.xp`
# A sphere drawn at coarse/fine mesh, flat/smooth shading — four panels.

# %%
import epix
from epix import P


def F(u, v):
    return epix.sph(1, u, v)


# %%
with epix.figure(P(0, 0), P(5, 1), "5 x 1in") as fig:
    coarse = epix.domain(P(0, 0), P(0.5, 1), epix.mesh(6, 12), epix.mesh(60, 60))
    fine = epix.domain(P(0, 0), P(0.5, 1), epix.mesh(15, 30), epix.mesh(60, 60))
    epix.revolutions()
    epix.camera.at(P(5, 4, 3))
    epix.fill(epix.White())
    epix.nofill()  # fill color set -> flat shading
    scr1 = epix.screen(P(-1, -1), P(1, 1))
    epix.activate(scr1)
    epix.surface(F, coarse)
    scr2 = epix.screen(P(-1, -1), P(1, 1))
    epix.activate(scr2)
    epix.surface(F, fine)
    epix.fill()
    scr3 = epix.screen(P(-1, -1), P(1, 1))
    epix.activate(scr3)
    epix.surface(F, coarse)
    scr4 = epix.screen(P(-1, -1), P(1, 1))
    epix.activate(scr4)
    epix.surface(F, fine)
    epix.inset(scr1, P(0, 0), P(1, 1))
    epix.inset(scr2, P(1.25, 0), P(2.25, 1))
    epix.inset(scr3, P(2.5, 0), P(3.5, 1))
    epix.inset(scr4, P(3.75, 0), P(4.75, 1))
fig
