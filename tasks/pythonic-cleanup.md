# Task: Pythonic cleanup pass of the ported Python code

**Status:** proposed — **DEFERRED until the bindings + 81-demo port
(`python-bindings-and-notebooks.md`) is complete.** Analyze + suggest first; only
edit on Bill's explicit go-ahead.
**Requested:** 2026-06-10 (Bill)
**Owner:** Bill (via Claude)

## Goal

Once the porting effort is finished, review the Python code and make it **more
Pythonic / readable** — clearer variable names, better idioms, consistent style —
**without changing any output.** Bill's prompting example: several variable names
are currently unclear *because they were deliberately kept matching the original
C++ source* during porting (e.g. `pt1`…`pt8`, `N1`/`N2`, `du`/`dv`, `db05`, `DF`).
That fidelity was the right call while porting; post-completion it can be improved.

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

- **Rename freely; restructure math never.** Per the float byte-identity gotcha in
  `CLAUDE.md` ("Byte-identity needs the same float *expression structure*"),
  renaming variables and reordering independent statements is safe, but "tidying"
  arithmetic — re-associating sums, folding constants, swapping `5*(pi/4)` for
  `5*pi/4` — can change rounding and silently break byte-identity. Touch names and
  idioms, not the numeric expressions.

## Scope

- **`notebooks/*.py`** — the ported demos. The primary target: the C++-mirrored
  short/cryptic names and the little hand-rolled carrier classes
  (`MeshQuad`/`MeshElt` with classmethod constructors, etc.).
- **`python/epix/*.py`** — the package (`figure.py`, `render.py`,
  `__init__.py`) — already fairly Pythonic; review for consistency and naming.
- **Binding arg names** (`nb::arg(...)` in `python/epix/_epix.cc`) — only if a name
  reads poorly at a notebook call site. That's an API-naming change → coordinate
  with `notebook-keyword-args.md`, decide deliberately.

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
