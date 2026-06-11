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
# # S2_harmonics — ported from `samples/S2_harmonics.xp`
# A spherical-harmonic orbital `|P(sin φ)|` (the C++ default is the zonal
# harmonic `Y_3^0`), color-shaded by sign and drawn with a green "tube" boundary,
# plus an inset graph of the underlying Legendre polynomial `P(z)`.

# %%
from __future__ import annotations

import epix
from epix import Point


def P_n(z: float) -> float:
    # Y_3^0
    return 0.5 * z * (5 * z * z - 3)


def rho(th: float, phi: float) -> float:
    return P_n(epix.sin(phi))


def color(x: float, y: float) -> epix.Point:
    return Point(x=rho(x, y), y=0, z=-rho(x, y))


def orbital(u: float, v: float) -> epix.Point:
    return epix.sph(radius=abs(rho(u, v)), theta=u, phi=v)


# %%
with epix.figure(
    lower_left=Point(x=-1, y=-1), upper_right=Point(x=1, y=1.5), size="4x5in"
) as fig:
    epix.backing(epix.black())

    epix.set_crop()
    epix.revolutions()
    epix.fill()

    # back 3/4
    Bk: epix.Domain = epix.Domain(
        lower_left=Point(x=0.25, y=-0.25),
        upper_right=Point(x=1, y=0.25),
        coarse=epix.Mesh(nx=24, ny=96),
        fine=epix.Mesh(nx=48, ny=96),
    )

    epix.plain(epix.rgb(1, 0.8, 0.2) * 0.8)

    epix.camera.at(epix.sph(radius=10, theta=0.06, phi=0.05))

    # color-shaded surface
    epix.surface(orbital, Bk, color)

    # draw boundary with "tube-like" green pen
    epix.nofill()
    epix.plain(epix.green(1.6))
    epix.base(epix.green(0.6), width="2pt")
    epix.plot(orbital, Bk.slice1(0.25))
    epix.plot(orbital, Bk.slice1(1))

    # set color for axes and labels
    epix.set_rgb(0.9, 0.7, 1)
    epix.base(epix.rgb(0.8, 0, 1), width="1pt")

    epix.arrow(tail=Point(x=0, y=0, z=0), head=Point(x=1, y=0, z=0))
    epix.arrow(tail=Point(x=0, y=0, z=0), head=Point(x=0, y=0.75, z=0))
    epix.arrow(tail=Point(x=0, y=0, z=0), head=Point(x=0, y=0, z=1.25))

    epix.label(
        Point(x=1, y=0, z=0),
        offset=Point(x=-2, y=-2),
        text="$x$",
        align=epix.LabelPos.bl,
    )
    epix.label(
        Point(x=0, y=0.75, z=0),
        offset=Point(x=4, y=-2),
        text="$y$",
        align=epix.LabelPos.r,
    )
    epix.label(
        Point(x=0, y=0, z=1.25),
        offset=Point(x=0, y=4),
        text="$z$",
        align=epix.LabelPos.t,
    )

    # "unset" base pen
    epix.base(epix.neutral())

    # inset graph of Legendre polynomial
    graph: epix.Screen = epix.Screen(
        lower_left=Point(x=-1, y=-1), upper_right=Point(x=1, y=1)
    )
    with epix.activated(graph):
        epix.camera.at(Point(x=0, y=0, z=10000))
        epix.plain(epix.green(0.6))
        epix.grid(lower_left=Point(x=-1, y=-1), upper_right=Point(x=1, y=1), nx=8, ny=8)
        epix.bold(epix.yellow())
        epix.h_axis(8)
        epix.v_axis(8)

        epix.bold(epix.green())
        epix.plot(P_n, -1, 1, n=60)

        epix.font_size("footnotesize")
        epix.label_color(epix.green(0.6))
        epix.label_mask(epix.rgb(1, 0.8, 0.2))

        # Y30
        epix.label(
            Point(x=0, y=-1),
            offset=Point(x=0, y=-4),
            text=r"$P(z)=\frac{1}{2}(5z^2-3)$",
            align=epix.LabelPos.b,
        )

        epix.inset(lower_left=Point(x=-1, y=0.875), upper_right=Point(x=-0.375, y=1.5))

    epix.tikz_format()
fig
