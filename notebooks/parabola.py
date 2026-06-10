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
import epix
from epix import P


def f(x):
    return x * x


# %%
with epix.figure(P(-2, 0), P(2, 4), "2.5x2.5in") as fig:
    epix.pen(epix.Black(0.3))
    epix.grid(8, 8)
    epix.pen(epix.Black())
    Ax = epix.axis(P(epix.xmin(), 0), P(epix.xmax(), 0), 4, P(0, -4), epix.LabelPos.b)
    Ay = epix.axis(P(0, epix.ymin()), P(0, epix.ymax()), 4, P(4, 0), epix.LabelPos.r)
    epix.font_size("scriptsize")
    Ax.draw()
    Ay.unmark(0).draw()
    epix.bold(epix.Red())
    epix.plot(f, epix.xmin(), epix.xmax(), 40)
fig
