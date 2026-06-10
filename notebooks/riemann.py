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
# # riemann — ported from `samples/riemann.flx` (animation)
# The Riemann surface of Re √(x+iy), sliced and rotated through the family.

# %%
import epix
from epix import P


def F(t, r, theta):
    return P(
        r * r * epix.Cos(2 * theta),
        r * r * epix.Cos(t) * epix.Sin(2 * theta) + r * epix.Sin(t) * epix.Sin(theta),
        r * epix.Cos(theta),
    )


R = epix.domain(P(0, 0, 0), P(1, 1.5, 1), epix.mesh(24, 8, 32), epix.mesh(24, 40, 80))


def build():
    t = epix.tix()
    R1 = R.slice1(t)
    epix.picture(P(-4, -4), P(4, 4), "5x5in")
    epix.begin()
    epix.revolutions()
    epix.grid(1, 1)
    ctr = P(3.25, -3.25)  # "clock" center
    epix.bold()
    epix.circle(ctr, 0.5)
    epix.line(ctr, ctr + epix.polar(0.4, t))
    epix.label(ctr + epix.polar(0.6, t), "$y$")
    epix.line(ctr, ctr + epix.polar(0.4, t + 0.25))
    epix.label(ctr + epix.polar(0.6, t + 0.25), "$w$")
    epix.viewpoint(4, 5, 3)
    epix.camera.range(20)
    epix.line(P(0, 0, 0), P(3, 0, 0))
    epix.line(P(0, 0, 0), P(0, 3, 0))
    epix.line(P(0, 0, 0), P(0, 0, 2))
    epix.plain(epix.Red())
    epix.plot(F, R1.resize3(0, 0.5))
    epix.blue()
    epix.plot(F, R1.resize3(0.5, 1))
    epix.bold(epix.Magenta())
    epix.plot(F, R.slice3(0))
    epix.plot(F, R.slice3(0.5))
    epix.black()
    epix.masklabel(P(3, 0, 0), "$x$")
    epix.masklabel(P(0, 3, 0), "$y$")
    epix.masklabel(
        P(0, 0, 2), P(-4, 0), r"$z=\textrm{Re}\,\sqrt{x+iy}$", epix.LabelPos.tr
    )


# %%
anim = epix.animate(build, count=24)
anim
