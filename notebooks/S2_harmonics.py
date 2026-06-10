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
import epix
from epix import P


def P_n(z):
    # Y_3^0
    return 0.5 * z * (5 * z * z - 3)


def rho(th, phi):
    return P_n(epix.Sin(phi))


def color(x, y):
    return P(rho(x, y), 0, -rho(x, y))


def orbital(u, v):
    return epix.sph(abs(rho(u, v)), u, v)


# %%
with epix.figure(P(-1, -1), P(1, 1.5), "4x5in") as fig:
    epix.backing(epix.Black())

    epix.set_crop()
    epix.revolutions()
    epix.fill()

    # back 3/4
    Bk = epix.domain(P(0.25, -0.25), P(1, 0.25), epix.mesh(24, 96), epix.mesh(48, 96))

    epix.plain(epix.RGB(1, 0.8, 0.2) * 0.8)

    epix.camera.at(epix.sph(10, 0.06, 0.05))

    # color-shaded surface
    epix.surface(orbital, Bk, color)

    # draw boundary with "tube-like" green pen
    epix.nofill()
    epix.plain(epix.Green(1.6))
    epix.base(epix.Green(0.6), "2pt")
    epix.plot(orbital, Bk.slice1(0.25))
    epix.plot(orbital, Bk.slice1(1))

    # set color for axes and labels
    epix.rgb(0.9, 0.7, 1)
    epix.base(epix.RGB(0.8, 0, 1), "1pt")

    epix.arrow(P(0, 0, 0), P(1, 0, 0))
    epix.arrow(P(0, 0, 0), P(0, 0.75, 0))
    epix.arrow(P(0, 0, 0), P(0, 0, 1.25))

    epix.label(P(1, 0, 0), P(-2, -2), "$x$", epix.LabelPos.bl)
    epix.label(P(0, 0.75, 0), P(4, -2), "$y$", epix.LabelPos.r)
    epix.label(P(0, 0, 1.25), P(0, 4), "$z$", epix.LabelPos.t)

    # "unset" base pen
    epix.base(epix.Neutral())

    # inset graph of Legendre polynomial
    graph = epix.screen(P(-1, -1), P(1, 1))
    epix.activate(graph)

    epix.camera.at(P(0, 0, 10000))
    epix.plain(epix.Green(0.6))
    epix.grid(P(-1, -1), P(1, 1), 8, 8)
    epix.bold(epix.Yellow())
    epix.h_axis(8)
    epix.v_axis(8)

    epix.bold(epix.Green())
    epix.plot(P_n, -1, 1, 60)

    epix.font_size("footnotesize")
    epix.label_color(epix.Green(0.6))
    epix.label_mask(epix.RGB(1, 0.8, 0.2))

    # Y30
    epix.label(P(0, -1), P(0, -4), r"$P(z)=\frac{1}{2}(5z^2-3)$", epix.LabelPos.b)

    epix.inset(P(-1, 0.875), P(-0.375, 1.5))
    epix.deactivate(graph)  # a good habit, but not strictly necessary

    epix.tikz_format()
fig
