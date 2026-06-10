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
# # slopefield — ported from `samples/slopefield.xp`
# A vector field with several integral curves (`ode_plot`).

# %%
import epix
from epix import P


def F(s, t):
    return P(0.1 * s, 0.025 * t) + (1 / (0.01 + s * s + t * t)) * P(-t, s)


# %%
with epix.figure(P(-4, -3), P(2, 2), "3x2.5in") as fig:
    epix.blue(1.8)
    epix.dart_field(F, P(epix.xmin(), epix.ymin()), P(epix.xmax(), epix.ymax()),
                    int(4 * epix.xsize()), int(4 * epix.ysize()))
    epix.set_crop()
    epix.bold()
    for i in range(7):
        epix.rgb(0.05 * (7 - i), 1 - 0.1 * i, 0.15 * i)
        epix.ode_plot(F, P(-0.9 - 0.025 * i, 0), 20, 200)
fig
