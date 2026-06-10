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
# # saddle — ported from `samples/saddle.xp`
# A quadratic saddle cut by a tangent plane, clipped into two half-spaces
# (`clip_face`/`clip_restore`).

# %%
import epix
from epix import P


def F(u, v):
    return P(u, v, 0.25 * (u + v) * (u - v))


def TF(u, v):
    return P(u, v)


# %%
with epix.figure(P(-3, -2), P(3, 2), "6x4in") as fig:
    epix.backing(epix.Black())
    epix.label_color(epix.White())
    epix.label(
        P(0, epix.ymax()),
        P(0, -4),
        r"\textbf{A quadratic saddle cut by a tangent plane}",
        epix.LabelPos.b,
    )
    epix.camera.at(P(6, 12, 8))
    epix.camera.look_at(P(0, 0, -0.25))
    R = epix.domain(P(-2, -2), P(2, 2), epix.mesh(24, 24), epix.mesh(48, 48))
    epix.clip_face(P(0, 0, 0), -epix.E_3)  # 2nd arg is inward normal
    epix.fill()
    epix.surface(F, R)
    epix.bold()
    epix.green()
    epix.plot(F, R)
    epix.label(F(0, 2), P(0, -4), r"$\mathbf{z<0}$", epix.LabelPos.b)
    epix.red()
    epix.plot(TF, R)
    epix.label(P(2, 0, 0), P(-4, -2), r"$\mathbf{z=0}$", epix.LabelPos.bl)
    epix.clip_restore()
    epix.clip_face(P(0, 0, 0), epix.E_3)
    epix.fill(epix.Blue())
    epix.surface(F, R)
    epix.pen(epix.Red(), 0.2)
    epix.plot(TF, R)
    epix.bold(epix.Blue())
    epix.plot(F, R)
    epix.rgb(0.5, 0.5, 1)
    epix.label(F(-2, 0), P(4, 4), r"$\mathbf{z>0}$", epix.LabelPos.tr)
fig
