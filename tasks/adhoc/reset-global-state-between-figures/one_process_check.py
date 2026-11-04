#!/usr/bin/env python3
"""Prove that ePiX figures drawn one after another in ONE process are unaffected
by each other, now that `epix.figure()` / `epix.animate()` call `reset_state()`.

Two modes (run inside the container, where `epix` is installed editable):

  one_process_check.py fresh NAME     -> print NAME's eepic(s), run alone in this process
  one_process_check.py all            -> run EVERY notebook in this one process, in
                                         order, and compare each one's eepic(s) against
                                         a `fresh NAME` subprocess; print PASS/FAIL per
                                         notebook and exit 1 on any FAIL
  one_process_check.py all --no-reset -> the control: the same run with
                                         reset_state() stubbed out in the one-process
                                         run, which should FAIL wherever state leaks

Notebooks that set a picture up by hand (no `epix.figure(`) get an explicit
`epix.reset_state()` before they run, the documented contract for that form.
Paths are relative to this script, so it works wherever the repo is mounted.
"""

from __future__ import annotations

import os
import pathlib
import subprocess
import sys
import types

REPO: pathlib.Path = pathlib.Path(__file__).resolve().parents[3]
NOTEBOOKS: pathlib.Path = REPO / "notebooks"
# hello.py only displays existing .xp files (epix.show); it builds no figure.
SKIP: frozenset[str] = frozenset({"hello"})


def strip(text: str) -> str:
    """Drop the lines that carry run-specific text (timestamps, generator name)."""
    return "\n".join(
        line
        for line in text.splitlines()
        if "Generated" not in line and "Automatically" not in line
    )


def eepics(name: str) -> list[str]:
    """Execute notebooks/NAME.py in this process and return its eepic text(s)."""
    import epix

    source: str = (NOTEBOOKS / f"{name}.py").read_text()
    if "epix.figure(" not in source:
        epix.reset_state()
    # A real module registered in sys.modules: dataclasses (and anything else that
    # resolves a class's module) looks the notebook up there.
    module: types.ModuleType = types.ModuleType(f"notebook_{name}")
    sys.modules[module.__name__] = module
    # Running the notebook as Python is the point (as build-aux/verify_ports.py does).
    exec(compile(source, f"{name}.py", "exec"), module.__dict__)  # noqa: S102
    result: object = module.__dict__.get("fig") or module.__dict__.get("anim")
    match result:
        case None:
            raise RuntimeError(f"{name}: notebook defined neither `fig` nor `anim`")
        case epix.Animation() as anim:
            return [strip(frame.eepic) for frame in anim.frames]
        case figure:
            return [strip(figure.eepic)]


SEPARATOR: str = "\n=====FRAME=====\n"


def main() -> int:
    match sys.argv[1:]:
        case ["fresh", name]:
            sys.stdout.write(SEPARATOR.join(eepics(name)))
            sys.stdout.flush()
            os._exit(0)  # skip nanobind's shutdown leak report (cosmetic)
        case ["all", *flags]:
            if "--no-reset" in flags:
                import epix

                # Stub the binding that figure()/animate() call; fresh runs keep it.
                epix._epix.reset_state = lambda: None
                epix.reset_state = lambda: None
            names: list[str] = sorted(
                p.stem for p in NOTEBOOKS.glob("*.py") if p.stem not in SKIP
            )
            failures: int = 0
            for name in names:
                together: str = SEPARATOR.join(eepics(name))
                alone: str = subprocess.run(
                    [sys.executable, __file__, "fresh", name],
                    check=True,
                    capture_output=True,
                    text=True,
                ).stdout
                ok: bool = together == alone
                failures += 0 if ok else 1
                print(f"{name}: {'PASS' if ok else 'FAIL'}", flush=True)
            print(f"{len(names) - failures}/{len(names)} identical in one process")
            sys.stdout.flush()
            os._exit(1 if failures else 0)
        case _:
            print(__doc__)
            return 2
    return 0


if __name__ == "__main__":
    sys.exit(main())
