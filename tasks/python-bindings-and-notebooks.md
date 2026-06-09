# Task: Feasibility — Python bindings + percent-format notebooks for ePiX

**Status:** proposed — feasibility investigation done 2026-06-09, awaiting go-ahead
**Requested:** 2026-06-09 (Bill)
**Owner:** Bill (via Claude)

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

*Recommendation:* given the **all-81-demos** goal, the choice is closer than for
an MVP. **A** is the faithful answer (one source of truth) but must cover the
demo-audited surface. **B** can reach **full natural-syntax demo coverage
faster** — a Python free-function API that mirrors the `.xp` idioms ports each
demo near-mechanically and decouples from the C++ ABI. Suggest deciding A vs B
**after Stage 0 (the demo API audit)**, when the real coverage requirement is
known.

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
  optional extras, and reuse of the **`Makefile.docker` container pattern** — a
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
5. **Build coupling** — pybind11 + Meson + a Python interpreter dependency added
   to the image (small).

## Rough effort

Porting all 81 demos makes this a **multi-stage program, not one task**:

- **Stage 0 — demo API audit** (which functions/classes the 81 demos use):
  **~0.5 session.** Bounds everything below.
- **Notebook + render-helper MVP** (reuse `elaps`, container scaffolding,
  `_repr_png_`): **~1–2 sessions.**
- **Bindings to cover the audited surface** + the `Figure`/`tix` shims:
  **several sessions** (scales with the audit; the demos need broad coverage,
  not a core subset). *If `natural-Python ports of all demos` is the priority,
  option B (codegen/emit) may reach full coverage faster, since a Python API
  that mirrors the free functions ports each demo near-mechanically.*
- **Porting the 81 demos** to notebooks + verifying each renders to its original
  figure: **the bulk of the work**, parallelizable per demo once the API +
  render helper exist.

Net: a working *pipeline* (bindings/emit + render + one ported notebook) is
**~3–4 sessions**; the full 81-demo port is a sustained effort on top.

## Decisions for Bill (when/if this proceeds)

1. **Binding approach** — real C++ bindings (A, recommended) vs codegen/emit (B)?
2. **Binding tech** — pybind11 (mature) vs nanobind (leaner)?
3. **Scope** — core-subset MVP first (recommended) vs aim broad?
4. **Inline format** — PNG (simple raster) vs SVG (vector)?
5. **Container/jupyter scaffolding** — mirror the `Makefile.docker` jupyter
   target + jupytext flow now, or just a local `pyproject` first?

## Out of scope (this task)

- Any implementation — this is the feasibility read only.
- Choosing the final API shape / naming for the Python layer.
