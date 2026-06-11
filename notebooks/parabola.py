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
# # parabola — ported from `samples/parabola.xp`
#
# Graph of $y = x^2$ with a background grid and annotated axes. This is a
# faithful port of the C++ sample — the rendered figure is **byte-identical** to
# the original. Note the natural Python form: the C++ `double f(double)` becomes
# an ordinary Python function, and the axis methods chain (`Ay.unmark(0).draw()`).

# %%
from __future__ import annotations

import epix
from epix import Point


def f(x: float) -> float:
    return x * x


# %%
with epix.figure(
    lower_left=Point(x=-2, y=0), upper_right=Point(x=2, y=4), size="2.5x2.5in"
) as fig:
    epix.pen(epix.black(0.3))
    epix.grid(nx=8, ny=8)
    epix.pen(epix.black())
    Ax: epix.Axis = epix.Axis(
        Point(x=epix.xmin(), y=0),
        Point(x=epix.xmax(), y=0),
        n=4,
        offset=Point(x=0, y=-4),
        align=epix.LabelPos.b,
    )
    Ay: epix.Axis = epix.Axis(
        Point(x=0, y=epix.ymin()),
        Point(x=0, y=epix.ymax()),
        n=4,
        offset=Point(x=4, y=0),
        align=epix.LabelPos.r,
    )
    epix.font_size("scriptsize")
    Ax.draw()
    Ay.unmark(0).draw()
    epix.bold(epix.red())
    epix.plot(f, epix.xmin(), epix.xmax(), n=40)
fig
