"""Draw arrows to a red reference line, render at 600 dpi, and measure the overshoot.

Run inside an image with the epix Python package, TeX and Ghostscript (geometricalgebra's
image; ``run_probe.sh`` drives it). Writes ``probe.png`` and ``probe.eepic`` to the directory
given as the first argument and prints one line per horizontal arrow: its pen width and how
far its rightmost dark pixel lies past the reference line, in pixels and points (negative
means short of it).
"""

import io
import pathlib
import sys

import epix
from epix import Point
from PIL import Image

DPI: int = 600
LOWER_LEFT: Point = Point(x=-0.2, y=-0.5)
UPPER_RIGHT: Point = Point(x=1.3, y=1.7)
HEIGHT_IN: float = 4.4
WIDTHS: tuple[float, ...] = (0.8, 1.6, 2.4)
ROW_GAP: float = 0.3


def main(out_dir: pathlib.Path) -> None:
    """Draw, render, save and measure the probe figure."""
    fig: epix.PendingFigure
    with epix.figure(
        lower_left=LOWER_LEFT, upper_right=UPPER_RIGHT, size=f"3x{HEIGHT_IN}in", dpi=DPI
    ) as fig:
        epix.pen(color=epix.red(), width=0.2)
        epix.line(tail=Point(x=1, y=-0.5), head=Point(x=1, y=1.7))
        index: int
        width: float
        for index, width in enumerate(WIDTHS):
            epix.pen(color=epix.black(), width=width)
            epix.arrow(
                tail=Point(x=0, y=ROW_GAP * index),
                head=Point(x=1, y=ROW_GAP * index),
                scale=0.7,
            )
        # A dashed arrow, a diagonal one, and two arrows meeting tip to tip at
        # (1, 1.5), for the eye.
        epix.pen(color=epix.black(), width=1.1)
        epix.dashed()
        epix.arrow(tail=Point(x=0, y=0.9), head=Point(x=1, y=0.9), scale=0.7)
        epix.line_style(style="-")
        epix.pen(color=epix.blue(), width=2.4)
        epix.arrow(tail=Point(x=0.4, y=1.05), head=Point(x=1, y=1.3), scale=0.7)
        epix.pen(color=epix.green(), width=2.4)
        epix.arrow(tail=Point(x=0.2, y=1.5), head=Point(x=1, y=1.5), scale=0.7)
        epix.pen(color=epix.blue(), width=1.6)
        epix.arrow(tail=Point(x=1, y=1.1), head=Point(x=1, y=1.5), scale=0.7)
    png: bytes = fig._repr_png_()
    (out_dir / "probe.png").write_bytes(png)
    (out_dir / "probe.eepic").write_text(fig.eepic)

    image: Image.Image = Image.open(io.BytesIO(png)).convert("RGB")
    pixels = image.load()
    assert pixels is not None
    columns: int
    rows: int
    columns, rows = image.size
    height_units: float = UPPER_RIGHT.x2() - LOWER_LEFT.x2()
    # the reference line, read on an empty row (y = -0.3) below every arrow
    empty_row: int = round((UPPER_RIGHT.x2() + 0.3) / height_units * rows)
    red_columns: list[int] = [
        x
        for x in range(columns)
        if (lambda r, g, b: r > 200 and g < 120 and b < 120)(*pixels[x, empty_row])
    ]
    red_x: float = sum(red_columns) / len(red_columns)
    for index, width in enumerate(WIDTHS):
        y: float = ROW_GAP * index
        row: int = round((UPPER_RIGHT.x2() - y) / height_units * rows)
        dark: list[int] = [
            x
            for r in range(row - 25, row + 26)
            for x in range(columns)
            if sum(pixels[x, r]) < 150
        ]
        past_px: float = max(dark) - red_x
        print(
            f"pen {width} pt: tip {past_px:+.1f} px = {past_px / DPI * 72:+.2f} pt"
            " past the point"
        )


if __name__ == "__main__":
    main(pathlib.Path(sys.argv[1]))
