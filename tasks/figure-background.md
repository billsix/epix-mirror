# An optional opaque background for rendered figures

**Status:** proposed — needs go-ahead
**Priority:** 4
**Difficulty:** 3
**Created:** 2026-11-16 (William Emerison Six <billsix@gmail.com>)
**Related:** github.com/billsix/geometricalgebra `tasks/live-epix-figure-background.md` (the
consumer half); `tasks/archive/2026/11/16/figure-fit-to-points.md` (the same two-repo workflow)

## BLUF

`python/epix/render.py` rasterizes a figure's eps with `gs -sDEVICE=pngalpha`, so every PNG
`figure()` produces has a transparent background. On a dark-mode HTML page that reads badly.
Add an optional `background="#rrggbb"` to `figure`, `figure_around` and `render` (and the `Figure`
they build) that makes the PNG opaque in that colour. The default, `None`, stays transparent, so
existing output is unchanged. Done means the option exists, is checked, and
github.com/billsix/geometricalgebra's live notebook figures use it with `#f2f2f2` (its half,
`tasks/live-epix-figure-background.md`, bumps `EPIX_COMMIT` and passes the colour).

## Context

- **How a PNG is made:** `Figure.png` is rasterized lazily on first access
  (`_rasterize_eepic_text` → `_render_via_elaps`: `elaps -o fig.eps`, then `_eps_to_png`, which
  runs `gs -sDEVICE=pngalpha … -sOutputFile=fig.png`). `Figure._repr_png_` returns those bytes to
  Jupyter.
- **What geometricalgebra does for its prebuilt figures:** its `tools/render_epix_figures.py`
  flattens each PNG with ImageMagick onto `#f2f2f2`. It wants the same for figures drawn live in
  notebooks, which only pass through this package.
- **The decision is the maintainer's** (William Emerison Six <billsix@gmail.com>, 2026-11-16, in
  that repo's task): the background is an option here, default transparent.
- ImageMagick and ghostscript are in this repo's full image (`entrypoint/02-install-render.sh`).
  The lean image has neither, and rasterizing already raises there.

## Plan (draft)

1. Choose the mechanism (Open question 1). Thread `background: str | None = None` through
   `figure` / `figure_around` / `render` into `Figure`, and from there into the rasterization.
2. Leave the eepic untouched. The background affects only the PNG, so the byte-identity harness is
   unaffected.
3. Check it with a one-shot script under `tasks/adhoc/figure-background/`:
   - with a background, every PNG pixel is opaque and the corners are that colour;
   - without one, the PNG is unchanged (byte-identical to before);
   - the eepic is identical either way.
4. Gates: this repo's `make format` (its `tools/one_process_check.py` findings fail on `master`
   too, see `tasks/archive/2026/11/16/figure-fit-to-points.md`), and
   `make shell-exec SCRIPT=tools/verify_image.sh`.
5. Then geometricalgebra's half, after this is merged and pushed.

## Open questions

1. **Mechanism:** a ghostscript device that renders opaque (`png16m` with the background painted
   first, for example), or keep `pngalpha` and flatten afterwards with ImageMagick
   (`magick fig.png -background <colour> -flatten`), the way geometricalgebra's tool does?
   *(Recommend: flatten with ImageMagick. It matches geometricalgebra's prebuilt figures pixel for
   pixel in method, and the no-background path stays byte-identical.)*
