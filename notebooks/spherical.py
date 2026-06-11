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
from __future__ import annotations

import epix
from epix import Point

MAX: int = 3


def sph3(
    rho: float, th: float, phi: float
) -> epix.Point:  # wrap so arity is detectable (nanobind fns report 2)
    return epix.sph(radius=rho, theta=th, phi=phi)


# %%
with epix.figure(
    lower_left=Point(x=-MAX, y=-MAX), upper_right=Point(x=MAX, y=MAX), size="6x6in"
) as fig:
    epix.camera.at(30, 5, 20)
    epix.revolutions()
    R: epix.Domain = epix.Domain(
        lower_left=Point(x=0, y=0, z=-0.25),
        upper_right=Point(x=3, y=1, z=0.25),
        coarse=epix.Mesh(nx=12, ny=48, nz=24),
        fine=epix.Mesh(nx=12, ny=96, nz=120),
    )
    Phi: float = 7.0 / 48
    Theta: float = 7.0 / 48
    epix.plain(epix.black(0.5))
    epix.fill(epix.red(1.9))
    coords: epix.Scenery = epix.Scenery(sph3, R.slice1(2))  # rho = 2
    epix.fill(epix.yellow(0.35))
    coords.add(sph3, R.slice2(0)).add(sph3, R.slice2(Theta))
    epix.fill(epix.blue(1.9))
    coords.add(sph3, R.slice3(0)).add(sph3, R.slice3(Phi))
    coords.draw()
    epix.fill(False)
    epix.pen(epix.black())
    epix.base(epix.white(), width="1.2pt")
    epix.arrow_fill(True)
    epix.arrow(tail=Point(x=3, y=0, z=0), head=Point(x=3.5, y=0, z=0))
    epix.label(
        Point(x=3.5, y=0, z=0),
        offset=Point(x=-2, y=-2),
        text="$x$",
        align=epix.LabelPos.bl,
    )
    epix.arrow(tail=Point(x=0, y=3, z=0), head=Point(x=0, y=3.5, z=0))
    epix.label(
        Point(x=0, y=3.5, z=0),
        offset=Point(x=2, y=-2),
        text="$y$",
        align=epix.LabelPos.br,
    )
    epix.arrow(tail=Point(x=0, y=0, z=3), head=Point(x=0, y=0, z=3.5))
    epix.label(
        Point(x=0, y=0, z=3.5),
        offset=Point(x=0, y=4),
        text="$z$",
        align=epix.LabelPos.t,
    )
    epix.bold()
    epix.arc_arrow(
        center=Point(x=0, y=0, z=0), radius=2, start=0.5 * Theta, finish=Theta
    )
    epix.arc_arrow(center=Point(x=0, y=0, z=0), radius=2, start=0.5 * Theta, finish=0)
    epix.aarrow(
        tail=Point(x=0, y=0, z=0), head=epix.sph(radius=2, theta=Theta, phi=Phi)
    )
    epix.label(
        epix.sph(radius=2, theta=0.5 * Theta, phi=0),
        offset=Point(x=2, y=2),
        text=r"$\theta$",
        align=epix.LabelPos.t,
    )
    epix.label(
        epix.sph(radius=2, theta=Theta, phi=0.5 * Phi),
        offset=Point(x=-2, y=0),
        text=r"$\phi$",
        align=epix.LabelPos.l,
    )
    epix.label(
        epix.sph(radius=1, theta=Theta, phi=Phi),
        offset=Point(x=2, y=-2),
        text=r"$\rho$",
        align=epix.LabelPos.br,
    )
    epix.base(epix.neutral(), width=0)
    epix.dashed()
    epix.plain()
    epix.line(tail=Point(x=0, y=0, z=0), head=Point(x=2, y=0, z=0))
    epix.line(tail=Point(x=0, y=0, z=0), head=epix.sph(radius=2, theta=Theta, phi=0))
    epix.line(tail=Point(x=0, y=0, z=0), head=epix.sph(radius=2, theta=Theta, phi=Phi))
fig
