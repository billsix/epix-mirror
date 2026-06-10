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
# # house — ported from `samples/house.flx` (animation)
# A little house at sunset — sky/ground gradients animate with `tix()`.

# %%
import epix
from epix import P

DY = 0.1
N = 200
dN = 1.0 / N


def build():
    t = epix.tix()
    epix.picture(P(0, 0), P(5, 3), "5 x 3in")
    epix.begin()
    epix.set_crop()
    epix.bbold()
    for i in range(2 * N):
        epix.rgb(
            (1 - 0.6 * t) * (0.5 - 0.25 * dN * i),
            (1 - 0.6 * t) * (0.5 - 0.25 * dN * i),
            (1 - 0.6 * t) * (1 - 0.2 * dN * i),
        )
        epix.line(P(0, 1 + i * dN), P(5, 1 + i * dN))
    epix.pen(4)
    epix.revolutions()
    for i in range(1, 11):
        epix.rgb((1 - 0.25 * t) * (1 - 0.01 * i), (1 - t) * (1 - 0.015 * i), 0)
        epix.circle(P(3 + 0.5 * t, 2.5 - 2 * t), 0.025 * i)
    epix.red(0.6 * (1 - 0.6 * t))
    epix.line(P(0.9, 1.5), P(1.5, 1.8))
    epix.line(P(2.1, 1.5), P(1.5, 1.8))
    epix.bbold()
    for i in range(N):
        epix.green((1 - 0.6 * t) * 0.4 * (1 + i * dN))
        epix.line(P(0, i * dN), P(5, i * dN))
    epix.plain()
    epix.fill()
    epix.gray(0.2 + 0.8 * t)
    house = epix.path()
    house.pt(P(1, 1 - DY)).pt(P(2, 1 - DY)).pt(P(2, 1.5)).pt(P(1.5, 1.75)).pt(P(1, 1.5))
    house.close().fill()
    house.draw()
    epix.gray(0.1 + 0.4 * t)
    epix.rect(P(1.2, 1.1), P(1.4, 1.3))  # window
    epix.pen(2)
    epix.dot_size(1)
    epix.rgb(0.8 * (1 - t), 0.8 * (1 - t), 0)
    epix.ring(P(1.65, 1.15))  # doorknob
    epix.fill(False)
    epix.plain(epix.Black())
    epix.rect(P(1.6, 1 - DY), P(1.85, 1.4))  # door frame
    epix.bold(epix.RGB((1 - t) * 0.6, (1 - t) * 0.4, (1 - t) * 0.2))
    epix.grid(P(1.2, 1.1), P(1.4, 1.3), 2, 2)  # panes


# %%
anim = epix.animate(build, count=24)
anim
