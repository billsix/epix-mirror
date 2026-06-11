# Task: Annotate local variables across the Python code (types everywhere)

**Status:** proposed — investigation DONE (scope below); ready to execute on
go-ahead. Zero output risk (local annotations are never evaluated), still
harness-gated.
**Requested:** 2026-06-11 (Bill)
**Owner:** Bill (via Claude)

## Goal

Add type annotations to **local variables** throughout the Python code
(`notebooks/*.py` + `python/epix/*.py`) — the scalars and intermediate values the
earlier type-hints pass deliberately left bare. Bill's examples:

```python
def Phi12(t: float) -> epix.Point:
    cusp_pt = cubic(t)                 # -> cusp_pt: epix.Point = cubic(t)
    return Point(x=cusp_pt.x1(), y=cusp_pt.x2(), z=-8)

def g(t: float) -> float:
    d = 1 - t * t                      # -> d: float = 1 - t * t
    return 1.0 / d if d != 0.0 else float("inf")
```

These are lecture artifacts; the bar is teaching clarity, and "every value has a
visible type" is part of the point.

## Why this reverses an earlier decision (on purpose)

`tasks/archive/2026/06/11/notebook-structure-and-types.md` Part A annotated `def`
params/returns and **object-typed** locals (`panel: epix.Screen`, `R:
epix.Domain`, lists) but **explicitly left "obvious scalars (t, i, x) bare —
annotating those is noise."** Bill now wants those scalars typed too. So this task
picks up exactly what Part A skipped, plus any object-typed locals Part A's
"representative subset" missed.

## Hard constraint: NO output change (harness-guarded) — and it's the safest case

- **Local variable annotations are *never evaluated*** (PEP 526: an annotation in
  a function/block body is not evaluated and not stored). `d: float = 1 - t*t`
  runs exactly as `d = 1 - t*t`. So this is **zero runtime / zero eepic risk** —
  even safer than Part A's signature hints.
- Still run the full `verify_ports.py` sweep at the end as cheap insurance (and
  because annotations can introduce a *syntax* slip).

## Scope (measured 2026-06-11, via AST)

**262 untyped `name = …` assignments across 59 files.**
- **90 inside `def` bodies** (Bill's example case).
- **172 at cell / `with` / `for`-block level** (the notebook drawing code inside
  `with epix.figure(...)`).

RHS-kind distribution (survey guess at the type):

| count | RHS shape | annotation |
|---|---|---|
| 62 | numeric expr (`1 - t*t`, `du*i`, …) | `float` (or `int` if all-int) |
| 59 | int literal / counter | `int` |
| 29 | float literal | `float` |
| 12 | `Point(...)` | `epix.Point` |
| 11 | `epix.animate(...)` (figure.py) | `epix.Animation` |
| 7  | `epix.tix()` | `float` |
| 6  | `cubic(t)` / user fns returning a point | `epix.Point` |
| 6  | `os.path.join(...)` (render.py/figure.py) | `str` |
| 5  | `math.sqrt(...)` / `.norm()` / `epix.sin(...)` | `float` |
| 3  | `epix.sph/cyl/polar(...)` | `epix.Point` |
| 3  | `epix.render()` | `epix.Figure` |
| ~15| method calls / subscripts (`extract_ellipse`, …) | per-case (read the binding return) |

Top files: `lighting` (18), `helicoid` (15), `log` (15), `figure.py` (12),
`steiner2` (11), `render.py` (10), `butterfly` (9), `pendulum` (9), `decorate`
(8), `extract` (8). 59 files total carry at least one.

## Type sources (the rules to apply)

- **Scalars:** `int` for integer literals/counters, `float` for float literals,
  division, and any expression touching a float (`1 - t*t`, `4.5/N1`, `du*i`).
  Getting int-vs-float "wrong" has **zero runtime effect** (not evaluated), but
  match the real runtime type for correctness/teaching.
- **Points:** `epix.Point` for `Point(...)`, `sph/cyl/polar(...)`, and user
  functions that return a point (`cubic`, `Phi`, `Phi12`, …). Coordinate
  accessors `.x1()/.x2()/.x3()` and `.norm()` return **`float`**.
- **Strings:** `str` for path joins, filenames, f-strings.
- **Colors:** `epix.Color` for color-factory results (`epix.black()`, …).
- **Lists:** `list[epix.Point]`, `list[epix.Figure]`, etc.
- **Package types** (`figure.py`/`render.py`): `epix.Figure`, `epix.Animation`,
  `bytes`, `str`, file handles — finish these for a consistent example.
- **The ~15 method/subscript cases:** inspect the binding's return type
  (`nm -C`/the `_epix.cc` signature) when not obvious — e.g.
  `canvas().extract_ellipse(...)` returns an epix canvas/screen type.

## What CANNOT be annotated (leave alone — note in the doc)

- **`for` targets** — `for i in range(N):` can't take `i: int` (syntax error).
  Loop variables are naturally excluded (the survey already only counts
  `ast.Assign`, not `for`).
- **Tuple-unpacking** `a, b = ...` — no inline annotation form. Skip.
- **Augmented assignments** `mesh_data += [...]` — can't annotate. Skip (the first,
  annotated, binding already carries the type).
- **Re-assignments** of an already-annotated name — annotate the first binding
  only; don't repeat.

## Approach

1. **Script-assisted, human-reviewed.** An AST pass can locate every untyped
   `Name = …` in a def/block body and *propose* an annotation from the RHS
   (literal type, known call → return type, numeric expr → `float`). Insert the
   easy/unambiguous ones programmatically; **hand-resolve the ~15 method/subscript
   cases** and any int-vs-float judgement calls.
2. Keep `from __future__ import annotations` at each notebook top (already there).
3. Per-file or per-batch, then **run `verify_ports.py` over the full set** (one
   batched container run) — expect 79/79 ports still byte-identical.
4. `ruff format` + `ruff check --fix` (Python half of `make format`); confirm no
   new lint. (No type-checker gate exists for the Python today — ruff only; the
   annotations are for readability/teaching + future `ty`/mypy adoption.)

## Open questions

- **Aggressiveness:** annotate *every* untyped local (all 262, including trivial
  `i = 0` cell constants), or skip the truly self-evident literals? Bill's ask
  reads as "as many as possible" → lean **all**, but confirm the trivial-literal
  cell vars are wanted (they're the bulk of the 172 cell-level ones).
- **int vs float judgement:** match runtime type precisely (preferred), or default
  ambiguous numeric exprs to `float`? (No runtime difference either way.)
- **Package (`figure.py`/`render.py`):** finish to a fully-annotated example
  (recommended — it's the API surface), or notebooks only?

## Relationship to other tasks

- Extends Part A of `tasks/archive/2026/06/11/notebook-structure-and-types.md`
  (params/returns + object-typed locals already done); same harness gate, same
  `from __future__ import annotations` convention.
- Independent of the other open tasks (slice-study, grouping, packaging, meson).
  Touches `notebooks/` broadly, so sequence it so it doesn't overlap a notebook
  reshuffle (`source-notebook-grouping.md`) if that runs.
