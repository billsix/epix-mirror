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

---

# ANALYSIS — findings catalog (2026-06-11)

Read: `python/epix/{__init__,figure,render}.py`, `python/epix/_epix.cc` (full
binding surface), and all 81 `notebooks/*.py`. Below is the catalog grouped by
*kind*, with the byte-identity-safe vs. off-limits line drawn explicitly.

## A. Keyword-argument convention (the `notebook-keyword-args` half)

The binding already names its args (`nb::arg(...)`). Proposed rule: **name the
genuinely-unclear trailing scalar/flag/enum/string args; keep points & colors
positional.** Concretely, always keyword these (binding arg name in parens):

| Pattern (current) | → proposed | binding arg |
|---|---|---|
| `plot(f, a, b, 120)` / `shadeplot`/`polarplot`/`envelope`/`backplot_N`/`riemann_sum` trailing int | `n=120` | `n` |
| `figure(sw, ne, "2x2in")` / `picture(...)` / `show(...)` | `size="2x2in"` | `size` |
| `surface(f, R, 0)` / `scenery.draw(0)` | `cull=0` | `cull` |
| `label(at, off, txt, LabelPos.b)` / `dot`/`circ`/`*_axis_labels` | `align=LabelPos.b` | `align` |
| `label(at, P(0,-4), txt, ...)` offset point | `offset=P(0,-4)` | `offset` |
| `pen(col, 0.2)` / `pen(col,"0.1pt")` / `border`/`base` | `width=0.2` | `width` |
| `path(pts, True, True)` | `closed=True, filled=True` | `closed`,`filled` |
| `dart_field(F, R, 0.5)` / `vector_field` / `arrow(t,h,s)` | `scale=0.5` | `scale` |

**Open binding-name issue:** `grid(...)`, `mesh(...)`, `polar_grid(...)` use
`n1`/`n2`/`n3` — keywording them (`grid(n1=8, n2=8)`) is only marginally clearer.
Two choices (Bill's call, §decision 2): (a) leave grid/mesh counts positional, or
(b) rename the bindings `n1`/`n2`→`nx`/`ny` (a deliberate API change, byte-safe)
and then keyword them. **Note:** keyword args must use the *binding's* actual
names, not invented ones (`nx`/`ny` only work if we rename the binding first).

## B. Local-variable renames (the `pythonic-cleanup` half) — SAFE

Consistent cryptic→clear renames found across notebooks (rename the *binding/var*,
never the math expression it holds):

- **Screens/panels:** `scr1/scr2/scr3`, `tmp`, `my`, `scr` (artifacts, color_sep,
  coord_tricks, tori, trig, wheel, inverse, koch, layout, layout2, symmetries) →
  descriptive `screen_*` / `panel`. ~10 notebooks.
- **`DF` → `data`** (dataplot); **`db` → `bins`** (dataplot).
- **Lat/long counts `N1`/`N2`** (decorate, helicoid, lighting, log) → `n_lat`/`n_long`
  (matches the task's own example). `du`/`dv`/`dt` are standard differential
  notation — **keep**.
- **Throwaway `temp`/`a`/`tmp`** holding an intermediate point (butterfly, log,
  sqrt, stereo_proj, twisted_cubic) → `pt`/meaningful name (or inline).
- **`m_`-prefixed members** in the ported carrier classes (std_F `StdF`, lighting
  `Chip`/`Spot`/`Fog`) — drop the C++ `m_` prefix (`m_loc`→`loc`, etc.).
- **Geometric outcome names:** `q1/q2/q3`→`altitude_foot_*` (medians),
  `R1/R2/R3`→`intersection_*` (pascal), `hl1/hl2`→`hyperbolic_line_*` (minkowski),
  `V/W`→direction names (pascal).
- **`pt1..pt8`** (decorate, helicoid, log, lighting, medians) — corner/vertex
  points; either `corner1..` or geographic (`nw/ne/se/sw`) where the quad winding
  is known; **keep numbered if meaning is just "the i-th vertex."**

**KEEP (already the clearest — math/teaching names):** `f`/`g`/`F` (functions),
`x`/`y`/`z`/`u`/`v`/`t` (coords/params), `rho`/`theta`/`phi`/`Phi`/`psi`/`tau`
(Greek), `R` (region/domain), `E_1/E_2/E_3`, `du`/`dv`/`dt`, Legendre `P_n`, etc.

## C. Idioms — SAFE

- **Carrier classes → `@dataclass`:** `MeshQuad` (decorate), `MeshElt` (log),
  `Element` (helicoid), `Chip`/`Spot`/`Fog` (lighting), `StdF` (std_F) use
  `__new__`/classmethod or `m_`-fields. Where `__init__` is field-assignment +
  derived values, a `@dataclass` (+ `__post_init__` for the centroid/normal
  computations) reads better. Logic stays identical → output identical.
- **Named color channels:** `rgb(0.25 + 0.1*i, 0, -0.1*i)` → assign
  `red = 0.25 + 0.1*i` etc. then `rgb(red, green, blue)`. **Safe** — the
  subexpressions are unchanged, just bound to names. (steiner2, vfield, house.)
- Minor: f-strings already used; `enumerate` where a manual index is only used to
  index; remove unused `sqrt3` (levelset3).

## D. OFF-LIMITS — would break byte-identity (NOT doing; several agents suggested these)

- **Do NOT** convert float-accumulation loops to `sum(...)`/comprehensions
  (weierstrass, the Taylor loops in legend/plotting3, geomsum): `sum()`
  re-associates the additions → different rounding → eepic diff.
- **Do NOT** introduce `numpy.linspace` or any resampling (twisted_cubic) — changes
  the values *and* adds a dependency.
- **Do NOT** re-associate/fold any numeric expression, color math, or constant
  (the `5*(pi/4)` rule). Renaming the variable that holds it is fine; rewriting the
  expression is not.

## Decisions needed before editing (asked 2026-06-11)

1. **API type/function naming** — keep ePiX names (`P`, `Sin`, `RGB`, …) close to
   the `.xp` samples for side-by-side teaching, vs. rename to Pythonic
   (`Point`, …). Byte-safe either way; this is the pedagogy call.
2. **Keyword-arg scope** — name only the clearly-unclear trailing args (table A),
   vs. also rename grid/mesh `n1`/`n2`→`nx`/`ny` at the binding and keyword those.
3. **Carrier classes** — convert to `@dataclass` + drop `m_` prefixes, vs. leave
   the ported class shape as-is.
4. **Sequencing** — per-notebook increments (harness-gated each step;
   recommended), vs. one big pass then verify all.

## DECISIONS MADE (Bill, 2026-06-11)

1. **Rename the public API to Pythonic names** (P→Point, …).
2. **Also rename grid/mesh `n1`/`n2`/`n3` → `nx`/`ny`/`nz`** at the binding, then
   keyword them in notebooks.
3. **Convert carrier classes to `@dataclass` + drop `m_` prefixes.**
4. **One big pass, verify the whole suite at the end.**

## PROPOSED NAMING SCHEME (per-name map) — needs Bill's nod on the §Exceptions

**Types → CapWords** (Python class convention; the lowercase-named bound classes
get capitalized):

| current | → | current | → |
|---|---|---|---|
| `P` | `Point` | `screen` | `Screen` |
| `pair` | `Pair` | `axis` | `Axis` |
| `domain` | `Domain` | `legend` | `Legend` |
| `domain_list` | `DomainList` | `affine` | `Affine` |
| `mesh` | `Mesh` | `frame` | `Frame` |
| `path` | `Path` | `scenery` | `Scenery` |
| `data_file` | `DataFile` | `data_bins` | `DataBins` |

*Already CapWords — keep:* `Color`, `Camera`, `Complex`, `Sphere`, `Circle`,
`Segment`, `Plane`, and the enums `MarkType`/`LabelPos`/`IntegralType`.

**Action functions → snake_case** — already the case for ~95% (`picture`, `plot`,
`label`, `surface`, `clip_box`, `ode_plot`, `font_size`, …). Only a few need a touch:
- `Line` → `infinite_line` (disambiguates from `line`, which draws a segment)
- `J` → `quarter_turn`
- `rootC` → `root_complex`
- `Atan2` stays (paired with the trig exception below)

**Binding arg renames (decision 2):** `grid`/`mesh`/`polar_grid` `n1`/`n2`/`n3` →
`nx`/`ny`/`nz`.

### §Exceptions — names that should STAY Capitalized (the one point to confirm)

ePiX uses a **Capital = returns-a-value-factory / lowercase = sets-pen-state**
convention that a blind lowercase pass would collide with and erase:

- **Color factories** `RGB`, `Black`, `White`, `Red`, `Green`, `Blue`, `Yellow`,
  `Cyan`, `Magenta`, `Neutral`, `RGB_Neutral`, `CMY_Neutral`, `CMYK_Neutral`,
  `C_Process`, `M_Process`, `Y_Process`, `K_Process` — each **returns a `Color`**.
  There are ALREADY lowercase `rgb()`, `red()`, `blue()`, `green()`, `yellow()`,
  `white()`, `black()`, `magenta()`, `gray()` that **set the pen** (return void).
  `Red()` (a color) vs `red()` (set pen red) is a real, used distinction —
  lowercasing the factories collides with the setters.
- **ePiX trig function-objects** `Sin`, `Cos`, `Tan`, `Atan`, `Atan2`, `Asin`,
  `Acos` — composable ePiX objects passed to `plot()`, distinct from `math.sin`
  (which the notebooks also import). Lowercasing collides with `math` and blurs
  "ePiX function-object" vs "scalar math".

**Recommended:** keep these Capitalized as deliberate, documented value-factories
(like Python's own `Path`, `Decimal` factories) — it preserves the semantic split
and avoids collisions, while everything *else* goes Pythonic. The alternative
(force-lowercase) requires renaming the pen-setters to `set_red()`/`set_rgb()` etc.
to free the names — more churn and arguably less clear. **Confirm which.**

**Bill chose: force everything lowercase.** Resolution applied:
- Color factories take the lowercase names (`Red`→`red`, `RGB`→`rgb`, …); the
  pen-setters become `set_red()`/`set_rgb()`/…/`set_gray()`.
- ePiX trig lowercased (`Sin`→`sin`, …) — **no alias needed**: every notebook calls
  it namespaced (`epix.sin`) and none do `from epix import *`, so there's no clash
  with `math.sin` (verified: 0 star-imports).

## EXECUTION LOG

### Stage 1 — mechanical API rename (DONE 2026-06-11, verified byte-identical)

Pure rename of the public API across `_epix.cc` + package + all 81 notebooks via a
scripted pass (`/tmp/rename_api.py`): types→CapWords, functions→snake_case,
color factories→lowercase + setters→`set_*`, trig lowercased, `Line`→`infinite_line`,
`J`→`quarter_turn`, `rootC`→`root_complex`, binding args `n1/n2/n3`→`nx/ny/nz`.
**Full harness: 79/79 PASS.** Three bugs found+fixed during this stage (keep as
lessons for any future mass-rename):
1. **Color factories are registered in a loop** (`{"Black",&Black},…`), not via
   `m.def("Black"…)`, so a `m.def("name"`-targeted rename misses them. Fixed the
   pair-list strings directly.
2. **Multi-line `m.def(\n "name",` forms** (only `rootC`) are missed by a
   single-line `m.def("name"` match. Fixed directly.
3. **`P(`→`Point(` over-matched inside LaTeX label strings** — the probability
   `$P(|Z|>1.96)$` (R_demo) and Legendre `$P(z)$` (S2_harmonics) are *not* the
   `Point` type. A bare `P(` regex hits string literals → eepic mismatch. Reverted
   those 3 in-string occurrences. **Lesson: a bare-identifier rename must exclude
   string-literal contents.**

### Stage 2 — judgment edits (keyword args + local renames + @dataclass) — DONE (2026-06-11)

Per-notebook, notebook-only (binding arg names already `nx/ny`). Applied across all
81 notebooks: keyword args for the unclear trailing params (`n=`, `size=`, `cull=`,
`offset=`/`text=`/`align=`, `width=`, `nx=`/`ny=`/`nz=`, `scale=`, `thickness=`,
`radius=`, `coarse=`/`fine=`); local renames (screens→`panel`/`screen_*`, `DF`→`data`,
`db`→`bins`, `N1`/`N2`→`n_lat`/`n_long`/`n_merid`, `q1..`→`altitude_foot_*`,
`R1..`→`intersection_*`, `V`/`W`→`v_dir`/`w_dir`, `hl*`→`hyperbolic_line_*`,
`A1/A2/A3`→`sigma/rho/beta`, `tmp`/`my`/`scr`→meaningful, `m_`-prefixes dropped);
and the 7 carrier classes (`MeshQuad`, `Element`, `Chip`/`Spot`/`Fog`, `MeshElt`,
`StdF`) converted to `@dataclass` with `__post_init__` for derived values.
Math names kept (`f`/`g`/`F`, coords, Greek, `R`, `du`/`dv`/`dt`, `P_n`, `k`).
**Full harness: 79/79 PASS — byte-identical.** (One straggler, `symmetries.py` dot,
keyworded after for style consistency; re-verified.)

## STATUS: DONE (2026-06-11)

Pythonic-cleanup + keyword-args both complete and harness-verified byte-identical.

**Known small follow-ups (not blocking):**
- `CLAUDE.md` still references the *old* binding names in a couple of gotchas
  (`nb::class_<P>(m, "P")`, color-function notes) — update to the renamed API.
- Deeper structural work (extract-to-helpers, more Python idioms, type hints) is its
  own task: see `tasks/notebook-structure-and-types.md`.
