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
# # pole — ported from `samples/pole.xp`
# Re(1/(x+iy)) as a surface with a simple pole, clipped top/bottom for transparency.

# %%
import epix
from epix import P

M = 8
MAX = 1.5
Bd = 2


def F(x, y):  # fake pole
    return P(x, y, x / (0.01 + x * x + y * y))


pt1 = P(-MAX, -MAX)
pt2 = P(MAX, MAX)
R = epix.domain(pt1, pt2, epix.mesh(2 * M, 2 * M), epix.mesh(6 * M, 6 * M))

# %%
with epix.figure(P(-Bd, -Bd), P(Bd, Bd), "2.5x2.5in") as fig:
    epix.label(
        P(epix.xmax(), 2.5),
        P(-2, -2),
        r"$z=\mathrm{Re}\,\displaystyle\frac{1}{x+iy}$",
        epix.LabelPos.bl,
    )
    epix.camera.at(P(6, 8, 5))
    epix.clip_box(P(-Bd, -Bd, -Bd), P(Bd, Bd, 0))
    epix.plain(epix.Magenta())
    epix.fill(epix.White())
    epix.surface(F, R)  # bottom half
    epix.grid(pt1, pt2, M, M)
    epix.pen(epix.White(0.75), "0.15pt")
    epix.plot(F, R)  # simulate transparency
    epix.clip_box(P(-Bd, -Bd, -Bd), P(Bd, Bd, 2 * Bd))
    epix.plain(epix.Magenta())
    epix.arrow(P(-Bd, 0, 0), P(Bd, 0, 0))
    epix.label(P(Bd, 0), P(-4, -2), "$x$", epix.LabelPos.l)
    epix.arrow(P(0, -Bd, 0), P(0, Bd, 0))
    epix.label(P(0, Bd), P(2, -2), "$y$", epix.LabelPos.br)
    epix.arrow(P(0, 0, 0), P(0, 0, 2.5))
    epix.label(P(0, 0, 2.5), P(0, 4), "$z$", epix.LabelPos.t)
    epix.clip_box(P(-Bd, -Bd, 0), P(Bd, Bd, Bd))
    epix.surface(F, R)  # top half
    epix.bbold(epix.Red())
    epix.plot(F, R.resize1(0.25, MAX).slice2(0))
fig
