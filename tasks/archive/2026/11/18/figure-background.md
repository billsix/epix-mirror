# An optional opaque background for rendered figures

**Completed:** 2026-11-18 (William Emerison Six <billsix@gmail.com>)
**Priority:** 4
**Difficulty:** 3
**Created:** 2026-11-16 (William Emerison Six <billsix@gmail.com>)
**Related:** github.com/billsix/geometricalgebra `tasks/live-epix-figure-background.md` (the
consumer half, which waits for this to be merged and pushed);
`tasks/archive/2026/11/16/figure-fit-to-points.md` (the same two-repo workflow)

## Outcome

`figure`, `figure_around`, `render`, `render_eepic`, `render_xp` and `Figure` take an optional
`background="#rrggbb"`. With it, the PNG ghostscript writes (`pngalpha`, transparent) is flattened
onto that colour with ImageMagick (`magick png -background <colour> -flatten png`, falling back to
`convert`), so the figure reads well on a dark page. Without it, the default `None`, the PNG is
produced exactly as before. The eepic never changes, so the byte-identity harness is unaffected.
A colour that is not `#rrggbb` raises `ValueError` when the `Figure` is built.

Code: `python/epix/render.py` (`_require_background`, `_flatten`, the `background` parameter
threaded through `Figure` → `_rasterize_eepic_text` → `_render_via_elaps`) and
`python/epix/figure.py` (`render`, `figure`, `figure_around`).

## Decision (William Emerison Six <billsix@gmail.com>, 2026-11-18)

1. **Flatten with ImageMagick, not an opaque ghostscript device.** This matches geometricalgebra's
   prebuilt figures in method, and it leaves the no-background path untouched.

## Verification

- **`tasks/adhoc/figure-background/check.py`**, a one-shot deleted at archive, run with
  `make shell-exec CMD='python3 tasks/adhoc/figure-background/check.py'`:
  - the eepic is identical with and without a background;
  - a `#f2f2f2` figure's PNG is opaque, with corner pixel `F2F2F2`;
  - the default PNG is still transparent and byte-identical to the old pipeline's output for the
    same eepic;
  - a bad colour raises.
- `make shell-exec SCRIPT=tools/verify_image.sh` passed (result 0).
- `entrypoint/format.sh`: ruff and clang-format were clean on the changed files. Its six findings
  are all in `tools/one_process_check.py` and fail on `master` too (noted in
  `tasks/archive/2026/11/16/figure-fit-to-points.md`).
