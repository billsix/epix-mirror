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
# # newton — ported from `samples/newton.xp`
# Newton's method: tangent-line iterates converging to a root.

# %%
import epix
from epix import P


def f(t):
    return 1 + t * (-3 + t * t)


def df(t):
    return epix.deriv(f, t).x2()


# %%
with epix.figure(P(1.5, 0), P(2, 3), "2.5 x 2.5in") as fig:
    epix.h_axis(2)
    x0 = 2
    x1 = x0 - f(x0) / df(x0)
    epix.label(P(x0, 0), P(0, -4), "$x_i$", epix.LabelPos.b)
    for _ in range(3):
        epix.dashed()
        epix.line(P(x0, 0), P(x0, f(x0)))
        epix.solid()
        epix.line(P(x0, f(x0)), P(x0 - f(x0) / df(x0), 0))
        x0 -= f(x0) / df(x0)
    epix.bold()
    epix.plot(f, epix.xmin(), epix.xmax() + 0.05, 60)
    epix.label(P(1.75, f(1.75)), P(-2, 2), "$y=f(x)$", epix.LabelPos.tl)
    epix.label(P(x1, 0), P(0, -4), "$x_{i+1}$", epix.LabelPos.b)
    epix.label(
        P(1.75, df(2) * (1.75 - x1)), P(0, -4), "Slope $= f'(x_i)$", epix.LabelPos.br
    )
fig
