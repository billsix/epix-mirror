# Task: Evaluate grouping the notebooks + source figures into a folder structure

**Status:** proposed — **DEFERRED until the port is complete.** Analyze + propose a
grouping first; reorganize only on Bill's go-ahead.
**Requested:** 2026-06-10 (Bill)
**Owner:** Bill (via Claude)

## Goal

Once porting is done, evaluate whether the Python notebooks (`notebooks/*.py`) and
the original figure sources (`samples/*.xp` / `*.flx`) should be **grouped into a
folder structure by topic/concept** — instead of the current flat ~80-file
directories — to make the collection easier to understand and navigate. Produce a
proposed grouping; on confirmation, apply it.

## Why deferred

Reorganizing mid-port would churn the paths the active loop depends on
(`verify_ports.py`, `make notebooks`) for no immediate benefit. Do it once the set
is stable (81/81), as a single deliberate pass.

## Key tension to settle FIRST: `samples/` is an upstream mirror

`samples/*.xp` / `*.flx` are upstream-authored; `CLAUDE.md` calls this a "faithful
mirror," and `samples/README` annotates every sample as the canonical learning
index. Physically reshuffling `samples/` diverges from upstream. Three options to
weigh with Bill before anything moves:

- **(a) Group only `notebooks/`** (our own artifacts); leave `samples/` flat —
  least disruptive, but the two dirs stop being parallel.
- **(b) Group both in parallel subfolders** — most legible, but reorganizes
  upstream files (mirror divergence; future-sync friction).
- **(c) Keep both flat, add a categorized INDEX** (a doc/README that groups them
  logically without moving files) — zero churn, no real folders, but no on-disk
  structure.

Recommend deciding (a)/(b)/(c) up front; given the mirror constraint, (a) or (c)
are the safer defaults.

## Candidate categories (to propose, not prescribe)

From the demo set, e.g.: intro/basics (hello, plotting), calculus (parabola,
calculus, riemann, uppersum, shadeplot, extract), 2-D geometry (objects, conic,
pascal, medians, symmetries), 3-D surfaces (sphere, torus, saddle, surface_shade,
butterfly, S2_harmonics, clipping, minkowski), vector fields / ODEs (vfield,
lorenz, slopefield, dipole, oscillator, pendulum), color (densityplot, color_sep),
projective (conic), layout / multi-panel (layout, layout2, wheel), animations
(cube, steiner, steiner2, house, riemann, pendulum), data (dataplot, histogram).
Cross-reference `samples/README`'s existing per-sample annotations.

## Mechanical consequences if files actually move (must handle)

- `build-aux/verify_ports.py` hardcodes `NB=notebooks`, `SAMPLES=samples` and
  `NAME.py`/`NAME.xp` — would need to walk subdirs / map name→path, and keep the
  notebook↔sample basename pairing working.
- `entrypoint/percentToIpynb.sh` (`make notebooks`), `Makefile`/`jupyter`,
  `meson.build` sample install lists, and `samples/README` cross-references.

## Process

1. **Analyze** the full set; propose a grouping **and** which of (a)/(b)/(c).
2. **Present** to Bill; **stop** for confirmation.
3. **On go-ahead:** move/restructure, fix every path consumer
   (`verify_ports.py` first), re-run the full harness, `make format`.

## Relationship to other tasks

- Deferred behind `python-bindings-and-notebooks.md` (81/81).
- There was a prior **`normalize-repo-structure.md`** (referenced in the bindings
  task — it set up the `include/epix/` layout; likely archived). Check it for
  precedent/intent before proposing demo-dir changes.
- Coordinate with `pythonic-cleanup.md` / `notebook-keyword-args.md` — all three
  touch `notebooks/`; sequence them so the churn doesn't overlap.
