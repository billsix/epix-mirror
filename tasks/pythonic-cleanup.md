# Task: Pythonic cleanup pass of the ported Python code

**Status:** proposed — **effectively unblocked** (the port reached 80/81 on
2026-06-10; only `histogram` is left, hard-blocked by a missing data file, so the
binding surface is stable). Analyze + suggest first; only edit on Bill's explicit
go-ahead.
**Requested:** 2026-06-10 (Bill)
**Owner:** Bill (via Claude)

## Goal

Once the porting effort is finished, review the Python code and make it **more
Pythonic / readable** — clearer variable names, better idioms, consistent style —
**without changing any output.** Bill's prompting example: several variable names
are currently unclear *because they were deliberately kept matching the original
C++ source* during porting (e.g. `pt1`…`pt8`, `N1`/`N2`, `du`/`dv`, `db05`, `DF`).
That fidelity was the right call while porting; post-completion it can be improved.

**This extends to the *bindings'* public names, not just notebook-local variables
(Bill, 2026-06-10).** The binding exposes the C++ `P` type to Python *also* as `P`;
it could expose it as `Point` (and likewise rename free functions / `nb::arg`
names), then update every notebook's `from epix import P` + `P(...)` accordingly.
See the dedicated section below — the short answer is that this is **just as
byte-identity-safe** as renaming a local, because no Python identifier ever reaches
the eepic output.

## Why this is deferred (not done during porting)

During the port, **faithfulness to the C++ source — same names, same structure —
is a feature, not a bug**: it keeps the port→verify loop fast and makes the eepic
diff against the oracle obvious. Renaming/refactoring mid-port would add review
noise and risk for no immediate benefit. So this is a single, focused pass *after*
all demos are byte-identical and the binding surface is stable.

## Hard constraint: NO behavior change (harness-guarded)

Every notebook must still pass `build-aux/verify_ports.py NAME` (byte-identical
eepic) after the cleanup. This is a **pure readability refactor**; run the full
harness over every notebook when done.

- **Rename freely (locals *and* the public API); restructure math never.**
  Two categories with very different risk:
  - **Renaming identifiers is SAFE** — local variables, *and* the binding's
    exported names (`P`→`Point`, function names, `nb::arg` names). Names never
    appear in the eepic output, so a consistent rename can't change a figure (see
    the dedicated section below). The harness re-checks regardless.
  - **Restructuring math is RISKY** — per the float byte-identity gotcha in
    `CLAUDE.md` ("Byte-identity needs the same float *expression structure*"),
    "tidying" arithmetic (re-associating sums, folding constants, swapping
    `5*(pi/4)` for `5*pi/4`) can change rounding and silently break byte-identity.
    Touch names and idioms, **not** the numeric expressions.

## Renaming the bindings themselves (e.g. `P` → `Point`) — and why it's byte-identity-safe

**Bill's question (2026-06-10):** the C++ type is `P` and the binding also exposes
it as `P` (`nb::class_<P>(m, "P")` in `_epix.cc`); could it expose it as `Point`
instead, with every notebook updated from `P` to `Point`? **Does that affect the
bit-identical ports?**

**Answer: no — it does not affect byte-identity, and that's worth stating plainly.**
Byte-identity is a property of the **eepic figure text**, which is emitted by
**`libepix` (C++)**. That text is LaTeX `picture` macros — coordinates, colors,
label strings — and contains **no Python identifier of any kind**. The Python API
names (`P` vs `Point`, `epix.plot`, the `nb::arg` names) are pure call-site sugar
over the *same* `libepix` calls; renaming them changes which Python name you type,
not which C++ function runs or what eepic it emits. `verify_ports.py` execs the
(renamed) notebook to capture its eepic and diffs it against the C++ oracle — the
oracle is untouched, and the renamed notebook produces the **identical** eepic. So a
**consistent** rename across the binding + package + all notebooks is exactly as
safe as renaming a local variable, and the harness proves it the same way.

**Consequence:** the "safe rename" category is wider than the original task framed —
it includes the whole **public API surface** (type names, free-function names, arg
names), not just notebook locals. (The math-expression-structure constraint is
unchanged.)

**Mechanical scope of a type rename like `P` → `Point`:**
- `python/epix/_epix.cc`: `nb::class_<P>(m, "P")` → `…"Point")`, plus the `__repr__`
  string literal `"P("` → `"Point("` (cosmetic — not in any eepic, but keep it
  consistent). Rebuild with `make py-ext`.
- `python/epix/{figure,render,__init__}.py`: any `P` references.
- **every** `notebooks/*.py`: `from epix import P` → `Point`, and every `P(...)`.
- Re-verify the whole set with `verify_ports.py`, then `make format`.

**Still a deliberate naming *decision*, not automatic.** Trade-offs to weigh
(Bill's call, pedagogy-first):
- `P` matches the C++/ePiX and the math convention (P = point / ordered triple) and
  keeps the Python reading **close to the `.xp` samples** — which has teaching value
  (students can compare the Python and C++ side by side). `Point` is more
  self-documenting in isolation but drifts from the source vocabulary.
- If `P`→`Point`, consider the *whole* public API for the same treatment so the
  naming stays internally consistent (don't rename one type and leave the rest).
- This is exactly the **"choosing the final API shape / naming for the Python
  layer"** that `python-bindings-and-notebooks.md` listed as out-of-scope for the
  port — it now lives here, to be decided deliberately.

**Investigate (when this task runs):** enumerate the full public API the notebooks
touch (types: `P`, `Color`, `pair`, `domain`, `mesh`, `path`, `screen`, `axis`,
`legend`, `affine`, `Sphere`/`Circle`/`frame`, `Complex`, `scenery`, `data_file`/
`data_bins`, the `MarkType`/`LabelPos` enums; ~150 free functions), and propose a
naming scheme + the per-name before→after, for Bill to approve before any edit.

## Scope

- **`notebooks/*.py`** — the ported demos. The primary target: the C++-mirrored
  short/cryptic names and the little hand-rolled carrier classes
  (`MeshQuad`/`MeshElt` with classmethod constructors, etc.).
- **`python/epix/*.py`** — the package (`figure.py`, `render.py`,
  `__init__.py`) — already fairly Pythonic; review for consistency and naming.
- **The binding's public API names** (`python/epix/_epix.cc`) — type names
  (`P`→`Point`, …), free-function names, and `nb::arg(...)` names. Now explicitly
  in scope (see the "Renaming the bindings themselves" section) and byte-identity-
  safe under the harness. Decide the scheme deliberately and coordinate the arg
  names with `notebook-keyword-args.md`.

## Process (the workflow Bill asked for)

1. **Analyze.** Read all the ported notebooks (+ the package), and catalog the
   issues — unclear names, un-Pythonic idioms, inconsistencies — grouped by kind,
   with concrete before→after examples.
2. **Suggest.** Present the catalog (in this doc or a findings list) and **stop for
   Bill's confirmation.** Do not edit yet.
3. **On confirmation, apply.** Make the changes, **re-verify every notebook** with
   `verify_ports.py`, then `make format`. Prefer per-notebook (or per-group)
   commits-worth of changes so each stays reviewable and harness-checkable.

## What "more Pythonic" might cover (to analyze, not prescribe yet)

- **Clearer names:** `pt1`…`pt8` → corner/edge-meaningful names; `N1`/`N2` →
  `n_lat`/`n_long`; `du`/`dv`; `db05`/`db10`/… ; `DF` → `data`; cryptic loop-body
  locals. **But keep names that are already the clearest** — mathematical ones like
  `Phi`, `rho`, `orbital`, `psi`, `f`/`g` for the graphed function read better as
  the math than as prose.
- **Idioms:** comprehensions where natural (already used in some), `enumerate`,
  `@dataclass` for the small carrier classes instead of `__new__`/classmethod
  constructors, f-strings, etc.
- **Consistency across notebooks** so they teach a *uniform* style — Bill teaches
  from these, so readability is **pedagogical**, not just cosmetic. Names may want
  to optimize for matching the math notation in his lectures over generic Python
  conventions (confirm).

## Relationship to other tasks

- **`notebook-keyword-args.md`** is a closely-related readability task (keyword
  args for the unclear positional params). Both must preserve byte-identity and
  both aim at teaching-quality notebooks — **sequence or merge them** (likely:
  settle the keyword convention there, do naming/idioms here), and apply a single
  consistent style.
- **`python-bindings-and-notebooks.md`** is the blocker — this pass starts only
  once it's at 81/81 (or Bill calls the port "done enough").

## Open questions

- One big pass vs per-notebook? (Per-notebook keeps diffs reviewable and lets the
  harness gate each increment.)
- Pedagogy vs. generic Python conventions for naming — Bill's call (he's the
  teacher and these are lecture artifacts).
