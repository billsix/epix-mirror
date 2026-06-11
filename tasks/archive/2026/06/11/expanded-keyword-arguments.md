# Task: Expand keyword arguments to the structural/point args (teaching context)

**Status:** DONE (2026-06-11) — convention A applied; cryptic arg names renamed;
all 79 ports verified byte-identical.
**Requested:** 2026-06-11 (Bill)
**Owner:** Bill (via Claude)

## Outcome (2026-06-11)

**Convention A** chosen and applied. First the cryptic *binding* arg names were
renamed (Bill: "I have no idea what sw and ne mean") so the keywords actually teach:
- `sw`/`ne` → **`lower_left`/`upper_right`** (box corners; Bill's pick)
- `ctr`/`rad` → **`center`/`radius`** (unifies with the free `circle()` already using them)
- `perp` → **`normal`**
(in `_epix.cc` `nb::arg(...)` + the `figure()` context-manager params in `figure.py`;
extension rebuilt). Then the notebooks keyword the structural/paired-point args:
`figure`/`picture`/`Domain`/`grid`/`rect`/`inset`/`clip_box`/`Screen` →
`lower_left=`/`upper_right=` (+ `coarse=`/`fine=`/`child=`); `arrow`/`line`/
`Segment`/`dart` → `tail=`/`head=`; `circle`/`Sphere`/`ellipse` →
`center=`/`radius=`/`normal=`; `legend.draw` → `loc=`; `triangle`/`quad` →
`a=`/`b=`/`c=`/`d=`; `clip_face`/`clip_slice` → `loc=`/`normal=`; `arc`/`arc_arrow`
→ `center=`/`r=`/`start=`/`finish=`. Kept bare (convention A): the lone anchor point
in `label(at, …)`/`dot`/`marker`, the function `f`/`F` in `plot`/`surface`, the
domain `R`, colors, and `viewpoint`/`camera.at` coords. **Full harness: 79/79 PASS.**

## Goal

The first keyword-args pass (done, `tasks/archive/2026/06/11/notebook-keyword-args.md`)
deliberately kept **points and colors positional** — only the "unclear trailing
scalar/flag/enum" args got names. Bill wants to **go further**: name the *structural*
arguments too — including the `Point` args — because for a teaching audience the
**role** of each argument is exactly the context a reader needs. The order of two
bare points (which is `sw` vs `ne`? `tail` vs `head`? `loc` vs `offset`?) is *not*
obvious, and naming it is the whole point.

Bill's examples (current → proposed):

```python
L.draw(Point(4, 0.35), Point(0, 0), epix.LabelPos.tr)
L.draw(loc=Point(4, 0.35), offset=Point(0, 0), align=epix.LabelPos.tr)

epix.figure(Point(-1, -1), Point(1, 1.5), size="4x5in")
epix.figure(sw=Point(-1, -1), ne=Point(1, 1.5), size="4x5in")

epix.arrow(Point(0, 0, 0), Point(1, 0, 0))
epix.arrow(tail=Point(0, 0, 0), head=Point(1, 0, 0))

coarse = epix.Domain(Point(0, 0), Point(0.5, 1),
                     epix.Mesh(nx=6, ny=12), epix.Mesh(nx=60, ny=60))
coarse = epix.Domain(sw=Point(0, 0), ne=Point(0.5, 1),
                     coarse=epix.Mesh(nx=6, ny=12), fine=epix.Mesh(nx=60, ny=60))
```

The binding already names all of these (`nb::arg("sw")`, `("ne")`, `("tail")`,
`("head")`, `("loc")`, `("offset")`, `("coarse")`, `("fine")`, `("center")`,
`("perp")`, …), so it's a notebook-only edit. **Byte-identity safe** (keyword args
don't change the call), harness-gated like before.

## Convention to settle (the clarification)

How far to go — the two reasonable lines (decide before the pass):

- **(A) Name args whose role/order isn't self-evident** — paired/ordered points
  (`sw`/`ne`, `tail`/`head`, `loc`/`offset`, `center`/`perp`, `coarse`/`fine`,
  triangle/quad vertices `a`/`b`/`c`/`d`), plus the already-keyworded scalars/enums.
  Keep *single, obviously-positioned* args bare (the lone anchor `Point` in
  `label(at, …)`, the function `f` in `plot(f, …)`, a color in `fill(color)`).
- **(B) Name essentially every argument** that has a binding name, including the
  lone anchor point and the function arg — maximal self-documentation, more verbose.

**Lean (A)** for signal-to-noise (name what's ambiguous; a single point or `f` is
already clear), but Bill's framing ("so much could be added to give the reader
context") leans toward aggressive — **confirm A vs B**, and whether `f`/colors
should be named too.

## Candidate call families (binding arg names to use)

- `figure`/`picture`: `sw=`, `ne=`, `size=`
- `Domain(sw, ne, coarse, fine)`: `sw=`, `ne=`, `coarse=`, `fine=`
- `arrow`/`aarrow`/`dart`/`line`/`infinite_line`/`Segment`: `tail=`, `head=`
- `legend.draw`: `loc=`, `offset=`, `align=`
- `label`/`dot`/`circ`/`masklabel` (4-arg): `at` (bare?) + `offset=`, `text=`, `align=` (already done)
- `circle`/`Circle`: `center=`, `radius=`, `perp=`
- `Sphere`/`sphere`: `center=`/`ctr=`, `radius=`/`rad=`
- `triangle`/`quad`: `a=`, `b=`, `c=`(`, d=`)
- `clip_face`/`clip_slice`: `loc=`, `perp=`(`, thickness=`)
- `inset`: `sw=`, `ne=` (and `child=`)
- `rect`: `sw=`, `ne=`
- `viewpoint`/`camera.at`: `x=`,`y=`,`z=` (or leave — clearly coords)

## Constraints / process

- **No output change** — harness-gate the whole suite (`verify_ports.py`), same as
  the prior passes.
- **Never edit text inside string literals** (LaTeX labels) — see the cleanup doc's
  `P(`-inside-`$…$` lesson.
- Notebook-only; no rebuild needed (binding arg names already exist).
- Apply the agreed convention uniformly across all 81 notebooks for a consistent
  teaching style; likely fold together with `tasks/notebook-structure-and-types.md`
  (type hints + structure) into one final notebook-polish pass so the suite is only
  re-verified once.

## Relationship

- Extends `tasks/archive/2026/06/11/notebook-keyword-args.md` (which stopped at the
  "unclear trailing args" line).
- Pairs naturally with `tasks/notebook-structure-and-types.md`.
