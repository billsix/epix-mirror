"""Render ePiX output to images for inline notebook display.

A :class:`Figure` holds the figure's eepic text (the correctness artifact, always
present) and rasterizes it to PNG **lazily**, the first time it is displayed or
saved. Building a figure therefore needs only libepix; displaying one needs the
TeX-Live + ghostscript stack (pipeline: elaps -> eps -> ghostscript png), which
the full container image ships and the lean image (``MINIMAL_IMAGE=1``) omits.

`elaps` accepts either a `.xp` source or an existing `.eepic`, so one backend
serves both `render_xp` (a source file; eager, since the eepic itself comes out
of elaps) and `render_eepic` (what the nanobind scene API emits via
``print_eepic``; lazy).
"""

from __future__ import annotations

import os
import shutil
import subprocess
import tempfile

# Heavy eepic figures (e.g. surface_shade's ~129K-line `picture`) overflow TeX's
# default main_memory when rasterized, leaving an empty PNG (the
# "Pending"-instead-of-figure symptom). extra_mem_top/bot extend the pool at
# *runtime* and kpathsea reads them straight from the environment -- so we pass
# them to the `elaps` subprocess and need no texmf.cnf edit or format rebuild.
# (The C++ originals sidestep the issue by emitting PSTricks instead of eepic.)
_TEX_MEM_ENV = {
    "extra_mem_top": "40000000",
    "extra_mem_bot": "40000000",
    "save_size": "1000000",
    "pool_size": "12500000",
}


class Figure:
    """An ePiX figure: its eepic text, rasterized to PNG on first display.

    ``png`` is computed on demand and cached. A figure too large to rasterize
    (LaTeX/ghostscript fail on it) has an empty ``png`` and displays as nothing
    rather than raising, so a notebook keeps running; a *missing* toolchain
    (``elaps``/``gs`` not installed, i.e. the lean image) raises
    ``FileNotFoundError`` at display time.
    """

    def __init__(self, eepic: str, dpi: int = 150, png: bytes | None = None):
        self.eepic = eepic
        self.dpi = dpi
        self._png: bytes | None = png  # None: not yet rasterized; b"": failed

    @property
    def png(self) -> bytes:
        if self._png is None:
            try:
                self._png, _ = _rasterize_eepic_text(self.eepic, self.dpi)
            except subprocess.CalledProcessError:
                self._png = b""
        return self._png

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


def _render_via_elaps(src_path: str, dpi: int) -> tuple[bytes, str | None]:
    """Rasterize a `.xp` or `.eepic` file in a scratch dir: (png bytes, eepic text)."""
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
            env={**os.environ, **_TEX_MEM_ENV},
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
        return data, eepic


def _rasterize_eepic_text(eepic: str, dpi: int) -> tuple[bytes, str | None]:
    """Write eepic text to a scratch file and rasterize it."""
    with tempfile.TemporaryDirectory() as d:
        path: str = os.path.join(d, "fig.eepic")
        with open(path, "w") as f:
            f.write(eepic)
        return _render_via_elaps(path, dpi)


def render_xp(xp_path: str, dpi: int = 150) -> Figure:
    """Render an ePiX ``.xp`` source file to a :class:`Figure` (eager: the eepic
    itself comes out of ``elaps``, so this needs the full toolchain)."""
    png, eepic = _render_via_elaps(xp_path, dpi)
    return Figure(eepic or "", dpi, png)


def render_eepic(eepic_path: str, dpi: int = 150) -> Figure:
    """Wrap an existing ``.eepic`` file (e.g. from ``print_eepic``) as a Figure.

    The PNG is rasterized lazily on first display.
    """
    with open(eepic_path) as f:
        return Figure(f.read(), dpi)


# convenience alias for rendering an existing .xp: epix.show("foo.xp")
show = render_xp
