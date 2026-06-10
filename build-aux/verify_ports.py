#!/usr/bin/env python3
"""Verify ported notebooks reproduce their C++ sample's figure, byte-identically.

For each NAME, execute notebooks/NAME.py (a jupytext percent notebook is valid
Python — markdown is comments, code cells run), grab its result object (`fig` for
a figure, `anim` for an animation), and diff its eepic against the oracle:

  - samples/NAME.xp  -> `epix` renders the oracle eepic.
  - samples/NAME.flx -> compile once, run `./bin i count` per frame (tix = i/count,
    exactly as flix does), diff each frame against anim.frames[i].

Run ONE name per process (state accumulates in libepix across renders); loop in
the shell for several. Run in the container with PYTHONPATH=/epix/python.

Usage:  verify_ports.py NAME            ->  prints "NAME: PASS" or "NAME: FAIL ..."
Exit 0 on PASS, 1 on FAIL/error.
"""

import os
import subprocess
import sys
import tempfile

REPO = "/epix"
NB = os.path.join(REPO, "notebooks")
SAMPLES = os.path.join(REPO, "samples")


def strip(s):
    return "\n".join(
        l for l in s.splitlines() if "Generated" not in l and "Automatically" not in l
    )


def run_notebook(name):
    ns = {}
    with open(os.path.join(NB, name + ".py")) as f:
        exec(compile(f.read(), name + ".py", "exec"), ns)
    for key in ("fig", "anim"):
        if key in ns and ns[key] is not None:
            return ns[key]
    raise RuntimeError("notebook defined neither `fig` nor `anim`")


def _compile(src, out, defs=()):
    subprocess.run(
        [
            "g++",
            "-std=c++20",
            "-DEPIX_FMT_EEPIC",
            *defs,
            "-I/usr/local/include",
            f"-I{SAMPLES}",
            "-x",
            "c++",
            src,
            "-L/usr/local/lib64/epix",
            "-lepix",
            "-lm",
            "-o",
            out,
        ],
        check=True,
        capture_output=True,
        text=True,
    )


def verify_xp(name):
    # Oracle = the sample compiled with eepic forced (-DEPIX_FMT_EEPIC overrides any
    # in-file pst_format()/tikz_format()), run, eepic captured from stdout. This
    # matches the figure the eepic-rendered notebook produces, uniformly.
    obj = run_notebook(name)
    py = strip(obj.eepic)
    with tempfile.TemporaryDirectory() as d:
        binp = os.path.join(d, "bin")
        _compile(os.path.join(SAMPLES, name + ".xp"), binp)
        orig = strip(
            subprocess.run(
                [binp], cwd=d, check=True, capture_output=True, text=True
            ).stdout
        )
    return py == orig, "eepic mismatch" if py != orig else ""


def verify_flx(name):
    anim = run_notebook(name)
    count = len(anim.frames)
    with tempfile.TemporaryDirectory() as d:
        subprocess.run(["cp", os.path.join(SAMPLES, name + ".flx"), d], check=True)
        binp = os.path.join(d, "bin")
        _compile(os.path.join(d, name + ".flx"), binp)
        for i in range(count):
            with open(os.path.join(d, "f.eepic"), "w") as fo:
                subprocess.run([binp, str(i), str(count)], stdout=fo, check=True)
            orig = strip(open(os.path.join(d, "f.eepic")).read())
            if strip(anim.frames[i].eepic) != orig:
                return False, f"frame {i} mismatch"
    return True, ""


def main():
    name = sys.argv[1]
    try:
        if os.path.exists(os.path.join(SAMPLES, name + ".flx")):
            ok, why = verify_flx(name)
        else:
            ok, why = verify_xp(name)
    except Exception as e:  # noqa: BLE001
        print(f"{name}: FAIL ({type(e).__name__}: {e})")
        sys.exit(1)
    print(f"{name}: {'PASS' if ok else 'FAIL (' + why + ')'}")
    # collect notebook-namespace reference cycles (e.g. a scenery holding a
    # callable whose __globals__ points back) so nanobind's shutdown leak-check
    # stays quiet; cosmetic, doesn't affect the result.
    import gc

    gc.collect()
    sys.stdout.flush()
    os._exit(0 if ok else 1)


if __name__ == "__main__":
    main()
