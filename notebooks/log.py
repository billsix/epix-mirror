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
# # log — ported from `samples/log.xp`
# Two sheets of the Riemann surface of `log`, with the image of the unit circle.
# Surface patches and path elements share the scene and are painter-sorted by
# distance (patches "fudged" farther so their edges don't occlude the paths).
# Patches are octagons shaded by a point light; the path is drawn in green.

# %%
import math

import epix
from epix import P

LIGHT = P(2, 2, 0)  # location of light, for shading
VIEWPT = P(15, -10, 6)

# surface and path mesh fineness
N1 = 18
N2 = 80
N3 = 120

du = 4.5 / N1
dv = 6.0 / N2
dt = 4.0 / N3

# "gap size" between surface mesh elements
EPS = 0  # (0.002)


# visual styles
def path_color():
    epix.green(0.8)


def label_color():
    epix.yellow(0.5)


def dot_color():
    epix.red()


def path_width():
    epix.pen(1.5)


# can represent either a surface element or a path element
class MeshElt:
    last_was_seg = False  # static shared state

    @classmethod
    def surface(cls, f, u0, v0):
        self = cls.__new__(cls)
        self.is_segment = False
        self.fudge = 0.25  # artificial increment to distance
        self.pt1 = f(u0 + EPS, v0 + EPS)
        self.pt2 = f(u0 + 0.5 * du, v0 + EPS)
        self.pt3 = f(u0 + du - EPS, v0 + EPS)
        self.pt4 = f(u0 + du - EPS, v0 + 0.5 * dv)
        self.pt5 = f(u0 + du - EPS, v0 + dv - EPS)
        self.pt6 = f(u0 + 0.5 * du, v0 + dv - EPS)
        self.pt7 = f(u0 + EPS, v0 + dv - EPS)
        self.pt8 = f(u0 + EPS, v0 + 0.5 * dv)
        self.center = 0.25 * (self.pt1 + (self.pt3 + (self.pt5 + self.pt7)))
        return self

    @classmethod
    def segment(cls, f, t0):
        self = cls.__new__(cls)
        self.is_segment = True
        self.fudge = 0
        self.pt1 = f(t0)
        self.pt2 = f(t0 + 0.25 * dt)
        self.pt3 = f(t0 + 0.5 * dt)
        self.pt4 = f(t0 + 0.75 * dt)
        self.pt5 = f(t0 + dt)
        self.center = 0.333 * (self.pt1 + (self.pt3 + self.pt5))
        return self

    def how_far(self):
        return self.fudge + (self.center - epix.camera.viewpt()).norm()

    def draw(self):
        if not self.is_segment:
            normal = (self.pt2 - self.pt1) ^ (self.pt4 - self.pt1)
            normal = (1 / normal.norm()) * normal

            dens = 0.5 * (1 - ((normal | LIGHT) / LIGHT.norm()))

            if MeshElt.last_was_seg:
                MeshElt.last_was_seg = False
                epix.plain()  # reset pen width

            epix.black()
            epix.gray(dens)

            bd = [
                self.pt1,
                self.pt2,
                self.pt3,
                self.pt4,
                self.pt5,
                self.pt6,
                self.pt7,
                self.pt8,
            ]
            temp = epix.path(bd, True, True)  # closed and filled
            temp.draw()
        else:  # segment
            if not MeshElt.last_was_seg:
                MeshElt.last_was_seg = True
                path_width()
                path_color()

            bd = [self.pt1, self.pt2, self.pt3, self.pt4, self.pt5]
            temp = epix.path(bd, False, False)
            temp.draw()


# the maps to be plotted
def C_log(u, v):
    return epix.polar(math.exp(u), math.pi * v) + P(0, 0, u + math.pi * v)


def C_log1(t):
    return C_log(0, t)


# %%
epix.bounding_box(P(-6, -12), P(6, 12))
epix.unitlength("1in")
epix.picture(P(4, 8))

epix.begin()
epix.fill()

epix.degrees()
epix.label(
    P(0, epix.ymin()),
    P(0, 4),
    r"$z=\mathrm{Re}\,\log(x+iy) + \mathrm{Im}\,\log(x+iy)$",
    epix.LabelPos.t,
)
epix.radians()

epix.viewpoint(VIEWPT)
epix.camera.range(20)

mesh_data = [
    MeshElt.surface(C_log, -3 + du * i, -3 + dv * j)
    for i in range(N1)
    for j in range(N2)
]
mesh_data += [MeshElt.segment(C_log1, -2 + i * dt) for i in range(N3)]

mesh_data.sort(key=lambda m: m.how_far(), reverse=True)

for m in mesh_data:
    m.draw()

dot_color()
epix.marker(C_log(0, -2), epix.MarkType.BOX)
epix.marker(C_log(0, 0), epix.MarkType.BOX)
epix.marker(C_log(0, 2), epix.MarkType.BOX)

label_color()
epix.label(C_log(0, -2), P(6, 0), r"$-2\pi i$", epix.LabelPos.r)
epix.label(C_log(0, 0), P(6, 0), "$0$", epix.LabelPos.r)
epix.label(C_log(0, 2), P(-6, 0), r"$2\pi i$", epix.LabelPos.l)

fig = epix.render()
fig
