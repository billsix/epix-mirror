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
from __future__ import annotations

import math

import epix
from epix import Point


# re cos(u+iv)
def f(u: float, v: float) -> epix.Point:
    return Point(x=u, y=v, z=2 * math.cosh(u) * epix.cos(v))


# RGB densities
def color(x: float, y: float, z: float) -> epix.Point:
    return Point(x=epix.sin(x), y=-epix.sin(y), z=-epix.sin(x))


R: epix.Domain = epix.Domain(
    lower_left=Point(x=-1, y=-1),
    upper_right=Point(x=1, y=1),
    coarse=epix.Mesh(nx=16, ny=16),
)

# %%
with epix.figure(
    lower_left=Point(x=0, y=0), upper_right=Point(x=5, y=1), size="5x1in"
) as fig:
    epix.camera.filter(epix.cmyk_neutral())  # convert colors to CMYK in output
    epix.grid(nx=5, ny=1)
    epix.camera.at(4, 3, 1)

    epix.plain(epix.red(1.6))

    # Original
    screen_full: epix.Screen = epix.Screen(
        lower_left=Point(x=-1.5, y=0.5), upper_right=Point(x=1.5, y=3.5)
    )
    epix.activate(screen_full)
    epix.set_crop()

    epix.surface(f, R, color)
    epix.inset(
        child=screen_full, lower_left=Point(x=0, y=0), upper_right=Point(x=1, y=1)
    )

    # Cyan
    screen_cyan: epix.Screen = epix.Screen(
        lower_left=Point(x=-1.5, y=0.5), upper_right=Point(x=1.5, y=3.5)
    )
    epix.activate(screen_cyan)
    epix.set_crop()
    epix.camera.filter(epix.c_process())  # get cyan layer

    epix.surface(f, R, color)
    epix.inset(
        child=screen_cyan, lower_left=Point(x=1, y=0), upper_right=Point(x=2, y=1)
    )

    # Magenta
    screen_magenta: epix.Screen = epix.Screen(
        lower_left=Point(x=-1.5, y=0.5), upper_right=Point(x=1.5, y=3.5)
    )
    epix.activate(screen_magenta)
    epix.set_crop()
    epix.camera.filter(epix.m_process())

    epix.surface(f, R, color)
    epix.inset(
        child=screen_magenta, lower_left=Point(x=2, y=0), upper_right=Point(x=3, y=1)
    )

    # Yellow
    screen_yellow: epix.Screen = epix.Screen(
        lower_left=Point(x=-1.5, y=0.5), upper_right=Point(x=1.5, y=3.5)
    )
    epix.activate(screen_yellow)
    epix.set_crop()
    epix.camera.filter(epix.y_process())

    epix.surface(f, R, color)
    epix.inset(
        child=screen_yellow, lower_left=Point(x=3, y=0), upper_right=Point(x=4, y=1)
    )

    # Black
    screen_black: epix.Screen = epix.Screen(
        lower_left=Point(x=-1.5, y=0.5), upper_right=Point(x=1.5, y=3.5)
    )
    epix.activate(screen_black)
    epix.set_crop()
    epix.camera.filter(epix.k_process())

    epix.surface(f, R, color)
    epix.inset(
        child=screen_black, lower_left=Point(x=4, y=0), upper_right=Point(x=5, y=1)
    )
fig
