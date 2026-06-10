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
# # trig — ported from `samples/trig.flx` (animation)
# A wheel rolling generates a sine wave (two synchronized `screen` panels).

# %%
import epix
from epix import P


def f(x, t):
    return P(x, epix.Sin(x + t))


R = epix.domain(P(0, 0), P(1, 1), epix.mesh(60, 24), epix.mesh(120, 72))


def build():
    t = epix.tix()
    epix.picture(P(0, 0), P(5, 2), "5x2in")
    epix.begin()
    epix.revolutions()
    epix.border()
    epix.set_crop()
    rolling = epix.screen(P(-1, -1), P(1, 1))
    waving = epix.screen(P(0, -1), P(1, 1))
    epix.activate(rolling)
    ctr = epix.polar(0.5, t)
    rim = epix.polar(0.5, t) + epix.polar(0.5, -t)
    rim2 = epix.polar(0.5, t) + epix.polar(0.5, 0.5 - t)
    epix.circle(ctr, 0.5)  # small wheel
    epix.bold()
    epix.circle(P(0, 0), 1)  # big wheel
    epix.green()
    epix.line(P(1, 0), P(-1, 0))
    epix.blue()
    epix.line(P(0, 1), P(0, -1))
    epix.red()
    epix.line(ctr, rim)
    epix.line(ctr, rim2)
    epix.ddot(rim)
    epix.ddot(rim2)
    epix.deactivate(rolling)
    epix.activate(waving)
    epix.set_crop()
    epix.blue()
    epix.plot(f, R.slice2(t))
    epix.deactivate(waving)
    epix.inset(rolling, P(0, 0), P(2, 2))
    epix.inset(waving, P(3, 0), P(5, 2))
    epix.dashed()
    epix.pen(epix.Blue(1.6), 0.15)
    epix.line(P(1, 1 + epix.Sin(t)), P(3, 1) + f(0, t))


# %%
anim = epix.animate(build, count=24)
anim
