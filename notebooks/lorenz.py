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
# # lorenz — ported from `samples/lorenz.xp`
# The Lorenz attractor as a stereo pair (two `screen`s at slightly different
# camera angles), with the flow field and two trajectories.

# %%
import epix
from epix import P

A1 = 10
A2 = 28
A3 = 2.6666
MAX = 30
XMAX = 50
TMAX = 2000


def F(x, y, z):
    return 0.01 * P(A1 * (y - x), x * (A2 - z) - y, x * y - A3 * z)


R = epix.domain(P(-MAX, -MAX, 0), P(MAX, MAX, 2 * MAX), epix.mesh(6, 6, 6))


def draw_frame():
    epix.dart_field(F, R.resize1(0, MAX), 0.5)
    epix.bold(epix.Red())
    epix.ode_plot(F, P(10, 0, 0), 0, TMAX, TMAX)
    epix.pen(epix.Blue())
    epix.ode_plot(F, P(10.5, 0, 0), 0, TMAX, TMAX)
    epix.plain(epix.Black())
    epix.dart_field(F, R.resize1(-MAX, 0), 0.5)


def init_cam(arg):
    epix.camera.at(arg)
    epix.camera.look_at(P(0, 0, MAX))
    epix.camera.range(500)


# %%
with epix.figure(P(-1, 0), P(1, 1), "6x3in") as fig:
    left_eye = epix.screen(P(-XMAX, -XMAX), P(XMAX, XMAX))
    epix.activate(left_eye)
    init_cam(P(-100, 90, 63))
    draw_frame()
    right_eye = epix.screen(P(-XMAX, -XMAX), P(XMAX, XMAX))
    epix.activate(right_eye)
    init_cam(P(-100, 80, 63))
    draw_frame()
    epix.inset(left_eye, P(0, 0), P(1, 1))
    epix.inset(right_eye, P(-1, 0), P(0, 1))
fig
