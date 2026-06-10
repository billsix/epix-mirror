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
# # decorate — ported from `samples/decorate.xp`
# A torus drawn facet-by-facet as a `mesh_quad` class, each quad shaded by the
# angle between its normal and the view direction and decorated with a diagonal
# "line field". Facets are painter-sorted back-to-front by distance to the eye.
#
# The C++ default (`#define LINEFIELD`, `NORMAL` off) is what's ported here.

# %%
import epix
from epix import P

N1 = 24  # latitudes
N2 = 48  # longitudes

du = 1.0 / N1
dv = 1.0 / N2

VIEWPT = P(6, 3, 4)

r_0 = 0.95  # minor radius
R_0 = 2  # major radius


def g(u):
    return R_0 + r_0 * epix.Cos(u)


def F(u, v):
    return epix.polar(g(u), v) + P(0, 0, r_0 * epix.Sin(u))


# a facet-like class that can be drawn with extra decorations (here, a diagonal
# "line field" and orientation-dependent colors)
class MeshQuad:
    def __init__(self, f, u0, v0):
        self.pt1 = f(u0, v0)
        self.pt2 = f(u0 + du, v0)
        self.pt3 = f(u0 + du, v0 + dv)
        self.pt4 = f(u0, v0 + dv)
        self.center = 0.25 * (self.pt1 + self.pt2 + self.pt3 + self.pt4)
        self.distance = (self.center - epix.camera.viewpt()).norm()

    def how_far(self):
        return self.distance

    def draw(self):
        direction = self.center - epix.camera.viewpt()
        normal = (self.pt2 - self.pt1) ^ (self.pt4 - self.pt1)
        normal = (1 / normal.norm()) * normal

        epix.blue(0.75 * (normal | (epix.recip(self.distance) * direction)))

        epix.fill()
        epix.quad(self.pt1, self.pt2, self.pt3, self.pt4)
        epix.fill(False)

        if (normal | direction) > 0:
            epix.bbold(epix.Blue(1.8))
        else:
            epix.bold(epix.Red())

        epix.line(self.pt1, 0.5 * (self.pt3 + self.pt4))
        epix.line(0.5 * (self.pt1 + self.pt2), self.pt3)


# %%
with epix.figure(P(-3, -3), P(3, 3), "4x4in") as fig:
    epix.revolutions()

    epix.viewpoint(VIEWPT)
    epix.camera.range(10)

    # chop off the front
    epix.clip_face(P(R_0, 0, r_0), P(-0.25, -0.25, -1))

    # build and draw a torus
    mesh = [MeshQuad(F, i * du, j * dv) for i in range(N1) for j in range(N2)]

    mesh.sort(key=lambda m: m.how_far(), reverse=True)

    for m in mesh:
        m.draw()

    epix.pst_format()
fig
