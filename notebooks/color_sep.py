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
# # color_sep — ported from `samples/color_sep.xp`
# CMYK color separation: the surface `Re cos(u+iv)` is rendered five times — the
# full color image, then each of the Cyan / Magenta / Yellow / Black process
# layers in turn — via the camera's output `filter`, laid out side by side.

# %%
import math

import epix
from epix import P


# re cos(u+iv)
def f(u, v):
    return P(u, v, 2 * math.cosh(u) * epix.Cos(v))


# RGB densities
def color(x, y, z):
    return P(epix.Sin(x), -epix.Sin(y), -epix.Sin(x))


R = epix.domain(P(-1, -1), P(1, 1), epix.mesh(16, 16))

# %%
with epix.figure(P(0, 0), P(5, 1), "5x1in") as fig:
    epix.camera.filter(epix.CMYK_Neutral())  # convert colors to CMYK in output
    epix.grid(5, 1)
    epix.camera.at(4, 3, 1)

    epix.plain(epix.Red(1.6))

    # Original
    all_scr = epix.screen(P(-1.5, 0.5), P(1.5, 3.5))
    epix.activate(all_scr)
    epix.set_crop()

    epix.surface(f, R, color)
    epix.inset(all_scr, P(0, 0), P(1, 1))

    # Cyan
    cyn = epix.screen(P(-1.5, 0.5), P(1.5, 3.5))
    epix.activate(cyn)
    epix.set_crop()
    epix.camera.filter(epix.C_Process())  # get cyan layer

    epix.surface(f, R, color)
    epix.inset(cyn, P(1, 0), P(2, 1))

    # Magenta
    mgn = epix.screen(P(-1.5, 0.5), P(1.5, 3.5))
    epix.activate(mgn)
    epix.set_crop()
    epix.camera.filter(epix.M_Process())

    epix.surface(f, R, color)
    epix.inset(mgn, P(2, 0), P(3, 1))

    # Yellow
    ylw = epix.screen(P(-1.5, 0.5), P(1.5, 3.5))
    epix.activate(ylw)
    epix.set_crop()
    epix.camera.filter(epix.Y_Process())

    epix.surface(f, R, color)
    epix.inset(ylw, P(3, 0), P(4, 1))

    # Black
    blk = epix.screen(P(-1.5, 0.5), P(1.5, 3.5))
    epix.activate(blk)
    epix.set_crop()
    epix.camera.filter(epix.K_Process())

    epix.surface(f, R, color)
    epix.inset(blk, P(4, 0), P(5, 1))
fig
