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
# # dipole — ported from `samples/dipole.xp`
# Field lines and equipotentials of two like point charges.

# %%
from math import pi

import epix
from epix import P

N = 13
charge1 = 1
charge2 = 1
Q1 = P(1, 0)
Q2 = P(-1, 0)


def unit_charge(arg):
    return epix.recip(arg.dot(arg)) * arg


def E(x, y):
    temp = P(x, y)
    e = charge1 * unit_charge(temp - Q1) + charge2 * unit_charge(temp - Q2)
    return (1.0 / e.dot(e)) * e


def potential(x, y):
    return epix.J(E(x, y))


MAX = 3

# %%
with epix.figure(P(-MAX, -MAX), P(MAX, MAX), "4x4in") as fig:
    epix.set_crop()
    epix.degrees()
    epix.blue()
    for i in range(N):
        epix.ode_plot(E, Q1 + epix.polar(0.05, i * 360.0 / N), 10, 120)
        epix.ode_plot(E, Q2 - epix.polar(0.05, i * 360.0 / N), 10, 120)
        epix.flow(E, Q2 - epix.polar(0.05, i * 360.0 / N), 3, 12)
    epix.green()
    for i in range(-10, 10):
        epix.ode_plot(potential, Q1 + epix.polar(0.25 * pow(0.8, i), 0), 2 * pi, 120)
        epix.ode_plot(potential, Q2 - epix.polar(0.25 * pow(0.8, i), 0), 2 * pi, 120)
    epix.dot_size(6)
    epix.circ(Q1)
    epix.circ(Q2)
    epix.magenta()
    epix.label(Q1, "$+$")
    epix.label(Q2, "$+$")
    epix.blue()
    for i in range(N):
        pt = epix.flow(E, Q2 - epix.polar(0.05, i * 360.0 / N), 3.5, 12)
        epix.arrow(pt, pt + 0.01 * E(pt.x1(), pt.x2()))
        epix.arrow(-pt, -pt + 0.01 * E(-pt.x1(), -pt.x2()))
fig
