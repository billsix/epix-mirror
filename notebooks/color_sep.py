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
# Each layer is the same surface on its own cropped Screen, drawn through a
# different CMYK output filter and inset at column x.  `process=None` keeps the
# camera's current filter (the cmyk_neutral set below) for the full-color image.
def separation(process: epix.Color | None, x: int) -> None:
    panel: epix.Screen = epix.Screen(
        lower_left=Point(x=-1.5, y=0.5), upper_right=Point(x=1.5, y=3.5)
    )
    with epix.activated(panel):
        epix.set_crop()
        if process is not None:
            epix.camera.filter(process)
        epix.surface(f, R, color)
        epix.inset(
            child=panel, lower_left=Point(x=x, y=0), upper_right=Point(x=x + 1, y=1)
        )


with epix.figure(
    lower_left=Point(x=0, y=0), upper_right=Point(x=5, y=1), size="5x1in"
) as fig:
    epix.camera.filter(epix.cmyk_neutral())  # convert colors to CMYK in output
    epix.grid(nx=5, ny=1)
    epix.camera.at(4, 3, 1)

    epix.plain(epix.red(1.6))

    separation(None, 0)  # full color
    separation(epix.c_process(), 1)  # cyan layer
    separation(epix.m_process(), 2)  # magenta layer
    separation(epix.y_process(), 3)  # yellow layer
    separation(epix.k_process(), 4)  # black layer
fig
