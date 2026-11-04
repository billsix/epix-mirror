#!/usr/bin/env python3
"""The 2026-11-04 repro: a red, dashed figure followed by a plain one, in ONE
process.  Before reset_state() the second figure's eepic was byte-identical to the
first.  Passes when the second equals the same plain figure drawn in a fresh
process, palette (\\xdefinecolor) lines included."""

from __future__ import annotations

import os
import subprocess
import sys

import epix
from epix import Point


def draw(red: bool) -> str:
    fig: epix.figure
    with epix.figure(
        lower_left=Point(x=-1, y=-1), upper_right=Point(x=1, y=1), size="2x2in"
    ) as fig:
        if red:
            epix.pen(color=epix.rgb(r=1, g=0, b=0), width=3)
            epix.dashed()
        epix.line(tail=Point(x=-1, y=0), head=Point(x=1, y=0))
    return fig.eepic


if sys.argv[1:] == ["alone"]:
    sys.stdout.write(draw(red=False))
    sys.stdout.flush()
    os._exit(0)
first: str = draw(red=True)
second: str = draw(red=False)
alone: str = subprocess.run(
    [sys.executable, __file__, "alone"], check=True, capture_output=True, text=True
).stdout
print("repro: PASS" if second == alone and second != first else "repro: FAIL")
sys.stdout.flush()
os._exit(0 if second == alone else 1)
