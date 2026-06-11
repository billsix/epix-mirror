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
from __future__ import annotations

from math import pi

import epix
from epix import Point

N: int = 13
charge1: int = 1
charge2: int = 1
Q1: epix.Point = Point(x=1, y=0)
Q2: epix.Point = Point(x=-1, y=0)


def unit_charge(arg: epix.Point) -> epix.Point:
    return epix.recip(arg.dot(arg)) * arg


def E(x: float, y: float) -> epix.Point:
    temp: epix.Point = Point(x=x, y=y)
    e: epix.Point = charge1 * unit_charge(temp - Q1) + charge2 * unit_charge(temp - Q2)
    return (1.0 / e.dot(e)) * e


def potential(x: float, y: float) -> epix.Point:
    return epix.quarter_turn(E(x, y))


MAX: int = 3

# %%
with epix.figure(
    lower_left=Point(x=-MAX, y=-MAX), upper_right=Point(x=MAX, y=MAX), size="4x4in"
) as fig:
    epix.set_crop()
    epix.degrees()
    epix.set_blue()
    for i in range(N):
        epix.ode_plot(E, Q1 + epix.polar(radius=0.05, theta=i * 360.0 / N), 10, n=120)
        epix.ode_plot(E, Q2 - epix.polar(radius=0.05, theta=i * 360.0 / N), 10, n=120)
        epix.flow(E, Q2 - epix.polar(radius=0.05, theta=i * 360.0 / N), 3, 12)
    epix.set_green()
    for i in range(-10, 10):
        epix.ode_plot(
            potential,
            Q1 + epix.polar(radius=0.25 * pow(0.8, i), theta=0),
            2 * pi,
            n=120,
        )
        epix.ode_plot(
            potential,
            Q2 - epix.polar(radius=0.25 * pow(0.8, i), theta=0),
            2 * pi,
            n=120,
        )
    epix.dot_size(diameter=6)
    epix.circ(Q1)
    epix.circ(Q2)
    epix.set_magenta()
    epix.label(Q1, "$+$")
    epix.label(Q2, "$+$")
    epix.set_blue()
    for i in range(N):
        pt: epix.Point = epix.flow(
            E, Q2 - epix.polar(radius=0.05, theta=i * 360.0 / N), 3.5, 12
        )
        epix.arrow(tail=pt, head=pt + 0.01 * E(pt.x1(), pt.x2()))
        epix.arrow(tail=-pt, head=-pt + 0.01 * E(-pt.x1(), -pt.x2()))
fig
