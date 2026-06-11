# Task: Feasibility — Python bindings + percent-format notebooks for ePiX

**Status:** DONE (2026-06-11) — port effectively complete at 80/81; `histogram` won't-fix (blocked by a data file the repo doesn't ship). Re-verified byte-exact 2026-06-11. Follow-ups split into their own task docs.
**Requested:** 2026-06-09 (Bill)
**Owner:** Bill (via Claude)

## Closeout (2026-06-11)

Closed as **effectively complete**. Final state:

- **All 79 byte-comparable port notebooks PASS** — re-verified 2026-06-11 with a
  fresh `make image` + `make build` + `verify_ports.py` over every notebook in the
  container; each reproduces its C++ oracle's eepic byte-for-byte.
- The 2 non-PASS notebooks are **not ports** and have no oracle to diff against,
  so they're outside the byte-exact suite (unchanged, expected):
  - `build.py` — Phase-2 "build a figure in Python" tutorial; there is no
    `samples/build.xp`.
  - `hello.py` — Phase-1 *inline-display* demo (`epix.show(...)`); never builds a
    scene, so defines neither `fig` nor `anim`.
- **`histogram` — WON'T-FIX (hard-blocked):** reads `samples/binom.dat`, which the
  repo doesn't ship; the C++ oracle can't run either, so there's nothing to verify
  against. Reopen only if `binom.dat` is ever generated/committed.

**Follow-ups (now unblocked, tracked separately):** `tasks/pythonic-cleanup.md`,
`tasks/notebook-keyword-args.md`, `tasks/source-notebook-grouping.md`,
`tasks/distro-packaging.md`, plus the ASan dev-tooling teardown (mirror the
batch-10/11 bindings into `build-aux/asan_smoke.cc`, run `make asan`, then remove
the dev-only ASan tooling per the Dockerfile/CLAUDE.md cleanup notes).

## Phase 0 — demo API audit (DONE 2026-06-09): the nanobind bind-list

**Method (accurate, not grep):** compiled each demo to an object file
(`g++ -std=c++20 -I/usr/local/include -x c++ -c`) and read its **undefined
`ePiX::` symbols** (`nm -uC`) — i.e. exactly what each demo *links* from
`libepix`. Aggregated across **105 of 108** demos (the 3 misses —
`lighting.flx`, `riemann.flx`, `stereo_proj.flx` — need `-I.`/`-D` and use the
same API).

**Result: 331 unique `ePiX` symbols = ~156 free functions + ~242 method symbols
across ~22 classes.** That is the bind-list.

**Classes to wrap (unique method *names*; sizes each wrapper):**
`screen` 16 · `legend` 11 · `path` 10 · `affine` 9 · `scenery` 8 · `domain` 8 ·
`axis` 8 · `Camera` 7 · `P` 6 · `data_file` 5 · `data_bins` 5 · `mesh` 3 ·
`Segment` 3 · `Plane` 3 · `Deriv` 3 · `frame` 2 · `Sphere` 2 · `Integral` 2 ·
`Color` 2 · `Circle` 2 · `Complex`/`pair`/`domain_list` 1.
(By call-volume the hot ones are `P`, `Color`, `domain`, `Camera`, `screen`,
`legend`, `mesh`, `axis`, `path`, `scenery`. `std_F` appears but is a *sample's
own* helper, not ePiX — exclude.)

**Top free functions (by # demos):** `picture` / `begin` / `end_picture`
(all 105) · `bold` 57 · `plot` 54 · `label` 46 · `Black` 44 · `fill` 41 ·
`pen` 36 · `plain` 35 · `Sin`/`Red` 32 · `pst_format` 31 · `line` 29 ·
`xmax`/`Blue` 26 · `grid` 25 · `xmin`/`revolutions`/`Cos` 24 · then the color
constructors (`RGB`/`Green`/`White`/`Yellow`/`rgb`/…), axes (`h_axis`/`v_axis`),
`surface`, `arrow`, `font_size`, `clip_box`, `inset`, `activate`/`deactivate`, …

**Priority tiers (of the 331 symbols, = MVP→long-tail):**
- **Core — 34 symbols used by ≥20 demos.** Binding these (P, Color + the basic
  color fns, picture/begin/end, plot, label, line, pen/fill/bold/plain, Sin/Cos,
  xmin/xmax/…, grid, axes) already renders a large fraction of demos.
- **Mid — 77 symbols (5–19 demos).** surfaces/mesh, legend, clipping, domains.
- **Tail — 108 (2–4 demos) + 112 (1 demo only).** bind on demand as those
  specific demos are ported.

**Caveats:** `nm -u` captures the *non-inline* (libepix-compiled) surface; a few
header-only/inline helpers and operator overloads won't appear and must be
added when a demo needs them. The 3 uncompiled `.flx` add nothing new in
principle. Raw data was in `/tmp/audit/allsyms.txt` (regenerate with the method
above).

**Implication for nanobind:** ~22 classes + ~156 free functions is a **bounded,
known** surface — bindable, not open-ended. Build **core-tier first** (renders
most demos), then widen by tier as demos are ported. Next: the notebook +
`_repr_png_` render-helper MVP (Phase 1).

## Question to answer

How feasible is it to (a) give ePiX **Python bindings** so you write a figure in
Python the way the `.xp` example programs are written in C++, and (b) build
**percent-format notebooks** that render those figures inline — the
jupytext/`# %%` + display-helper + containerized-jupyter pattern Bill already
uses in his geometric-algebra project? This task is the feasibility read, not
the implementation.

## Target scope (clarified by Bill)

The end goal is **every existing demo ported to a notebook** — all **~70
`samples/*.xp` + 11 `samples/*.flx`** (≈81 demos), each as a percent-format
notebook that renders inline — written in **natural, Pythonic function syntax**
(not a thin transliteration of C++). So this is **not** a core-subset MVP: the
full demo set is both the **coverage spec** (the bindings must cover the union
of API features the demos use) and the **test oracle** (each ported notebook
must reproduce its original figure).

**This is purely ADDITIVE (clarified by Bill 2026-06-09).** Nothing is removed
or migrated: the existing `samples/*.xp` / `.flx` and the whole C++/eepic
workflow stay exactly as they are. The Python notebooks are net-new artifacts
*alongside* them. Consequence: the C++ samples + their rendered figures are the
**permanent reference oracle** each Python notebook is checked against — and the
bindings must **not** require any breaking change to the C++ library or `.xp`
API (e.g. no `enum class` prerequisite; if the Python layer wants scoped enums,
that is defined in the binding layer, not by changing the C++ enums). See the
corrected enum note under "Relationship to other tasks."

**Implications:**
- **API coverage is dictated by the demos, not chosen.** The samples exercise a
  wide swath — points/vectors, colors, paths/curves, `plot`/surfaces, clipping &
  cropping, page layout, axes/legends, markers, affine maps, intersections, and
  the `tix()`-driven animation model for `.flx`. **First step of any
  implementation: audit which API surface the 81 demos collectively touch** —
  that bounds the binding job (likely most of the public API, but the audit
  turns "hundreds of functions" into a concrete, demo-justified list).
- **"Natural Python syntax."** ePiX's `.xp` style is already procedural — free
  functions called between `begin()`/`end()` — which maps cleanly to Python
  functions inside a `Figure` context manager. Demos that use C++ `for` loops /
  local variables / `-D` preprocessor switches (e.g. `polyhedra.xp`,
  `S2_harmonics.xp`) become ordinary Python loops / variables / function
  parameters — a *natural* port, often near-line-by-line, not a literal one.
- **Animations.** The 11 `.flx` demos drive frames via the global `tix()`
  counter; in Python they become a loop over frames producing an animation
  (inline via the notebook, or mng/gif as today).

## Verdict up front

- **Notebook + inline-render layer: HIGH feasibility.** Well-trodden pattern,
  and the rendering toolchain already exists in the repo (the `elaps` pipeline:
  eepic → latex → dvips → ps → pdf/png).
- **Python bindings: MEDIUM feasibility — no fundamental blocker, real scope.**
  The risks are surface area and the global-state model, not impossibility.
  Prior art exists: the README documents **Pyepix**, an independent Python
  implementation of ePiX 1.0.4 — proof a Python ePiX is viable (though it was a
  *reimplementation* of an old version, now stale, not bindings).

## What makes bindings non-trivial (from investigation)

- **Large API surface.** ~28 public classes and **hundreds of free functions**
  (216 in just 5 of the ~33 umbrella headers). Binding *everything* is a big,
  mechanical job; binding a **core subset first** is the pragmatic path.
- **Global-state drawing model.** Figures are built by mutating global state
  (current pen/color/picture) between `begin()` and `end()` — not by methods on
  an object you pass around. A Python binding needs a thin **session/context
  shim** (e.g. a `Figure` context manager that calls `begin()`/`end()`); it
  means one figure at a time, which is fine for notebooks.
- **Output capture is clean, though.** `picture` already has `void end()`,
  `write()`, a settable output **format** (eepic/pstricks/tikz), and "write in
  specified format to a **named output file**" (`picture.h`). So a binding sets
  a temp output path, builds the scene, calls `end()`, and reads the file back —
  no stdout-hijacking gymnastics required.

## Three architecture options (recommendation first)

**A — Real C++ bindings over `libepix` (recommended for fidelity).**
pybind11 or nanobind wrapping the existing library. Pros: one source of truth,
identical output, "real" bindings. Cons: must wrap the global-state model behind
a Python `Figure`/context shim, and the surface is large → **scope to a core
subset first** (P/vectors, Color, picture+begin/end, label, line/path, one
`plot`, markers, axis), expand incrementally. Build: Meson already has
first-class Python-module/pybind11 support, so it slots into the current build.

**B — Codegen / emit wrapper (faster MVP, less "binding").**
A pure-Python API that builds the scene and **emits eepic directly** (or emits a
`.xp` and shells out to `epix`/`elaps`). Pros: decoupled from the C++ ABI,
fastest to a working demo, "works like the example programs" literally (it
*is* generating them). Cons: not true bindings; logic duplicated from C++.

**C — Pure-Python reimplementation (à la Pyepix).** Most work, no C++ dependency,
drifts from the library. **Not recommended** (Pyepix already shows the cost, and
it went stale).

**DECIDED (Bill, 2026-06-09): Approach A — real C++ bindings over `libepix`,
using nanobind.** (B and C are off the table; kept above only as rationale.)
Choosing nanobind implies A — it's a C++↔Python binding generator, so the
codegen/emit path doesn't apply. nanobind needs ≥C++17 and the library is now
pinned at **C++20**, so it's a clean fit; it's leaner/faster-to-build than
pybind11, which matters given the large API surface. Phase 0 (the demo API
audit) still comes first — it now scopes *how much of `libepix` to bind*, not
the A/B choice.

## The notebook + inline-render layer (independent of A/B/C)

This is the high-feasibility half and mirrors the pattern Bill uses elsewhere:

- **Notebooks as jupytext percent files** — `# ---` jupytext header + `# %%`
  cells, kept as `.py` in a `notebooks/` dir, converted to `.ipynb` by a small
  `percentToIpynb` script (the existing pattern). Diff-friendly, git-friendly.
- **Inline figure display.** Wrap the Python figure object with a `_repr_png_`
  / `_repr_svg_` (analogous to the `_repr_latex_` Bill's GA objects expose) that
  renders the figure to an image via the **existing `elaps` pipeline**
  (eepic → latex → dvips → ps → png/svg) and returns the bytes. The renderer is
  the one genuinely new piece; it reuses logic the repo already ships.
- **Packaging + container.** A `pyproject.toml` with `notebooks` / `jupyter`
  optional extras, and reuse of the **`Makefile` container pattern** — a
  `jupyter` target + `jupyter.sh`/`percentToIpynb.sh` entrypoints — so the whole
  thing runs in the pinned image that already has the TeX/ghostscript toolchain.

## Key risks / unknowns

1. **API coverage scope** — effort scales with how much of the 200+ functions
   you bind. Mitigation: core-subset MVP, expand on demand.
2. **Global-state ergonomics** — making `begin()/end()` feel Pythonic (a context
   manager) without surprising users; thread-safety is out (one figure at a
   time).
3. **Inline-render latency** — each figure runs `latex` (seconds). Fine for
   notebooks; cache rendered images.
4. **Binding drift** — bindings must track the C++ API as it changes; a codegen
   step that reads the headers could help but is its own project.
5. **Build coupling** — nanobind + Meson + a Python interpreter dependency added
   to the image (nanobind itself is small/header-light; Meson has nanobind
   support via `dependency('nanobind')` / the nanobind module).

## Rough effort

Porting all 81 demos makes this a **multi-stage program, not one task**:

- **Phase 0 — demo API audit** (which functions/classes the 81 demos use):
  **~0.5 session.** Bounds everything below.
- **Notebook + render-helper MVP** (reuse `elaps`, container scaffolding,
  `_repr_png_`): **~1–2 sessions.**
- **nanobind bindings to cover the audited surface** + the `Figure`/`tix` shims:
  **several sessions** (scales with the audit; the demos need broad coverage,
  not a core subset).
- **Porting the 81 demos** to notebooks + verifying each renders to its original
  figure: **the bulk of the work**, parallelizable per demo once the API +
  render helper exist.

Net: a working *pipeline* (bindings/emit + render + one ported notebook) is
**~3–4 sessions**; the full 81-demo port is a sustained effort on top.

## Decisions

1. **Binding approach** — ✅ **A, real C++ bindings over `libepix`** (Bill, 2026-06-09).
2. **Binding tech** — ✅ **nanobind** (Bill, 2026-06-09).
3. **Scope** — *open.* Core-subset MVP first (recommended) vs aim broad. (The
   all-81-demos goal means it grows to the full demo-audited surface regardless;
   the question is just the starting increment.)
4. **Inline format** — *open.* PNG (simple raster) vs SVG (vector).
5. **Container/jupyter scaffolding** — *open.* Mirror the `Makefile` `jupyter`
   target + jupytext flow now, or just a local `pyproject` first?

## Relationship to other tasks

`modernize-cxx-standard.md` is now **complete** (archived), which resolves the
couplings that were open when this was written:

- **C++ standard — RESOLVED.** Pinned at **C++20** ≥ nanobind's C++17 floor;
  the binding module compiles at C++20 too. (This is partly *why* nanobind was
  chosen.)
- **`enum class` — RESOLVED + DONE** (in the modernize task): the C++ enums are
  already scoped. So the Python layer's scoped enums (`LabelPos.t`-style) line
  up naturally with the C++ side, and nothing here needs to touch them.
- **Approach A vs B and `enum class` (corrected 2026-06-09).** Because the port
  is **additive** — the C++ `.xp` samples stay and must keep compiling
  unchanged — a breaking C++ API change like `enum class` is **NOT** something
  the Python work can "absorb" (the earlier reasoning that the all-demos rewrite
  would absorb it is void). Under **A (real bindings)** the Python layer simply
  exposes its own scoped enums (`LabelPos.t`-style) without touching the C++
  `enum`; under **B (codegen/emit)** the C++ side isn't linked at all. Either
  way, **bindings do not require — and should not trigger — the C++ enum-class
  change.** `enum class` is now a fully separate, opt-in, compat-preserving
  decision (see `modernize-cxx-standard.md`), decoupled from this task.
- **Approach A retires a modernization gotcha.** Python users never invoke
  `g++`, so the runtime `-std` coupling the modernization task calls out
  disappears for the Python path under A (it remains under B, which still shells
  to `epix`).
- **Shared audit + locked oracle.** Phase 0 (the demo API audit) is the same
  public-API inventory the modernization task needs — share it. And the
  per-demo render verification assumes a **frozen** output, so let the
  modernization output-identical work settle first.
- Minor: `normalize-repo-structure.md`'s `include/epix/` layout makes the
  bindings' includes/packaging cleaner — structure-first is mildly favorable.

**Forward plan (modernize done; A + nanobind chosen).** Phases, single scheme:
- **Phase 0 — demo API audit** ✅ DONE (2026-06-09): the `libepix` surface the 81
  demos use → the concrete nanobind bind-list (see "Phase 0" section above).
- **Phase 1 — notebook + render-helper MVP** ✅ DONE (2026-06-09). `python/epix/`
  package (`Figure` with `_repr_png_`; `render_xp`/`show` shell to the installed
  `elaps` → eps → ghostscript png, in a scratch dir). `python/pyproject.toml`
  (extras: `notebooks`, `jupyter`). `notebooks/hello.py` (jupytext percent).
  `entrypoint/jupyter.sh` + `percentToIpynb.sh`; Dockerfile bakes
  jupyterlab/jupytext/ipython; `make jupyter` (port 8888) + `make notebooks`.
  **Verified:** `epix.show("samples/hello.xp")` → valid 318×152 PNG;
  `make notebooks` converts to `.ipynb`; JupyterLab launches at :8888.
  (Decisions taken: inline = **PNG** for now, SVG an easy follow-on; container
  jupyter scaffolding mirrored. The interactive UI is Bill-verified per the
  build-vs-runtime split.) Sibling-`#include` samples (`-I.`) + raw-eepic input
  (Phase 2 output) are small follow-ons in the render helper.
- **Phase 2 — nanobind bindings** — first increment ✅ DONE (2026-06-09):
  `python/epix/_epix.cc` (nanobind module: `P`, `picture`, `begin`, `font_size`,
  `label`, `print_eepic`); `python/epix/figure.py` (Pythonic scene API +
  `render()` + a `figure()` context manager; captures via `print_eepic` and
  renders through the Phase-1 helper, which now also takes `.eepic`). Built
  standalone (`entrypoint/build_py.sh` + `make py-ext`: g++ compiles the binding
  with nanobind's `nb_combined.cpp`, statically linking libepix); Dockerfile adds
  `python3-devel` + nanobind. **Verified:** Python builds the `hello` scene →
  eepic **byte-identical to the C++ baseline**, renders to PNG, **8/8 stable
  runs**.
  - **Latent libepix bug found + fixed (ASan):** `screen::screen()` left the
    pimpl pointer `m_screen` **uninitialized**; `picture()` does
    `the_canvas = screen(...)` → `delete m_screen` on garbage. It only "worked"
    because the canvas lives in zero-initialized static storage (`m_screen` →
    `nullptr`, `delete nullptr` safe); the binding context didn't grant that, so
    it segfaulted nondeterministically. Repro'd deterministically in plain C++
    under ASan; **pre-existing** (the old `screen::screen() {}` was equally
    uninitialized). Fix: default member initializer `screen_data* m_screen =
    nullptr;` in `screen.h`. Independent libepix bugfix, worth keeping.
  - **Core tier bound ✅ (2026-06-09):** expanded `_epix.cc` to the core surface
    — `P` arithmetic (`+ - * ^` cross / `dot` / `norm`), `Color` + the color
    constructors (`RGB`/`Black`/`Red`/`Green`/`Blue`/`Yellow`/`Cyan`/`Magenta`),
    `pen`/`fill`/`nofill`/`bold`/`plain`, `line`, `grid`, `h_axis`/`v_axis`,
    `xmin`/`xmax`/`ymin`/`ymax`/`tix`, `Sin`/`Cos`, and **`plot`**. `plot` takes a
    raw C function pointer, so it's bound via a **trampoline** (module-global
    Python callable + a `P`/`double` probe to pick the parametric vs graph
    overload) — accepts an ordinary Python lambda. `__init__.py` now does
    `from ._epix import *`, so the scene API scales automatically. **Verified:** a
    Python parabola (`plot(lambda t: P(t, t*t), …)` + axes + colors) renders to a
    PNG and its eepic is **byte-identical to the equivalent C++**, 5/5 stable.
    `notebooks/build.py` demos building figures in Python.
  - **Mid-tier 2D batch bound ✅ (2026-06-09):** the scoped enums
    `epix_mark_type` → `MarkType` and `epix_label_posn` → `LabelPos` (nanobind
    `enum_`); markers (`marker`, `dot`, `box`, `arrow`); shapes (`circle`,
    `rect`, `triangle`, `ellipse`, `dart`); the aligned `label(at, offset, text,
    align)` overload; angle modes (`radians`/`degrees`/`revolutions`); 3-D
    `viewpoint`; line styles (`solid`/`dashed`/`dotted`/`line_style`); clipping
    (`clip_box`/`clip`/`set_crop`/`crop`). **Verified:** a Python scene using
    circle/marker(enum)/arrow/rect/aligned-label renders, eepic **byte-identical
    to C++**, 5/5 stable, **ASan-clean** (no new uninitialized-pimpl bugs).
    **Pimpl-bug audit done:** of the raw-pointer pimpls, only `screen` had the
    uninitialized-default-ctor bug; `Color`/`picture_data`/`path` all initialize
    their pointers in the default ctor — no siblings to fix.
  - **3-D parametric surface bound ✅ (2026-06-09):** `mesh` + `domain` value
    classes and `surface(F(u,v) -> P, domain, cull=0)` via a **2-variable
    trampoline** (same idea as `plot`). **Verified:** a Python saddle
    `surface(lambda u, v: P(u, v, u*u - v*v), domain(...))` + `viewpoint`
    renders the mesh, eepic **byte-identical to C++**, 5/5 stable, ASan-clean.
  - **ASan tooling (DEV-ONLY — must be removed in the final cleanup phase):**
    `libasan` added to the image, `build-aux/asan_smoke.cc` exercises the bound
    libepix surface, `entrypoint/asan_check.sh` + **`make asan`** build an
    AddressSanitizer libepix and run the smoke test in-container. This exists
    only to catch latent libepix memory bugs (like the `screen` one) while the
    bindings are written. **Final-phase cleanup checklist:** drop `libasan` from
    the Dockerfile, delete `entrypoint/asan_check.sh` + `build-aux/asan_smoke.cc`,
    remove the `asan` Makefile target. (Keep mirroring new bindings into
    `asan_smoke.cc` until then.)
  - **`axis` class + `bold/bbold/plain(Color)` bound ✅ (2026-06-09):** the
    annotated `axis` class — ctor + `draw()` + chained `unmark`/`frac`/`trig`/
    `sci`/`log`/`align`/`align_labels`/`subdivide`/`precision`/`tick_ratio`
    (returning `axis&`, bound `rv_policy::reference`). **Caution learned:** bind
    only methods that are *defined* in libepix — `axis::dec()` is declared but
    has no definition, and an unresolved symbol breaks the whole module at import
    (`nm -C libepix.a | grep ' T '` to check). Driven by the first real demo
    port (below) — i.e. **bind-on-demand**, the intended workflow.
  - **`legend` class + axis factories + Deriv/Integral bound ✅ (2026-06-09):**
    `top_axis`/`bottom_axis`/`left_axis`/`right_axis` (return a chainable `axis`);
    `axis::draw_labels`/`draw_ticks`; the `legend` class (`path_item`/`fill_item`/
    `mark_item`/`border`/`item_border`/`backing`/`draw`, chained); and
    `plot_deriv`/`plot_integral` (the `Deriv`/`Integral` function-wrappers via the
    scalar trampoline — `plot_deriv(f, …)`, `plot_integral(f, x0, …)`). All
    driven by the calculus port below.
  - **3-D camera + sphere batch bound ✅ (2026-06-09):** the global `camera`
    object + `Camera` class (`at`/`look_at`/`range`), `sphere()`,
    `latitude`/`longitude`, and `backplot_N`/`frontplot_N` (hidden/visible
    plotting — each takes **two** scalar functions, so a two-function trampoline).
    Driven by the sphere port.
  - **Remaining for Phase 2 (bind on demand during porting):** `surface` over
    `domain_list` + the 3-arg `F(x,y,z)` form, more `Camera` controls, data
    plotting (`data_file`/`data_bins`), the geometric-object *classes*
    (Circle/Sphere/Plane/Segment + intersections), and the `tix()`-driven
    **animation** flow for `.flx`. ASan-check each (`make asan`).
- **Phase 3 — port the 81 demos** to notebooks, each verified against the locked
  render oracle (output frozen by the completed modernize work).
  - **First faithful port ✅ (2026-06-09): `parabola.xp` → `notebooks/parabola.py`.**
    The Python notebook's eepic is **byte-identical** to the original sample
    (`epix samples/parabola.xp`), 3/3 stable. Natural Python form: C++
    `double f(double)` → a plain Python function; `using enum` → `epix.LabelPos.b`;
    axis-method chaining preserved (`Ay.unmark(0).draw()`). Proves the whole
    pipeline (bindings → build-in-Python → render → oracle-match) end to end. The
    bind-on-demand loop (port → hit a missing symbol → bind it → verify) works.
  - **Second port ✅ (2026-06-09): `calculus.xp` → `notebooks/calculus.py`** —
    **byte-identical**, 3/3 stable. A meatier demo: boxed `legend`, trig-labelled
    axis factories, and numerical `Deriv`/`Integral` plots. Confirms the workflow
    scales past the trivial case.
  - **Third port ✅ (2026-06-09): `sphere.xp` → `notebooks/sphere.py`** —
    **byte-identical**, 3/3 stable, 778 eepic lines. A full 3-D demo: loxodromes
    with hidden/visible plotting, lat/long grid, perspective `camera`. Unlocks the
    3-D demo class. (Gotcha to remember in ports: `epix.Cos`/`Sin` honour
    `degrees()` mode — use them, not `math.cos`.)
  - **Fourth port ✅ — the animation mechanism (2026-06-09): `cube.flx` →
    `notebooks/cube.py`.** First `.flx`: a rotating wireframe cube + angle
    "clock". `epix.animate(build, count=N)` renders N frames (each with `tix()`
    set to `i/N`) and assembles a looping gif (`_repr_html_` shows it inline).
    All **4 test frames byte-identical** to the C++ original.
    - **Key finding — in-process state accumulation:** ePiX accumulates
      per-render global state with **no public reset** — notably the color
      palette (`picture_data::m_palette`, a private `std::set<Color>`), so a naive
      in-process frame loop leaks state (frame N's eepic carries earlier frames'
      colors/paths). This is *why* `flix` runs a fresh process per frame.
      `animate()` does the same: it **`fork()`s per frame**, building the scene +
      `print_eepic` in the child (fresh copy of the parent's clean state), then
      the parent renders each eepic→png. Result: byte-identical frames. *(Caveat:
      `fork()` inside a threaded Jupyter kernel is a runtime detail Bill should
      sanity-check; the mechanism itself is proven.)*
    - Bindings added on demand: coordinate ctors (`xyz`/`polar`/`sph`/`cyl`),
      `label_angle`, `red(d)`, 2-arg `ellipse`, `plot(f(x,y,z), domain)` (3-var
      trampoline), `set_tix`.
  - **Fifth port ✅ (2026-06-09): `conic.xp` → `notebooks/conic.py`** —
    **byte-identical** (314 lines), 3/3 stable. Projective parabola/circle: the
    tangent-line `envelope`, the line at infinity, a pulled-back camera. Bindings
    added on demand: `border()`, `pen(Color, width-string)`, region
    `grid(P, P, n1, n2)`, `envelope` (P-valued trampoline), `Line`, `masklabel`.
  - **Verification harness ✅ `build-aux/verify_ports.py`:** execs a notebook
    (percent `.py` is valid Python), grabs its `fig`/`anim`, and diffs the eepic
    against the sample compiled with **eepic forced** (`-DEPIX_FMT_EEPIC`, so
    in-file `pst_format()`/`tikz_format()` don't change the oracle backend) — for
    `.flx`, per frame with `(i, count)` argv. Run one name per process (libepix
    accumulates state). `PASS`/`FAIL` per demo.
  - **Batch 1 ✅ (2026-06-09), 8 demos:** `plotting`, `lissajous`, `golden_rect`,
    `polar`, `semicirc`, `slopefield`, `plotting2`, `objects` — all
    byte-identical. Bound on demand: color setters (`black`/`blue`/`green`/`rgb`),
    `arrow_inset`/`arrow_width`, `picture(w,h)`/`bounding_box`/`unitlength`/
    `pst_format`, `arc`/`polar_grid`, `h_axis_labels`/`v_axis_labels`, `xsize`/
    `ysize`, `Atan`/`Atan2`, and the field/polar trampolines `polarplot`/
    `tan_field`/`dart_field`/`ode_plot`.
  - **Batch 2 ✅ (2026-06-10), 7 demos:** `planes` (the `Plane` class),
    `geomsum`, `uppersum` (`riemann_sum` + `IntegralType` enum), `torus`
    (`domain` `resize`/`slice` + 2-var `plot(F, domain)`), `weierstrass`, `denom`,
    `demoivre` (the `pair` complex-like type) — all byte-identical. Bound:
    `Plane`/`pair` classes, `IntegralType` + `riemann_sum`, `domain.resize1/2` +
    `slice1/2`, `white`/`ddot`/`dot_size`/`tikz_format`/`cb`/`cis`, `E_1/E_2/E_3`.
    - **Gotcha — overload by callable arity.** `plot(f, domain)` has two forms
      (`f(u,v)` surface-wireframe vs `f(x,y,z)` box). Disambiguated by a
      **try-call probe** (`f(0,0)` → 2-var, else 3-var). `inspect.signature` is
      *unreliable* — nanobind functions report `(args, kwargs)` → arity 2 — and a
      function with a defaulted 3rd arg (`xyz(x,y,z=0)`) is callable with 2 args,
      so ports pass a **strict-arity lambda** (`lambda x, y, z: P(x, y, z)`) when
      they want the 3-var form.
    - **Gotcha — match C++ default args exactly.** `h_axis_labels` default align
      is `none`, not `b`; binding the wrong default silently breaks demos that
      rely on it (weierstrass). Check the header's `= ...` defaults.
  - **Batch 3 ✅ (2026-06-10), 7 demos:** `cropplot` (poles at sample points —
    `g` guards `1/0 → inf` to match C++ double division), `plotting3`, `sqrt`,
    `wheel`/`layout`/`layout2` (the **`screen`** class + `activate`/`inset`/
    `deactivate` for multi-panel layouts), `pascal` (the **`Segment`** class,
    intersections via `*`). Bound: `screen`, `Segment`, `activate`/`deactivate`/
    `inset`(2 forms)/`border(Color,str)`, 4-arg `dot`.
  - **Deferred:** `koch.xp` (`screen` is now bound, but it also needs `fractal`
    with a C int-array seed — bind a list→array shim when revisited); `newton.xp`
    (uses `Deriv(f)(t)` as a functor for the slope — needs a `Deriv` callable or a
    `slope(f, t)` helper).
  - **Batch 4 ✅ (2026-06-10), 3 demos:** `saddle` (half-space clipping —
    `clip_face`/`clip_restore`/`label_color`), `surface_shade` + `densityplot`
    (surface with a per-position color function). Bound: `label_color`,
    `clip_face`/`clip_restore`, `base`, `backing`, and `surface(f, domain, color)`
    (2-var `f` + 3-var color trampolines).
    - **`render()` made resilient:** the eepic is always captured; the PNG is
      best-effort. `surface_shade`'s eepic is **129 K lines** and exceeds LaTeX's
      memory when rasterized (the C++ original sidesteps this via PSTricks), so
      `render()` now returns a `Figure` with the correct eepic + an *empty* PNG
      instead of crashing the notebook (`_repr_png_` → `None`). The byte-identity
      check (the real correctness criterion) still passes. *(Bumping LaTeX's
      `main_memory` in the image would let these giant figures rasterize too —
      future polish.)*
  - **Batch 6 ✅ (2026-06-10), 2 (un-deferred):** `newton` — bound `deriv(f, t)`
    (atomic `Deriv(tramp_d)(P(t))`, returns the P whose `.x2()` is the slope);
    `bowl` — bound the **`scenery`** surface object via a Python-side wrapper
    (`PyScenery` holds the callable + domain; `draw()` builds the C++ `scenery`
    and renders atomically with the trampoline live). The wrapper holds a Python
    callable → a GC cycle that nanobind's *shutdown* leak-check flags (benign;
    invisible in a live kernel); the harness now `gc.collect()`s + `os._exit`s to
    keep that quiet.
  - **Still deferred:** `koch` (`fractal` + C int-array seed), `hyperboloid`
    (`surface_rev` + the `frame` class).
  - **Batch 5 ✅ (2026-06-10), 3 demos:** `pole` (already covered), `vfield` +
    `lorenz` (3-D vector fields). Bound the **3-var field forms** — `ode_plot`
    (`F(x,y,z)`, 5-arg), `dart_field`/`vector_field` over a domain — each sharing
    the Python signature with its 2-var sibling, so dispatched by the same
    try-call arity probe (`F(0,0)` → 2-var, else 3-var). `lorenz` also exercises
    the `screen` stereo-pair + `inset(screen, …)`.
  - **Ports so far (54/81):** + `tori`, `twisted_cubic`, `sqrt2`. Refactored `scenery` to build eagerly (per-surface fill captured correctly) + `add()` for multi-surface; fixes the GC-cycle leak. Bound `magenta`/`Neutral`/`RGB_Neutral`/`clip_box(P)`/`camera.filter`. `trig`, `coord_tricks`, `R_demo` (`axis_break`/`recip`/`shadeplot`), `artifacts`, + batch 9 (`dipole`/`medians`/`symmetries`/`contour`/`inverse` — `path`/`affine`/screen-transforms/`flow`/`J`/`circ`/`right_angle`/etc.). Arity probe switched to `inspect.signature` (robust to callables that raise at the probe point, e.g. `1/0`). `spherical`, `oscillator` (`scenery.add` 3-var via lambda-wrap, `arrow_fill`/`v_error_bar`/`h_axis_masklabels`/`slice3`/screen-corners). `steiner2` (4th animation; `camera.eye`/`clip_slice`). `riemann`/`steiner2`/`house` (animations; fixed broken `riemann.flx` enum, bound `camera.eye`/`clip_slice`/`ring`/`gray`). `pendulum` (6th animation; `slope_field`/`integral_eval`).
  - **Batch 10 ✅ (2026-06-10, session 2), 9 demos → 63/81:** `butterfly`,
    `shadeplot`, `clipping`, `legend`, `minkowski`, `decorate`, `log`, `extract`,
    `S2_harmonics` — all byte-identical (butterfly 3/3 stable), regressions clean
    across the whole set (re-verified after a `make format` clang-format/ruff pass
    + rebuild). Bound on demand: `label_border`(Color / +double / +str),
    `label_mask`(Color / no-arg), `CMY_Neutral`, `sgn`,
    `arrow(tail, head, scale)` (the plain curves.h form — **no default on
    `scale`** so a 2-arg `arrow()` still resolves to the existing labelled-marker
    form; registration order keeps old ports unaffected), `camera.viewpt()`,
    `quad(a,b,c,d)`, `path(list, closed, filled)` (added
    `<nanobind/stl/vector.h>` for the list→`std::vector<P>` conversion),
    `picture(P)` (single-P width×height form), `yellow(d)`, `deriv_eval(f, t)`
    (scalar `Deriv(f).eval(t)`), `Line(pt, slope)`, `screen.extract_ellipse`/
    `.backing`/`.border`/`.c()` (lens/magnification + screen center),
    `Color.__mul__`/`__rmul__` (scale intensity — for `RGB(...) *= d`). **Painter-
    sort demos confirmed safe:** `decorate`'s `std::sort` showed no float-tie
    divergence vs Python's Timsort, and `log`'s `stable_sort` matches Python's
    stable sort by construction. **Figure idiom:** the
    `bounding_box`+`unitlength`+`picture(w,h)` / `picture(P)` demos (minkowski,
    log) use the manual `epix.begin()` … `fig = epix.render()` form (like
    `contour`), not the `figure()` context manager (which only wraps
    `picture(sw, ne, size)`).
  - **`histogram` is BLOCKED (not just unbound):** it reads `samples/binom.dat`,
    which **does not exist** in the repo (the sample's own comment says generate
    it via `binom.cc`). The C++ oracle itself can't run without it, so there's
    nothing to verify against. Skip until/unless `binom.dat` is produced.
    `dataplot` is the *other* data demo and IS viable (it generates its data from
    `Cos`/`Sin`) but needs the meatier `data_file(f1, f2, t_min, t_max, n)`
    two-function-trampoline ctor + `transform(P f(u,v))` + `DF.plot(MarkType,…)`
    scatter + the `data_bins` class (`read`/`bar_chart`/`pop`).
  - **Batch 11 ✅ (2026-06-10, session 2 cont.), 11 demos → 75/81:** `levelset`,
    `levelset2`, `levelset3`, `polyhedra`, `hyperboloid`, `mirrorball` (`.flx`),
    `line_debug`, `label_debug`, `koch`, `cubic_cutaway`, `color_sep` — all
    byte-identical, regressions clean (incl. after a `make format` + rebuild).
    Also bound: `fractal` (Python `list[int]` → the C `int*` seed via
    `std::vector<int>.data()`); the **`Complex`** value type (`__mul__`/`__add__`/
    `__sub__`) + **`rootC`** (returns a `P` via ePiX's implicit `P(Complex)`
    conversion — sidesteps `Complex + P` arithmetic in the ports); **scenery with
    a color function** (additively extended `PyScenery` with a 3-arg color ctor +
    `add(f, domain, color)`, dispatching 2-var vs 3-var color — left the existing
    no-color path untouched, regressions on minkowski/spherical/bowl confirm);
    the **CMYK process colors** `CMYK_Neutral`/`C_Process`/`M_Process`/`Y_Process`/
    `K_Process` (camera filters, for `color_sep`). Bound on
    demand: the **`domain_list` subsystem** (`domain.slices2()`/`slices3()` wrap
    the `std::list<domain>` into a `domain_list`; the `domain_list` class;
    `plot(F, domain_list)`; `plot(f, p1, p2, coarse, fine)` **level-set/contour**
    form for a *double*-valued `f` via a new 2-var scalar trampoline `tramp_d2`);
    the **`Sphere`** class (ctors / `center` / `draw` / `__mul__`→`Circle`), the
    **`Circle`** class, **`Sphere*Sphere`→`Circle`** intersection; the **`frame`**
    class (default + 3-vector ctor + `sea`/`sky`/`eye`); `back_/front_dodeca` +
    `back_/front_icosa`; `cam()`; **`surface_rev(f, g, domain, coords, cull)`**
    (two-function trampoline); `label_pad`. `line_debug`/`label_debug` needed
    almost nothing new — their `line_color`/`base_pen`/`set_border`/… "setters"
    are the **sample's own bool-flag helper functions** (reimplemented in Python),
    not ePiX API (the earlier "bind base_color/… setters" note was a misread).
    - **Two binding gotchas re-learned (worth keeping):** (1) the
      **bind-only-*defined*-symbols** rule bit again — `plot(P f(double,double),
      p1, p2, mesh, mesh)` (the P-valued region form) is *declared but not
      compiled* in libepix; referencing it broke the whole module at import with
      an undefined-symbol error. Only the double-valued level-set form is defined,
      so only that is bound. (2) a **class-instance default arg needs its
      `nb::class_` registered first** — `nb::arg("coords") = frame()` on
      `surface_rev` threw `std::bad_cast` at import because `frame` is registered
      later in the module than `surface_rev`; fixed by making `coords` required.
    - **`.flx` count is arbitrary for verification** (Python and C++ both use
      `tix = i/count` with the same `count`); `mirrorball` uses `count=24` like
      `cube`.
  - **Batch 12 ✅ (2026-06-10, session 2 cont.), 3 demos → 78/81 — the lighting
    trio:** `helicoid`, `stereo_proj`, `lighting` (all `.flx`). The task-doc
    grouping was misleading: **only `lighting.flx` uses the shading library**
    `lighting.h` (the sample's own `Spot`/`Fog`/`Chip` classes — reimplemented in
    Python, needing `Color.blend`/`Color.filter` bindings). `helicoid` has its own
    `element` facet class (3-light RGB shading) and needed **no** new bindings;
    `stereo_proj` needed only `Tan` + the `using enum` oracle fix. The enum fix
    (`using enum epix_label_posn;` added to `stereo_proj.flx` and `lighting.flx`,
    after `using namespace ePiX;`) is **output-neutral** (compile-time name
    resolution only — same as the earlier `riemann.flx` fix) and is what lets the
    oracle build under enum-class. C++ `pow(x, 2)` → Python **`math.pow(x, 2)`**,
    not `x**2` (Python folds `**2` to `x*x`, which can differ in the last bit).
    - **`animate()` made render-resilient (figure.py):** a heavy `.flx` frame
      (e.g. `stereo_proj`'s 144×48 surface, or `helicoid`'s 1728 facets) can
      exceed LaTeX/ghostscript limits when rasterized; `animate()` now keeps the
      frame's **eepic** (the correctness artifact, captured pre-render) with an
      empty PNG and drops it from the gif, instead of crashing the whole
      animation — exactly mirroring what `render()` already does for giant single
      figures. The byte-identity check is unaffected (it compares eepic text).
      `.flx` `count` is arbitrary for verification; heavy ones use a small count
      (`stereo_proj` = 9, odd per its own advice) — bump later for a smoother gif.
  - **Batch 13 ✅ (2026-06-10, session 2 cont.), 2 demos → 80/81:** `dataplot`,
    `std_F`. Bound the **`data_file`/`data_bins`** subsystem: `data_file` with the
    2-function-trampoline ctor (placement-`new` `__init__`, sampling `f1`/`f2` via
    `g_fa`/`g_fb`), `transform(P f(u,v))`, `column(col)` (→ Python list), scatter
    `plot(MarkType)`; `data_bins(lo,hi,n)` + `read(list)`/`pop()`/`bar_chart()`.
    For `std_F` (a sample with an external impl file `std_F.cc`): extended
    `affine` (`v_scale`/`h_scale`/`scale`/`shift(pair)`/3-`pair` ctor/`__call__`)
    and `pair` (`__add__`/`__sub__`), reimplemented the `std_F` class in Python
    (skipping its paint-state save/restore — **output-neutral** here because every
    `draw()` fully re-sets fill/pen and nothing between/after the F-draws depends
    on the restored state), and **taught `verify_ports.py` to compile+link a
    sibling `samples/NAME.cc`** so the oracle links (the same pattern any future
    external-library sample would need). Gotcha: `v_axis_labels`' third arg is
    `unsigned int n` — C++ silently truncates `dataplot`'s `0.5*ysize()` double, so
    the port passes `int(0.5*epix.ysize())`.
  - **Remaining 1 — `histogram`, and it is BLOCKED, not portable:** it reads
    `samples/binom.dat`, which **does not exist** in the repo (the sample says to
    generate it via `binom.cc`). The C++ oracle can't run without it, so there's
    nothing to verify against. **This is the only unported demo — 80/81 is
    effectively complete.** Unblock only if `binom.dat` is produced/committed.
  - **Session 2 (2026-06-10) drove the grind to a clean, harness-verified 80/81**
    (batches 10–13 above) — **the port is effectively done.** The only unported
    demo, `histogram`, is hard-blocked by a missing data file (`samples/binom.dat`)
    that the repo doesn't ship. Note: the image store is an ephemeral tmpfs, so a
    session must `make image` first (≈ several min for the TeX-Live layer), then
    iterate with `make py-ext PODMAN_RUN_FLAGS=--cgroups=disabled` + a container
    loop over `verify_ports.py`.
  - **Next phase: the post-port follow-ups are now unblocked** — see
    `tasks/pythonic-cleanup.md`, `tasks/notebook-keyword-args.md`,
    `tasks/source-notebook-grouping.md`, `tasks/distro-packaging.md`, and the ASan
    teardown (mirror remaining bindings into `asan_smoke.cc`, then remove the
    dev-only ASan tooling).
  - **Outstanding (ASan cleanup):** the **batch-10/11 bindings are not yet mirrored
    into `build-aux/asan_smoke.cc`** (the standing "keep mirroring until removal"
    rule). The session-2 churn there is only clang-format reformatting, *not* new
    coverage. Risk is low — the new bindings (`path` list-ctor, `quad`,
    `screen.extract_ellipse`, `deriv_eval`, …) exercise already-covered libepix
    code, and all 9 ports ran clean — but mirror them (and run `make asan`) before
    the final dev-tooling teardown so the smoke test still covers the bound
    surface.

## Out of scope (this task)

- Any implementation — this is the feasibility read only.
- Choosing the final API shape / naming for the Python layer.
- **Pythonic cleanup / clearer variable names** — the ported notebooks
  deliberately mirror the C++ source's names + structure while porting (keeps the
  verify loop and the oracle diff clean). A post-completion readability pass is
  tracked separately in **`tasks/pythonic-cleanup.md`** (deferred until this task
  is done; harness-guarded so it stays byte-identical). Keyword-argument
  readability is in `tasks/notebook-keyword-args.md`.
