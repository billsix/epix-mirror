# A figure that fits its points, used by geometricalgebra's live notebooks

**Created:** 2026-11-16 (William Emerison Six <billsix@gmail.com>)
**Done:** 2026-11-16, on branch `figure-fit-to-points` here and branch
`use-epix-figure-fit-to-points` in github.com/billsix/geometricalgebra

## What was done

`epix.figure()` takes fixed bounds and a fixed size. geometricalgebra's Part III notebooks draw live
from vectors a reader can change, so they had computed the bounds themselves in a helper. This
task moved that computation into epix and switched the helper over to it.

- **Here** (`python/epix/figure.py`, exported from `python/epix/__init__.py`):
  - `fit_bounds(points, width, margin=0.18)` returns `(lower_left, upper_right, size)`, the first
    three arguments of `figure()`, sized to hold 2D points at a fixed width with equal scales on
    both axes.
    - The box is the points' extent, grown on each side by `margin` times the larger side.
    - Coincident points get a unit box.
    - An axis the points don't spread along gets the full extent, so neither side is ever zero.
    - An empty list raises `ValueError`.
  - `figure_around(points, width, margin=0.18, dpi=150)` is `figure()` with those arguments.
  - `figure()` itself is unchanged, so every existing figure and the byte-identity harness are
    unaffected.
  - `CLAUDE.md`'s `python/` layout note mentions both. No `ChangeLog` or `NEWS` entry: those are
    upstream prose, which this mirror leaves intact.
- **In geometricalgebra** (commit "Use epix.fit_bounds in the Part III notebook scenes; pin
  epix-mirror 8cb788a"):
  - `book/figures/epix/_bootstrap_live.py` `scene_around` gets its box and size from
    `epix.fit_bounds` and still draws its axes across the box.
  - `Dockerfile` `EPIX_COMMIT` moved from `275adef…` to `8cb788a…`, this repo's implementation
    commit. Between the two pins only `python/epix/` changed.
  - That repo's `tasks/reference/book-and-docs-pipeline.md` records the new minimum pin.

## Decisions (William Emerison Six <billsix@gmail.com>, 2026-11-16)

1. **A separate function**, `epix.figure_around`, leaving `figure()`'s signature and output alone.
2. **2D only.** 3D fitting through the camera's projection was left out.
3. **geometricalgebra's frame-QR notebooks keep their hand-tuned bounds.**

Judgment calls made during the work:
- `fit_bounds` is public alongside `figure_around`, because geometricalgebra's `scene_around` needs
  the box itself to draw its axes. It calls `fit_bounds` and then `figure()` rather than
  `figure_around`.
- The pin is the implementation commit `8cb788a`, not this branch's tip, so later task-doc commits
  don't matter to the pin.

## Owed: push before geometricalgebra's image build

geometricalgebra's `entrypoint/install-epix.sh` fetches `EPIX_COMMIT` from
github.com/billsix/epix-mirror. `8cb788a` exists only locally until this branch is merged and
pushed to the `github` remote. Until then, geometricalgebra's `make image` fails to fetch it. After
the push, run `make image` there, then `make docs`.

## Verification

- `tasks/adhoc/figure-fit-to-points/check_figure_around.py` (run in geometricalgebra's image with
  this tree's `python/` on `PYTHONPATH`; a one-shot script, removed at archive and kept in commit
  `8cb788a`) passes all six checks:
  - the docstring example;
  - coincident points give a unit box;
  - collinear points give a box with height;
  - empty input raises;
  - the result agrees with geometricalgebra's old hand computation for the Part III vectors;
  - `figure_around`'s eepic equals `figure()`'s for the same scene.
- ruff (this repo's `python/pyproject.toml` config): lint and format clean on `python/epix` and the
  check script.
- This repo's containerized gates, after a nested `make image`:
  - `make shell-exec SCRIPT=tools/verify_image.sh` passed (environment, the 8-port byte-identity
    sample, the Jupyter settings).
  - `make format` exits 1, but not because of this change. `master` fails identically, on six
    ruff findings in `tools/one_process_check.py` (one E501 and five T201 `print`s), a file this
    task didn't touch. The files this task changed are clean. Left as found.
- geometricalgebra, with this tree's `python/epix` mounted over the image's copy:
  - all six `bootstrap-*.py` notebooks execute;
  - their 13 figures are byte-identical PNGs before and after the change;
  - `make format` is green.
  - `make image` and `make docs` there wait on the push above.
