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
# # layout2 — ported from `samples/layout2.xp`
# A 3×2 grid of a vector field with integral curves at varying parameters.

# %%
import epix
from epix import P


def F(s, t):
    return P(0.1 * s, 0.025 * t) + (1 / (0.01 + s * s + t * t)) * P(-t, s)


# %%
with epix.figure(P(0, 0), P(2, 3), "5x7in") as fig:
    for k in range(3):
        for j in range(2):
            my = epix.screen(P(-4, -3), P(2, 2))
            epix.activate(my)
            epix.border(epix.Red(0.6), "1pt")
            epix.set_crop()
            epix.plain(epix.Blue(1.8))
            epix.dart_field(F, P(-4, -3), P(2, 2), 12, 10)
            epix.bold()
            for i in range(7):
                epix.pen(epix.RGB(0.05 * (7 - i), 1 - 0.1 * i, 0.15 * i))
                epix.ode_plot(
                    F,
                    P(-0.9 - 0.025 * i, 0),
                    4 * (3 * j + k + 0.5),
                    int(30 * (3 * j + k + 0.5)),
                )
            epix.inset(P(j + 0.05, 1.05 - k), P(j + 0.95, 1.95 - k))
            epix.deactivate(my)
fig
