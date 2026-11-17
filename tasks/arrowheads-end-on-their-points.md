# Arrowheads end on their points

**Status:** approved — in progress
**Priority:** 3
**Difficulty:** 4
**Created:** 2026-11-17 (William Emerison Six <billsix@gmail.com>)
**Related:** github.com/billsix/geometricalgebra `tasks/arrowheads-past-their-points.md` (the consumer
half, blocked on this); `tasks/archive/2026/11/16/figure-fit-to-points.md` (the same two-repo workflow)

## BLUF

`arrow(tail, head)` draws an arrow whose visible tip runs past `head`: about 2.7 times the pen width
(6.4 pt for a 2.4 pt pen), so arrows that meet at a point cross. Make the stroked arrowhead's tip land
exactly on `head`, and keep the shaft from showing past the head. Done means a rendered probe figure's
tips sit on their points at every pen width, and the port harness still passes.

## Context

- `src/curves.cc` `arrow(tail, head, scale)` builds an `arrow_data` (`src/arrow_data.cc`) and draws it.
  `arrow_data::photo` draws the **whole shaft, tail to tip,** as a `pen_line` tile, then adds a
  `pen_arrow` tile for the head (base = the last shaft point before the tip; for a straight arrow, the
  tail).
- `src/pen_arrow.cc` `pen_arrow::print_to` builds a four-point head polygon whose tip is exactly at
  `head` (half-width `scale * arrowhead width`, height `ratio *` that, in pt) and prints it with
  `format::print_fill`, which (`src/eepic.cc`) hatch-fills it **and strokes the outline with the line
  pen**.
- **Measured** (geometricalgebra's image, a 600 dpi probe, arrows drawn to a red reference line): the
  eepic tip coordinate is exactly on the point, and the rendered tip overshoots by about 1.9 pt at pen
  width 0.8 pt, 4.3 pt at 1.6 pt and 6.4 pt at 2.4 pt. The head is narrow (ratio 5.5, a tip
  half-angle $\alpha \approx \arctan(1/5.5) \approx 10°$), and a stroke of width $w$ with a miter
  join reaches $(w/2)/\sin(\alpha) \approx 2.8 w$ past the polygon's corner. The shaft also runs to
  the tip with round caps, so on its own it reaches $w/2$ past the point.

## Plan (the maintainer chose solutions A + B of the geometricalgebra task)

1. **The head:** in `pen_arrow::print_to`, move the polygon back along the arrow by
   $(w/2)/\sin(\alpha)$, so the stroked outline's outer tip is at `head`. The stroke still gives the
   head its visible width (the polygon alone is narrower than a thick shaft), so the head keeps its
   look.
2. **The shaft:** the `pen_arrow` tile draws the last shaft segment itself, ending at the head's foot,
   with the shaft's pen and path style (dashing); `arrow_data::photo` leaves that segment out of the
   `pen_line`. When the head is clipped or cropped away, the segment is drawn in full, as before.
3. **Verify:** a probe figure at several pen widths (saved under `tasks/adhoc/`), measured by pixel;
   the port harness (`build-aux/verify_ports.py`) over the samples that draw arrows; the smoke gate
   `tools/verify_image.sh`; `make format`.

## Open questions

None: the approach was decided in the geometricalgebra task (William Emerison Six
<billsix@gmail.com>, 2026-11-17).
