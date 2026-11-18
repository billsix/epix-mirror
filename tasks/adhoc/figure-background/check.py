"""Check the optional figure background (tasks/figure-background.md).

With a background every PNG pixel is opaque and the corners are that colour;
without one the PNG is byte-identical to the transparent rendering made by the
code before the option (the same ghostscript call, no flatten); the eepic is the
same either way. Run in the image:
make shell-exec CMD='python3 tasks/adhoc/figure-background/check.py'.
"""

import shutil
import subprocess
import tempfile

import epix
from epix.render import _rasterize_eepic_text


def identify(png: bytes, fmt: str) -> str:
    """ImageMagick's ``identify -format fmt`` of the PNG bytes."""
    magick: str = shutil.which("magick") or "convert"
    with tempfile.NamedTemporaryFile(suffix=".png") as f:
        f.write(png)
        f.flush()
        return subprocess.run(
            [magick, "identify", "-format", fmt, f.name],
            check=True,
            capture_output=True,
            text=True,
        ).stdout


def draw(background: str | None):
    with epix.figure(
        epix.Point(x=-1, y=-1), epix.Point(x=1, y=1), "2x2in", background=background
    ) as fig:
        epix.line(epix.Point(x=-1, y=-1), epix.Point(x=1, y=1))
    return fig


plain = draw(None)
grey = draw("#f2f2f2")
assert plain.eepic == grey.eepic, "eepic differs"

assert identify(grey.png, "%[opaque]") == "True", "background PNG not opaque"
corner: str = identify(grey.png, "%[hex:p{0,0}]")
assert corner.upper().startswith("F2F2F2"), corner
assert identify(plain.png, "%[opaque]") == "False", "default should stay transparent"

# the default path is the old pipeline: rasterize the same eepic with no flatten
old_png, _ = _rasterize_eepic_text(plain.eepic, 150)
assert old_png == plain.png, "default PNG changed"

try:
    epix.Figure("", background="grey")
    raise AssertionError("bad colour accepted")
except ValueError:
    pass
print("figure background: OK")
