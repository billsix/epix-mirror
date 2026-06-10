# Task: "Slice study" notebook — a surface cut by x- and y-planes with a 2D inset

**Status:** proposed — needs go-ahead to build (and the residual open questions
below answered). Investigation/demo-survey can proceed; the notebook itself waits
for confirmation.
**Requested:** 2026-06-10 (Bill)
**Owner:** Bill (via Claude)

## Goal

A worked-example notebook (`notebooks/<name>.py`) that takes a **surface
`z = f(x, y)`** and produces **one figure** showing the 3-D graph **cut by two
vertical planes — x = x₀ (x bound) and y = y₀ (y bound) at once** — with the **2-D
cross-section drawn as an inset in the upper-left**. `f`, the domain, and the
slice positions `x₀`/`y₀` are defined at the top so the user can swap in their own
function and re-run. (This is the partial-derivative picture: the x = x₀ slice is
the curve `z` vs `y`; the y = y₀ slice is `z` vs `x`.)

### Decisions captured (Bill, 2026-06-10)

- **Function form:** surface `z = f(x, y)` (a graph of a 2-variable function), not
  a scalar field `f(x,y,z)`. So "slices" are the coordinate-plane cross-sections,
  i.e. the partial-derivative cross-sections.
- **Layout:** a **single figure** with **both** slice planes (x = x₀ and y = y₀)
  cut into the one surface, and **one 2-D cross-section inset in the upper-left**.
- **Deliverable:** a **worked example notebook**, not a reusable library helper —
  but written as a template (`f` / domain / `x₀` / `y₀` at the top, easy to swap),
  which is how it satisfies "let the user specify a 3-D function."

### Residual open questions (answer before building)

1. **Which cross-section(s) in the upper-left inset?** With both planes in one
   figure there are two cross-sections (`z` vs `y` at x = x₀, and `z` vs `x` at
   y = y₀). Options: show **both** (two small 2-D axes, or overlaid), or just
   **one** (which?), or the inset cycles. Bill's call.
2. **Slice positions:** fixed `x₀`/`y₀` constants (simplest, like `clipping.xp`),
   or should the demo sweep/animate the slice position (a `.flx`-style frame loop)?
   Default assumption: fixed constants unless Bill wants animation.
3. **Static vs interactive:** the render pipeline produces static PNGs. A slider
   for `x₀`/`y₀` (ipywidgets) would be a bigger architectural add — out of scope
   unless Bill asks. Default: static.
4. **Example `f`:** pick a function that shows the idea well (the monkey saddle
   `0.75·y(y−√3·x)(y+√3·x)` from `clipping.xp`, or something with clearly different
   x- and y-cross-sections, e.g. `x² − y²`). Bill's preference welcome.

## Demo survey (the task's first ask) — seeded from the recent porting work

The two ingredients already exist in the ported demos; the new notebook combines
them. (A quick sweep can confirm completeness, but these are the load-bearing
ones.)

### Demos that show how to make **slices** (the `domain` slice/resize machinery)

The `domain` API does it (all already bound in `python/epix/_epix.cc`):
`slice1(a)` collapses domain coord 1 to the single value `a` (→ an isoline curve);
`slice2(a)` coord 2; `slice3(a)` coord 3; `resize1(a,b)` / `resize2(a,b)` restrict
a coord's range (→ a sub-rectangle of the surface). Then `surface(f, R.resize…)`
draws a piece and `plot(f, R.slice1(x0))` draws the cut curve.

- **`clipping.xp` / `notebooks/clipping.py` — THE reference.** A monkey saddle cut
  into pieces by `R.resize1(…).resize2(…)`, the cut curves drawn with
  `plot(f, R.slice1(x_0).resize2(…))` and `plot(f, R.slice2(y_0)…)`, the cutting
  **planes** drawn as `rect(...)`, everything `clip_box`-ed, with the two partial-
  derivative labels. This is ~90% of the requested figure already — the new demo
  is essentially clipping.py generalized over an arbitrary `f` **plus** a 2-D inset.
- **`torus.py`** — `resize1/resize2/slice1/slice2` for a cut-away wireframe.
- **`minkowski.py`** — `slice2/slice3` cross-sections of the hyperboloids.
- **`saddle.py`** — half-space clipping (`clip_face`/`clip_restore`) of a surface
  (an alternative "cut" mechanism to the box/plane approach).

### Demos that show a **2-D subgraph inset within a (3-D) scene** (`screen` + `inset`)

The `screen` class + `activate`/`inset`/`deactivate` (all bound) put a second,
independently-framed plot inside the main figure:

- **`S2_harmonics.py` — the closest model.** It builds a separate
  `screen graph(P(-1,-1), P(1,1))`, `activate`s it, flattens the camera
  (`camera.at(P(0,0,10000))`), draws a **2-D plot** (`grid` + `h_axis`/`v_axis` +
  `plot(P_n, …)`), then `inset(P(-1,0.875), P(-0.375,1.5))` places it in the
  **upper-left** of the 3-D orbital figure. This is exactly the "2-D plot in the
  upper-left of a 3-D scene" pattern.
- **`extract.py`** — `canvas().extract_ellipse(...)` makes a *magnified* inset of a
  sub-region, placed with `inset(mag, …)` (a different flavor — zoom, not a fresh
  2-D plot).
- **`layout.py` / `layout2.py` / `wheel.py`** — general multi-panel composition via
  `screen` + `activate`/`inset`/`deactivate`.
- **`dataplot.py`** (not yet ported) — superimposes a `screen raw` via
  `activate`/`inset`.

### Synthesis (the recipe the new notebook will follow)

clipping.py's **slicing** (surface pieces + cut curves + cutting planes, `clip_box`)
  ⊕  S2_harmonics.py's **screen inset** (an `activate`d 2-D `screen` with
  `grid`/`h_axis`/`v_axis`/`plot`, flattened camera, `inset` to the upper-left).
Parameterize `f`, domain, `x₀`, `y₀` at the top. Everything needed is already
bound — expect **no new bindings**, which the harness can't verify here (this is a
new figure, not a port of an existing `.xp`), so correctness is **visual** (Bill
verifies the rendered PNG per the build-vs-runtime split), not byte-identity.

## Process

1. **Survey** (above) — confirm the slice + inset demos; note anything missed.
2. **Confirm** the residual open questions with Bill; pick the example `f`.
3. **Build** the notebook (after go-ahead): surface cut by both planes + the 2-D
   cross-section inset upper-left; render and have Bill eyeball the PNG.
4. Iterate on the visual until it reads well; `make format`.

## Relationship to other tasks

- Builds directly on the bindings/ports in `python-bindings-and-notebooks.md`
  (clipping, S2_harmonics, torus, saddle are all ported and their API is bound).
  Unlike those, this is a **new** figure (no `.xp` oracle) → verified visually, not
  byte-for-byte.
- Independent of `notebook-keyword-args.md` / `pythonic-cleanup.md`, but should
  adopt whatever naming/style conventions those settle.

## Notes

- This is a **teaching artifact** (Bill teaches from these) — the point is to make
  the partial-derivative / cross-section idea legible, so clarity of the figure and
  the swap-in-your-own-`f` ergonomics matter more than code cleverness.
