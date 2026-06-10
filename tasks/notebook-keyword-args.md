# Task: Use keyword arguments in the ported notebooks

**Status:** proposed — needs go-ahead
**Requested:** 2026-06-10 (Bill)
**Owner:** Bill (via Claude)

## Goal

Make the ported notebooks (`notebooks/*.py`) read as teaching material by using
**keyword arguments** for the non-obvious parameters, instead of bare positional
values. The nanobind bindings already define argument names (`nb::arg(...)` in
`python/epix/_epix.cc`), so this is mostly a notebook edit — the API is already
self-documenting, the notebooks just don't take advantage of it yet.

Example (current → proposed):

```python
epix.plot(f, epix.xmin(), epix.xmax(), 120)
epix.plot(f, epix.xmin(), epix.xmax(), n=120)          # the sample count is the unclear one

epix.figure(P(-2, 0), P(2, 4), "2.5x2.5in")
epix.figure(P(-2, 0), P(2, 4), size="2.5x2.5in")

epix.surface(f, R, 0)
epix.surface(f, R, cull=0)

epix.h_axis_labels(4, P(0, -4), epix.LabelPos.b)
epix.h_axis_labels(4, P(0, -4), align=epix.LabelPos.b)
```

## Guidance

- **Keyword the *unclear* params, keep the obvious ones positional.** A bare
  `P(0, 0)` point or `Black()` color is self-explanatory positionally; a trailing
  integer (sample count `n`), a boolean flag, an alignment enum, an offset, or a
  size string benefits from a name. Don't keyword everything — that's noise.
- **Use the binding's existing arg names** (`t_min`/`t_max`/`n`, `sw`/`ne`/`size`,
  `center`/`radius`, `align`, `cull`, `offset`, …). If a name reads poorly in a
  notebook, fix it at the binding (`nb::arg`) — but that's an API-naming change,
  still flagged out of scope in `python-bindings-and-notebooks.md`; decide names
  deliberately.
- **No behavior change.** Keyword args are purely cosmetic — every notebook must
  still pass `build-aux/verify_ports.py NAME` (byte-identical eepic). Run the
  harness over the whole set after editing.
- **Consistency.** Apply the same convention across all notebooks so they teach a
  uniform style.

## Scope

- All `notebooks/*.py` ports (13 as of 2026-06-10, growing as the port effort in
  `python-bindings-and-notebooks.md` continues). Ideally settle the convention
  now and apply it to new ports as they're written, so this doesn't become a
  large retroactive pass later.

## Open questions

- Settle the keyword-vs-positional line precisely (which arg *kinds* always get a
  name) before a bulk pass — list it here once decided.
- A few binding arg names may be worth renaming for notebook readability; collect
  those and decide together (API-naming, deliberate).
