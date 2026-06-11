"""Render ePiX output to images for inline notebook display.

`elaps` accepts either a `.xp` source or an existing `.eepic`, so the same
backend serves both: Phase-1 `render_xp` (a source file) and Phase-2
`render_eepic` (what the nanobind scene API emits via `print_eepic`). Pipeline:
elaps -> eps -> ghostscript png. Runs in the container image that ships the
TeX-Live + ghostscript toolchain.
"""

from __future__ import annotations

import os
import shutil
import subprocess
import tempfile


class Figure:
    """A rendered ePiX figure. Displays inline in Jupyter via ``_repr_png_``."""

    def __init__(self, png: bytes, eepic: str | None = None):
        self.png = png
        self.eepic = eepic

    def _repr_png_(self) -> bytes | None:  # IPython/Jupyter inline display hook
        return self.png or None  # None when the figure was too large to rasterize

    def save(self, path: str) -> None:
        with open(path, "wb") as f:
            f.write(self.png)


def _eps_to_png(eps: str, png: str, dpi: int) -> None:
    subprocess.run(
        [
            "gs",
            "-q",
            "-dSAFER",
            "-dNOPAUSE",
            "-dBATCH",
            "-dEPSCrop",
            "-sDEVICE=pngalpha",
            f"-r{dpi}",
            f"-sOutputFile={png}",
            eps,
        ],
        check=True,
        capture_output=True,
    )


def _render_via_elaps(src_path: str, dpi: int) -> Figure:
    """Render a `.xp` or `.eepic` file to a Figure, in a scratch dir."""
    src_path = os.path.abspath(src_path)
    name: str = os.path.basename(src_path)
    with tempfile.TemporaryDirectory() as d:
        shutil.copy(src_path, os.path.join(d, name))
        eps: str = os.path.join(d, "fig.eps")
        subprocess.run(
            ["elaps", "-o", eps, name],
            cwd=d,
            check=True,
            capture_output=True,
            text=True,
        )
        png: str = os.path.join(d, "fig.png")
        _eps_to_png(eps, png, dpi)
        with open(png, "rb") as f:
            data: bytes = f.read()
        # for a .xp, elaps leaves the eepic intermediate; for eepic input use the source
        eepic_path: str = os.path.join(d, "fig.eepic")
        if os.path.exists(eepic_path):
            eepic: str | None = open(eepic_path).read()
        elif name.endswith(".eepic"):
            eepic = open(os.path.join(d, name)).read()
        else:
            eepic = None
        return Figure(data, eepic)


def render_xp(xp_path: str, dpi: int = 150) -> Figure:
    """Render an ePiX ``.xp`` source file to a :class:`Figure` (PNG)."""
    return _render_via_elaps(xp_path, dpi)


def render_eepic(eepic_path: str, dpi: int = 150) -> Figure:
    """Render an existing ``.eepic`` file (e.g. from ``print_eepic``) to a Figure."""
    return _render_via_elaps(eepic_path, dpi)


# convenience alias for rendering an existing .xp: epix.show("foo.xp")
show = render_xp
