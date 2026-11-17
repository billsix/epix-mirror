# Arrowheads end on their points

**Created:** 2026-11-17 (William Emerison Six <billsix@gmail.com>)
**Done:** 2026-11-17, on branch `arrowheads-end-on-their-points`; the fix is commit `8fe7ff3`
**Related:** github.com/billsix/geometricalgebra `tasks/arrowheads-past-their-points.md` (the consumer
half, which pins the fix)

## What was wrong

`arrow(tail, head)` drew an arrow whose visible tip ran past `head`, so arrows meeting at a point
crossed (seen in geometricalgebra's projection figure, where the projection and rejection meet $a$).
`arrow_data::photo` (`src/arrow_data.cc`) drew the whole shaft to the tip, and `pen_arrow::print_to`
(`src/pen_arrow.cc`) built a head polygon with its tip exactly on `head`, then `eepic::print_fill`
filled it and **stroked its outline with the line pen**. At the narrow tip (ratio 5.5, a half-angle
of about 10°) the stroke's miter reaches $(w/2)/\sin(\text{half-angle}) \approx 2.8 w$ past the
corner. The eepic coordinates were right; the rendered tip was not.

## What changed (commit `8fe7ff3`)

- `pen_arrow::print_to` sets the head polygon back along the arrow by
  $(w/2) \cdot \sqrt{wd^2 + ht^2} / wd$ (the miter length for pen width $w$, head half-width $wd$ and
  height $ht$, all in pt), so the stroked tip lands on `head`. The stroke still gives the head its
  visible width, so heads look as before.
- The `pen_arrow` tile now draws the last shaft segment (base to tip) itself, ending at the head's
  foot under the head, with the shaft's pen and path style (it captures `the_path_style()`, so dashed
  arrows stay dashed). `arrow_data::photo` leaves that segment out of the `pen_line` when the head is
  drawn. A head cropped away later (`crop_to`) prints the segment in full; `is_empty` no longer
  depends on the head being seen.
- Every arrow goes through this path (`arrow` in `src/curves.cc` for straight arrows, and the
  elliptical/arc arrows, whose base is the last point before the tip).

## Verification

- **Probe, before and after** (`tasks/adhoc/arrowheads-end-on-their-points/run_probe.sh head|tree`,
  in geometricalgebra's image, which has TeX and Ghostscript; it builds `HEAD` or the working tree,
  draws arrows to a red reference line at 600 dpi and measures the last dark pixel):

  | pen width | before | after |
  |---|---|---|
  | 0.8 pt | 1.98 pt past | 0.18 pt short |
  | 1.6 pt | 4.38 pt past | 0.18 pt short |
  | 2.4 pt | 6.30 pt past | 0.30 pt short |

  The rendered probe also showed a dashed arrow still dashed, a diagonal arrow ending on the line, and
  two arrows meeting tip to tip without crossing.
- **Gates** (`tasks/adhoc/arrowheads-end-on-their-points/gates.sh` via `make shell-exec`, in the lean
  epix image built for this): the port harness passes for all 11 arrow-drawing samples that have a
  notebook port, and `tools/verify_image.sh` passes (`result: 0`). The harness compares the Python
  port with the C++ sample built against the same new libepix, so it shows the two still agree, not
  the fix; the probe shows the fix.
- `make format`: clean for the changed files. It exits 1 on six ruff findings in
  `tools/one_process_check.py`, which `HEAD` has too (checked with the change stashed); left for
  `tasks/full-repo-audit.md`.

The three adhoc scripts were one-shot and were removed at archive; they remain in commit `8fe7ff3`.
