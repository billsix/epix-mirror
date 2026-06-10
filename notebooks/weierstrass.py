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
# # weierstrass — ported from `samples/weierstrass.xp`
# A Weierstrass-type nowhere-differentiable function (sum of scaled `cb` waves).

# %%
import epix
from epix import P

N = 8  # number of summands


def weierstrass(t):
    y = 0
    for i in range(N):
        y += pow(2, -i) * epix.cb(pow(2, i) * t)
    return y


# %%
with epix.figure(P(-2, 0), P(2, 1.5), "3.2 x 1.2in") as fig:
    epix.h_axis(int(2 * epix.xsize()))
    epix.v_axis(int(2 * epix.ysize()))
    epix.h_axis_labels(int(epix.xsize()), P(-4, -8))
    epix.blue()
    epix.plot(
        epix.cb, epix.xmin() - 0.25, epix.xmax() + 0.25, int(4 * epix.xsize() + 2)
    )
    epix.bold(epix.Black())
    epix.plot(weierstrass, epix.xmin(), epix.xmax(), int(pow(2, N)))
    epix.pen(epix.Red())
    epix.plot(weierstrass, 0.5, 1.5, int(pow(2, N - 2)))
fig
