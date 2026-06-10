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
# # spherical — ported from `samples/spherical.xp`
# Spherical coordinates: nested coordinate surfaces (ρ, θ, φ) with labelled axes.

# %%
import epix
from epix import P

MAX = 3


def sph3(rho, th, phi):  # wrap so arity is detectable (nanobind fns report 2)
    return epix.sph(rho, th, phi)


# %%
with epix.figure(P(-MAX, -MAX), P(MAX, MAX), "6x6in") as fig:
    epix.camera.at(30, 5, 20)
    epix.revolutions()
    R = epix.domain(
        P(0, 0, -0.25), P(3, 1, 0.25), epix.mesh(12, 48, 24), epix.mesh(12, 96, 120)
    )
    Phi = 7.0 / 48
    Theta = 7.0 / 48
    epix.plain(epix.Black(0.5))
    epix.fill(epix.Red(1.9))
    coords = epix.scenery(sph3, R.slice1(2))  # rho = 2
    epix.fill(epix.Yellow(0.35))
    coords.add(sph3, R.slice2(0)).add(sph3, R.slice2(Theta))
    epix.fill(epix.Blue(1.9))
    coords.add(sph3, R.slice3(0)).add(sph3, R.slice3(Phi))
    coords.draw()
    epix.fill(False)
    epix.pen(epix.Black())
    epix.base(epix.White(), "1.2pt")
    epix.arrow_fill(True)
    epix.arrow(P(3, 0, 0), P(3.5, 0, 0))
    epix.label(P(3.5, 0, 0), P(-2, -2), "$x$", epix.LabelPos.bl)
    epix.arrow(P(0, 3, 0), P(0, 3.5, 0))
    epix.label(P(0, 3.5, 0), P(2, -2), "$y$", epix.LabelPos.br)
    epix.arrow(P(0, 0, 3), P(0, 0, 3.5))
    epix.label(P(0, 0, 3.5), P(0, 4), "$z$", epix.LabelPos.t)
    epix.bold()
    epix.arc_arrow(P(0, 0, 0), 2, 0.5 * Theta, Theta)
    epix.arc_arrow(P(0, 0, 0), 2, 0.5 * Theta, 0)
    epix.aarrow(P(0, 0, 0), epix.sph(2, Theta, Phi))
    epix.label(epix.sph(2, 0.5 * Theta, 0), P(2, 2), r"$\theta$", epix.LabelPos.t)
    epix.label(epix.sph(2, Theta, 0.5 * Phi), P(-2, 0), r"$\phi$", epix.LabelPos.l)
    epix.label(epix.sph(1, Theta, Phi), P(2, -2), r"$\rho$", epix.LabelPos.br)
    epix.base(epix.Neutral(), 0)
    epix.dashed()
    epix.plain()
    epix.line(P(0, 0, 0), P(2, 0, 0))
    epix.line(P(0, 0, 0), epix.sph(2, Theta, 0))
    epix.line(P(0, 0, 0), epix.sph(2, Theta, Phi))
fig
