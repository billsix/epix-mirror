# A figure that fits its points, then use it in geometricalgebra's live notebooks

**Status:** proposed — needs go-ahead (all questions answered)
**Priority:** 6
**Difficulty:** 3
**Created:** 2026-11-16 (William Emerison Six <billsix@gmail.com>)

## BLUF

`epix.figure()` (`python/epix/figure.py`) takes fixed bounds and a fixed size:
`figure(lower_left, upper_right, size, dpi=150)`. A notebook that draws live from vectors a reader
can change needs a figure that fits whatever points it is given. Done means three things.
- This repo's Python layer has such a function, with a doctest or sample.
- In github.com/billsix/geometricalgebra, the `Dockerfile`'s `EPIX_COMMIT` is bumped to a commit
  that has it.
- That repo's local workaround, `scene_around` in `book/figures/epix/_bootstrap_live.py`, calls the
  new function instead of computing the bounds itself.

## Context

- **What geometricalgebra does today** (`book/figures/epix/_bootstrap_live.py` `scene_around`,
  added 2026-11-16 for its book's Part III notebooks `book/docs/notebooks/bootstrap-*.py`):
  - it takes the points a figure must show, plus the origin;
  - it adds a margin of 18% of the larger extent;
  - it computes `lower_left` / `upper_right`;
  - it sets `size` to a fixed 3.4 in width, with the height chosen to keep the two scales equal;
  - it calls `epix.figure(...)` and then draws light axes.
  Only the axes are specific to that book. The fitting is the general part.
- **Another caller there:** geometricalgebra's `book/figures/epix/_frame_scene.py`
  `scene_2d` / `scene_3d` take explicit bounds, which `book/docs/notebooks/frame-qr-2d.py` /
  `frame-qr-3d.py` pick by hand for each figure.
- **How geometricalgebra pins this repo:** the `ARG EPIX_COMMIT=…` in its `Dockerfile`, at
  `275adef…` on 2026-11-16, while this repo's `master` was `7515d93`. `entrypoint/install-epix.sh`
  there fetches and builds that commit. Bumping it needs a `make image` there.
- `epix.figure()` resets ePiX's global state for each figure (`reset_state()`), and the live
  notebooks rely on that. The new function must keep it.

## Plan (draft)

1. **Here:** add `figure_around` (Decision 1) to `python/epix/figure.py`, next to `figure`. It
   takes 2D points, a width, a margin and a dpi. It computes the bounds and the size, with equal scales,
   and delegates to `figure`, so the state reset and rendering stay in one place. Add a doctest or a
   `samples/` / `notebooks/` example, and a `ChangeLog` / `NEWS` entry if this repo keeps them for
   the Python layer.
2. **Gate here:** this repo's own build and test targets (see `CLAUDE.md`).
3. **geometricalgebra:**
   - bump `EPIX_COMMIT` to the new commit and run `make image`;
   - replace the bounds-and-size computation in `scene_around` with the new call, keeping the
     axes it draws;
   - rerun the six `bootstrap-*.py` notebooks and `make docs`, and check that the 13 figures are
     unchanged or better;
   - update the helper's docstring and its note in geometricalgebra's
     `tasks/reference/book-outline.md` ("Structural conventions").
4. Commit each repo's half in that repo.

## Decisions (William Emerison Six <billsix@gmail.com>, 2026-11-16)

1. **API shape:** a separate function, `epix.figure_around(points, width, margin=0.18, dpi=150)`.
   `epix.figure()` keeps its signature, so every existing figure stays byte-identical.
2. **2D only for now.** 3D fitting through the camera's projection is left out. The only callers
   that need fitting are 2D, and a 3D projection depends on a camera set inside the figure.
3. **geometricalgebra's frame-QR notebooks keep their hand-tuned bounds.** Only the Part III
   helper `scene_around` switches to the new function.

## Open questions

None. The task is ready to start on your go-ahead.
