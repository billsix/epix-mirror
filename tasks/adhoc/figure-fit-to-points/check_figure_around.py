"""Check epix.fit_bounds / epix.figure_around (tasks/figure-fit-to-points.md).

Run from the repo root with the package importable (``make shell-exec SCRIPT=...``):

    python3 tasks/adhoc/figure-fit-to-points/check_figure_around.py

Each check prints PASS or FAIL; the exit status is nonzero if any failed.
"""

from __future__ import annotations

import sys

import epix


def check(name: str, ok: bool) -> bool:
    """Print one result line and return ``ok``."""
    print(f"{'PASS' if ok else 'FAIL'}  {name}")  # noqa: T201 -- a check script's report is its output
    return ok


def close(a: float, b: float) -> bool:
    """Two floats equal to within rounding."""
    return abs(a - b) < 1e-12


def same_point(p: epix.Point, x: float, y: float) -> bool:
    """``p`` is (x, y)."""
    return close(p.x1(), x) and close(p.x2(), y)


def gacalc_bounds(
    points: list[epix.Point], width: float
) -> tuple[epix.Point, epix.Point, str]:
    """The computation geometricalgebra's ``scene_around`` did by hand (origin
    included, 18% margin), which ``fit_bounds`` replaces."""
    xs: list[float] = [0.0] + [p.x1() for p in points]
    ys: list[float] = [0.0] + [p.x2() for p in points]
    extent: float = max(max(xs) - min(xs), max(ys) - min(ys), 1e-9)
    margin: float = 0.18 * extent
    lower_left: epix.Point = epix.Point(x=min(xs) - margin, y=min(ys) - margin)
    upper_right: epix.Point = epix.Point(x=max(xs) + margin, y=max(ys) + margin)
    box_w: float = upper_right.x1() - lower_left.x1()
    box_h: float = upper_right.x2() - lower_left.x2()
    return lower_left, upper_right, f"{width:.2f}x{width * box_h / box_w:.2f}in"


def main() -> int:
    """Run every check; 0 when all pass."""
    results: list[bool] = []
    lower_left: epix.Point
    upper_right: epix.Point
    size: str

    # the docstring example
    lower_left, upper_right, size = epix.fit_bounds(
        points=[epix.Point(x=0, y=0), epix.Point(x=2, y=1)], width=4, margin=0
    )
    results.append(
        check(
            "docstring example",
            same_point(lower_left, 0, 0)
            and same_point(upper_right, 2, 1)
            and size == "4.00x2.00in",
        )
    )

    # coincident points: a unit box centred on them
    lower_left, upper_right, size = epix.fit_bounds(
        points=[epix.Point(x=3, y=3), epix.Point(x=3, y=3)], width=2, margin=0
    )
    results.append(
        check(
            "coincident points -> unit box",
            same_point(lower_left, 2.5, 2.5)
            and same_point(upper_right, 3.5, 3.5)
            and size == "2.00x2.00in",
        )
    )

    # points on a horizontal line: the box still has height
    lower_left, upper_right, size = epix.fit_bounds(
        points=[epix.Point(x=0, y=1), epix.Point(x=4, y=1)], width=4, margin=0
    )
    results.append(
        check(
            "collinear points -> nonzero height",
            same_point(lower_left, 0, -1) and same_point(upper_right, 4, 3),
        )
    )

    # empty input is an error
    try:
        epix.fit_bounds(points=[], width=1)
        results.append(check("empty points raise ValueError", False))
    except ValueError:
        results.append(check("empty points raise ValueError", True))

    # agrees with geometricalgebra's hand computation for the Part III vectors
    vectors: list[epix.Point] = [
        epix.Point(x=2, y=1),
        epix.Point(x=1, y=2),
        epix.Point(x=3, y=3),
    ]
    expected: tuple[epix.Point, epix.Point, str] = gacalc_bounds(vectors, 3.4)
    lower_left, upper_right, size = epix.fit_bounds(
        points=[epix.Point(x=0, y=0), *vectors], width=3.4
    )
    results.append(
        check(
            "matches geometricalgebra's scene_around",
            same_point(lower_left, expected[0].x1(), expected[0].x2())
            and same_point(upper_right, expected[1].x1(), expected[1].x2())
            and size == expected[2],
        )
    )

    # figure_around draws the same eepic as figure() given fit_bounds' result
    def scene() -> None:
        epix.arrow(tail=epix.Point(x=0, y=0), head=epix.Point(x=2, y=1), scale=0.7)

    with epix.figure_around(points=[epix.Point(x=0, y=0), *vectors], width=3.4) as fig:
        scene()
    around: str = fig.eepic
    with epix.figure(lower_left, upper_right, size) as fig:
        scene()
    results.append(
        check(
            "figure_around eepic == figure eepic", around == fig.eepic and bool(around)
        )
    )

    return 0 if all(results) else 1


if __name__ == "__main__":
    sys.exit(main())
