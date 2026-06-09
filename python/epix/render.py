"""Render ePiX figures to images for inline display in notebooks.

Phase 1: render an existing ``.xp`` source via the installed ePiX toolchain
(``elaps``: epix -> eepic -> latex -> dvips -> eps), then ghostscript eps->png.
Phase 2 (nanobind scene API) will produce eepic in-process and feed it to the
same eps/png backend, so this module is where inline display lives regardless of
how the figure was built.

The heavy lifting (latex etc.) runs in the container image that already ships
the TeX-Live + ghostscript toolchain; these helpers just shell out to it.
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

    def _repr_png_(self) -> bytes:          # IPython/Jupyter inline display hook
        return self.png

    def save(self, path: str) -> None:
        with open(path, "wb") as f:
            f.write(self.png)


def _eps_to_png(eps: str, png: str, dpi: int) -> None:
    subprocess.run(
        ["gs", "-q", "-dSAFER", "-dNOPAUSE", "-dBATCH", "-dEPSCrop",
         "-sDEVICE=pngalpha", f"-r{dpi}", f"-sOutputFile={png}", eps],
        check=True, capture_output=True,
    )


def render_xp(xp_path: str, dpi: int = 150) -> Figure:
    """Render an ePiX ``.xp`` file to a :class:`Figure` (PNG).

    Renders in a scratch dir so the source tree stays clean. ``elaps`` derives
    its temp paths from the input, so we copy the file in and run it there.
    """
    xp_path = os.path.abspath(xp_path)
    name = os.path.basename(xp_path)
    with tempfile.TemporaryDirectory() as d:
        shutil.copy(xp_path, os.path.join(d, name))
        eps = os.path.join(d, "fig.eps")
        subprocess.run(["elaps", "-o", eps, name],
                       cwd=d, check=True, capture_output=True, text=True)
        png = os.path.join(d, "fig.png")
        _eps_to_png(eps, png, dpi)
        with open(png, "rb") as f:
            data = f.read()
        eepic_path = os.path.join(d, "fig.eepic")
        eepic = open(eepic_path).read() if os.path.exists(eepic_path) else None
        return Figure(data, eepic)


# convenience alias for notebook use: epix.show("foo.xp")
show = render_xp
