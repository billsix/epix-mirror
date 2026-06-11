# Task: Structural refactor + type hints for the ported notebooks

**Status:** IN PROGRESS (2026-06-11). Go-ahead given. Defaults chosen: `activated()`
lives in the `epix` package; extraction limited to the worst offenders
(`color_sep`/`tori`/`coord_tricks`); object-typed locals annotated, scalar loop
vars left bare. Stages: A type hints → B1 `activated()` → B2 extraction → B4 enum.
**Requested:** 2026-06-11 (Bill)
**Owner:** Bill (via Claude)

## Goal

The Pythonic-cleanup + keyword-args pass is done (names, idioms, `@dataclass`,
`tasks/archive/2026/06/11/pythonic-cleanup.md`). This is the **next layer**, two
parts Bill asked for:

1. **Structure** — look at how each notebook is built and decide what should be
   *extracted into Python functions / shared helpers*, and where to use **Python
   constructs C++ doesn't have** (context managers, comprehensions, enums,
   generators, decorators) to make the figures read better and teach Python.
2. **Type hints** — add type annotations to notebook **functions** (params +
   returns) and **local variables**. Bill explicitly wants "more Python types."

These are lecture artifacts; the bar is *teaching clarity*, and showing off the
Python concepts C++ lacks is part of the point.

## Hard constraint: NO output change (harness-guarded)

Same rule as the cleanup pass — every notebook must still pass
`build-aux/verify_ports.py NAME` byte-for-byte. Run the full suite after.

- **Type hints: ZERO runtime/output risk** — annotations don't execute (and adding
  `from __future__ import annotations` makes them pure strings). This part is the
  safest possible change.
- **Extraction to functions: LOW risk** — ePiX is a *global-state* engine, so an
  extracted helper must call the drawing primitives in the **exact same order**
  with the **exact same expressions**. Safe when it's a faithful move, not a
  rewrite. Harness re-checks.
- **Comprehensions: CONDITIONAL** — safe when building a *list of objects*
  (`[Sphere(...) for ...]`); **forbidden** when they'd re-order float accumulation
  (the weierstrass / Taylor sums stay loops — see the cleanup doc's off-limits).
- Never restructure a numeric expression.

---

## Part A — Type hints (do first; zero risk)

**Notebook functions.** Almost every notebook defines a math function with a
predictable signature; annotate uniformly:

| pattern (now) | → annotated |
|---|---|
| `def f(x): return 0.4*exp(...)` | `def f(x: float) -> float:` |
| `def F(u, v): return epix.Point(...)` (parametric surface) | `def F(u: float, v: float) -> epix.Point:` |
| `def F(x, y, z): return epix.Point(...)` (3-D field/box) | `def F(x: float, y: float, z: float) -> epix.Point:` |
| `def color(x, y, z): return epix.rgb(...)` | `def color(x: float, y: float, z: float) -> epix.Color:` |
| vector field `def F(u, v): return epix.Point(...)` | `-> epix.Point` |

The bound types are real Python types (`epix.Point`, `epix.Color`, `epix.Domain`,
`epix.Mesh`, `epix.Screen`, `epix.Axis`, `epix.Legend`, `epix.Pair`,
`epix.Complex`, …), so they work directly as annotations. Add
`from __future__ import annotations` at the top of each notebook so annotations are
never evaluated (lazy strings) — keeps things robust and import-light.

**Local variables.** Annotate where it documents intent (not everything):
- `panel: epix.Screen = epix.Screen(sw, ne)`
- `data: epix.DataFile = epix.DataFile(...)`, `bins: epix.DataBins = ...`
- `R: epix.Domain = epix.Domain(...)`
- accumulators/lists: `frames: list[epix.Figure] = []`, `circles: list[epix.Circle] = [...]`
- Leave obvious scalars (`t`, `i`, `x`) unannotated — annotating those is noise.

**Dataclasses** (already converted) — fields are already typed; double-check they
use `epix.Point`/`float` and add `__post_init__` field types.

**Package** (`python/epix/*.py`) — `figure.py`/`render.py` are partly annotated;
finish them (`figure(sw: epix.Point, ne: epix.Point, size: str, ...)` etc.) for a
consistent example.

## Part B — Structural extraction & Python idioms C++ lacks

### B1. `activated(screen)` context manager (highest-value; 17 notebooks)

31 `epix.activate(scr)` / `epix.deactivate(scr)` pairs across 17 notebooks wrap a
block of drawing. That's exactly a **context manager** — a construct C++ has no
clean equivalent for. Add to the `epix` package:

```python
from contextlib import contextmanager
@contextmanager
def activated(screen):
    epix.activate(screen)
    yield screen
    epix.deactivate(screen)
```

Then notebooks read:

```python
with epix.activated(panel):
    epix.surface(F, R)
    epix.inset(panel, sw, ne)
```

instead of the manual `activate(...)` … `deactivate(...)` bracket. Byte-safe (same
two calls, same order). Great teaching moment (RAII-via-`with`).

### B2. Extract repeated panel blocks to helpers/loops

- **color_sep.py** — 5 near-identical CMYK-separation `Screen` blocks (full / C / M
  / Y / K). Extract `def separation(bounds, process_color): …` (or a loop over a
  list of `(bounds, process)`), preserving call order.
- **coord_tricks.py** (3 screen blocks), **tori.py** (3 torus-variant blocks:
  solid / cropped / boxed), **wheel.py** (per-panel loop), **layout.py/layout2.py**
  (grid-of-views loops) — same idea: one helper, called N times.
- Caveat: keep the activate→draw→inset→deactivate order identical; combine with B1.

### B3. Comprehensions where byte-safe

- Building object lists: `mirrorball.py` already does `[big * s for s in spheres]`.
  Apply the same to other "build a list of facets/segments then draw" demos
  (`decorate`, `log`, `lighting`, `helicoid`) **only for the list-build**, not the
  sort/accumulate math.
- Do **not** touch the float-accumulation loops (weierstrass, Taylor in
  legend/plotting3, geomsum) — re-association changes rounding.

### B4. Enums for ad-hoc state flags

- `log.py` carries a segment-vs-surface boolean (`is_segment` / the old
  `last_was_seg`); a small `enum.Enum` (`PatchKind.SURFACE/SEGMENT`) reads better
  than a bool and teaches `enum`.

### B5. Shared notebook helper module (decide location)

B1/B2 helpers can live either (a) in the **`epix` package** (e.g.
`epix.activated`) — reusable, discoverable, part of the teaching API; or (b) a
local `notebooks/_helpers.py`. **Recommend (a)** for `activated()` (broadly useful,
and "ePiX gives you a Python context manager" is a nice story); keep figure-specific
one-offs local. Bill's call.

### B6. Other Python-isms to showcase (lower priority)

- **f-strings** — already used; ensure consistency.
- **generators** — animation frame builders could be generators; minor.
- **`functools.partial`** — for the trampoline-style function args; only if it
  clarifies.
- **`@property`** on the dataclasses for derived read-only values (e.g. a facet's
  `depth`) — teaches properties.

## Risk tiers (for sequencing)

1. **Type hints (Part A)** — zero risk, do first, one pass, verify once.
2. **`activated()` context manager (B1)** — low risk, high clarity, broad.
3. **Panel-block extraction (B2)** — low risk but per-notebook judgement; harness-gate each.
4. **Comprehensions / enums (B3/B4)** — selective, byte-identity-sensitive.

## EXECUTION LOG

- **Stage A — type hints: DONE (2026-06-11), 79/79 PASS.** Added
  `from __future__ import annotations` + param/return annotations to every `def`
  (graph `-> float`, parametric/surface/field `-> epix.Point`, color helpers their
  real type, draw-only `-> None`, dataclass methods) and annotated object-typed
  locals (`panel: epix.Screen`, `R: epix.Domain`, legends/meshes/lists). Scalars and
  lambdas left bare. Annotations don't execute, so byte-identity is automatic.
- **Point() constructors keyworded (Bill, 2026-06-11): DONE, 79/79 PASS.** All 1149
  `Point(a, b[, c])` → `Point(x=a, y=b[, z=c])` via a paren/comment/string-aware
  parser (`/tmp/kw_point.py`). Lesson: the parser MUST skip `#` comments — an
  apostrophe/quote in a comment (`# the cube's edges`) otherwise flips it into
  string mode. Also reverted two Stage-1 leftovers where `P(`→`Point(` had wrongly
  hit *markdown* math notation (`P(z)`, `|P(sin φ)|` in S2_harmonics) — the earlier
  fix only covered string literals, not comments.
- **Stages B1 (`activated()` ctx mgr) / B2 (extraction) / B4 (enum): TODO.**

## Process

1. Agree the scope + the helper location (B5) and how aggressive on B2/B3.
2. Apply Part A everywhere; verify.
3. Apply B1 (package helper) + convert the 17 notebooks; verify.
4. B2/B3/B4 per-notebook; verify the full suite at the end.
5. `make format`.

## Open questions

- `activated()` in the `epix` package vs a local notebook helper? (Recommend package.)
- How far to push extraction — every 3×-repeated block, or only the worst offenders
  (color_sep, tori, coord_tricks)?
- Type-hint depth on locals — annotate all object-typed locals, or only the
  non-obvious ones? (Recommend: object-typed locals yes, scalar loop vars no.)

## Relationship

- Follows `tasks/archive/2026/06/11/pythonic-cleanup.md` (names/idioms/@dataclass)
  and `…/notebook-keyword-args.md` (done). Same harness gate.
- `tasks/source-notebook-grouping.md` (deferred) may interact if helpers get a home.
