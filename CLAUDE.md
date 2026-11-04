# ePiX — repository guide

This file orients Claude (and humans) working in this checkout. It is a
**mirror of an upstream project that is not actively maintained**; treat the
source as the ground truth and the older prose docs with suspicion (see
"Doc drift" below).

## What this is

ePiX (pronounced "epic", a pun on TeX) is a C++ **library plus a set of
command-line wrapper scripts** for producing mathematically accurate figures,
plots, and short animations for LaTeX. Author: Andrew D. Hwang, College of the
Holy Cross. License: **GNU GPL v2 or later** (`COPYING`).

The workflow mirrors LaTeX itself: you write a short C++ "scene description"
in a text editor, then *compile and run* it to emit a figure.

```cpp
/* hello.xp */
#include "epix.h"
using namespace ePiX;

int main() {
  picture(P(-1,-1), P(1,1), "2x1in");   // bounding box + physical size
  begin();
    font_size("Huge");
    label(P(0,0), "Hello, world!");
  end();
}
```

Output formats: LaTeX picture macros (**eepic**, **PSTricks**, **TikZ**),
vector images (eps/ps/pdf), and bitmaps (png/mng/gif).

## The command-line tools (what users actually run)

These four scripts are the user interface. **They are generated** from the
`*.in` templates by `configure`/`make` (sed substitutes `@bindir@`,
`@pkglibdir@`, the runtime compiler, etc.), so edit `epix.in`, not `epix`.

| Tool    | Source     | Does                                            |
|---------|------------|-------------------------------------------------|
| `epix`  | `epix.in`  | `.xp` → eepic (LaTeX picture)                   |
| `elaps` | `elaps.in` | `.xp` / eepic → eps or pdf                       |
| `laps`  | `laps.in`  | (La)TeX → PostScript                             |
| `flix`  | `flix.in`  | `.xp` / `.flx` → png or mng animation            |

Each prints help with `-h`. User config lives in `~/.epixrc`. Recognized input
extensions, in search-preference order: `xp flx cc c C cpp`.

- A **`.xp`** file is a figure: an ordinary C++ `main()` using the ePiX API.
- A **`.flx`** file is an animation: same idea, driven by the `tix()` frame
  counter (see `samples/*.flx`).

## Build

**Meson** (the autotools build was retired 2026-06-09 — see
`tasks/archive/.../port-to-meson.md`; recoverable from git history if needed):

```sh
meson setup build              # -Dmanual=true to also build the LaTeX manual
meson compile -C build
meson install -C build
```

Options (`meson_options.txt`): `epix_el`, `bash_path`, `runtime_compiler`,
`ps2eps`, `manual` (default off — the manual is heavy). `meson.build` pins
`cpp_std=c++20` and **`warning_level=3`** (`-Wall -Wextra -Wpedantic`), and the
library builds warning-free at that level with no suppressions — keep it that way
(an unused parameter is spelled `/*name*/`; see
`tasks/archive/2026/10/06/raise-meson-warning-level.md` for the sweep). Needs a C++ compiler
(`g++`); the tools expect `bash`, `latex`, and `ps2epsi`/`ps2eps` at runtime.
Meson is **out-of-source** — everything lands in `build/`, nothing in the tree.
The printable manual builds from `manual.tex` + the **committed** `doc/*.eepic`
figures (no epix run needed).

### Containerized build (`Makefile`)

`Makefile` wraps the Meson build in a Fedora `podman` image (full
toolchain baked in: meson/ninja, g++, TeX-Live, ghostscript, ImageMagick, plus
the Python/Jupyter stack for the bindings work — see below).
Targets (invoke with `make <t>`): `image`, `shell`, `shell-exec` (the batch twin of
`shell`: `make shell-exec SCRIPT=<repo-relative path> | CMD='...'` runs in the same
container env, no TTY — the two share one `SHELL_RUN_FLAGS` block; in a `CMD` make expands
`$var` (write `$$var`) and double quotes break the empty-payload guard, so use `SCRIPT=` beyond a
one-liner), `lib` (libepix.a + driver
scripts via meson), `build` (**full build** = `lib` + `py-ext`, so the Python
extension reflects edited C++ sources),
`examples` (samples/+doc/ → `./output`, `.eepic`; `RENDER=pdf` adds PDFs),
`examples-anim` (`.flx` → `./output/anim`), `clean`; and for the Python layer:
`py-ext` (build the nanobind extension — links the bind-mounted `build/libepix.a`
when present, else the installed copy; skips relinking when the `.so` is
up-to-date, `FORCE=1` to override), `jupyter` (JupyterLab on :8888 — **depends on
`py-ext`**, so it always builds the extension first),
`notebooks` (jupytext → `.ipynb`), `asan` (**dev-only** sanitizer smoke),
`format` (clang-format C++ + ruff Python via `entrypoint/format.sh`, which exits
nonzero if any step failed — it also runs when you `exit` a `make shell`, through the
hook in `entrypoint/dotfiles/.extrabashrc`).
Entrypoint scripts in `entrypoint/` are bind-mounted (edit without rebuild).
System packages install through two host-runnable group scripts the Dockerfile
dispatches — `entrypoint/01-install-base.sh` (Meson/g++, Python, uv, ruff,
clang-tools) and `entrypoint/02-install-render.sh` (TeX-Live, ghostscript,
ImageMagick) — so the same toolchain can be installed on a bare Fedora host. The
Python stack (JupyterLab, jupytext, ipykernel, nanobind) lives in a `uv`-managed
`/venv` (`--system-site-packages`) that every entry script activates; the `epix`
package is installed **editable** from `/epix/python` at image build and again,
offline, on every `shell.sh`/`jupyter.sh` entry, so the bind mount's live tree
(and the `_epix*.so` that `make py-ext` drops into `python/epix/`) is what
`import epix` sees. JupyterLab is pre-configured with announcements off and
`.py` (jupytext) files opening as notebooks.
Nested (podman-in-podman) runs auto-apply `--cgroups=disabled` via `PODMAN_RUN_FLAGS`
(keyed on the sandbox's `NESTED_PODMAN=1` export). The
render scripts run each figure from a scratch copy with `epix -I.` so the host
tree stays clean and sibling `#include`s resolve. See
`tasks/archive/2026/06/09/container-build-tooling.md` for the design + gotchas.

**Nested-podman note:** the inner image store is an ephemeral on-disk directory
that lives for one sandbox session (not a RAM tmpfs), so a full nested `make image`
fits and needs no pruning between rebuilds; the image does not survive the
session, so each session starts with one `make image`.

**Lean image (opt-in):** `make image MINIMAL_IMAGE=1` builds without the TeX-Live /
ghostscript / ImageMagick rendering stack (`BUILD_DOCS=0`) and without JupyterLab /
jupytext / ipykernel (`USE_JUPYTER=0`): **1.13 GB vs 2.49 GB** full (measured
2026-10-06; 194 vs 1392 rpms). Everything the gates need stays — `make build`,
`make asan`, the byte-identity harness and `tools/verify_image.sh`'s harness
sample all pass on it; `make examples*`, `make jupyter` and `make notebooks` exit 2
with a message naming the flag to re-enable (`make image MINIMAL_IMAGE=1 BUILD_DOCS=1`
re-enables one group). A plain `make image`, host or nested, is the full image —
`MINIMAL_IMAGE` is never auto-set. Convention: runClaudeInContainer
`tasks/reference/minimal-nested-images.md`.

### Generated files — do NOT hand-edit

- `epix.h` — **generated by `make_header`** (concatenates a curated, ordered
  header list that lives inside `make_header`; Meson wraps it via
  `build-aux/gen_header.sh`). Add a public header to that list, not to `epix.h`.
- `epix`, `elaps`, `flix`, `laps`, `epix.1`, `epix.el` — generated from the
  `*.in` templates by Meson's `configure_file` (`@var@` substitution). Edit the
  `.in`, not the output.

To add or remove a library source/header, edit the `epix_sources` / `epix_headers`
lists in `meson.build` (now the authoritative manifest).

## Python bindings + notebooks (`python/`, `notebooks/`)

A second front-end: **real nanobind bindings over `libepix`** plus a notebook
layer, so you write a figure in Python the way the `.xp` samples are written in
C++, rendered inline. Goal was to port all ~81 `samples/` to percent-format
notebooks, each **byte-identical** to the C++ original. The C++ samples stay as
the permanent oracle — this is purely additive. **Port DONE: 79/81 byte-identical**
(recounted 2026-11-04: 79 notebooks port a sample and all pass the harness). Two
samples have no port: `histogram` is hard-blocked (its `samples/binom.dat` is absent,
so the C++ oracle can't run either), and `hello.xp` has only `notebooks/hello.py`, a
display demo (`epix.show`) rather than a port. `notebooks/build.py` is a second demo,
with no sample. Closeout + full history:
`tasks/archive/2026/06/11/python-bindings-and-notebooks.md`. Tech: nanobind →
architecture A, real bindings; C++20.

**Pythonic API (post-2026-06-11).** After the port, the public Python API was
renamed from the C++/ePiX vocabulary to **Pythonic names** (cleanup closeout:
`tasks/archive/2026/06/11/pythonic-cleanup.md`). Use these names in new notebooks:
- **Types → CapWords:** `Point` (was `P`), `Mesh`, `Domain`, `Screen`, `Axis`,
  `Legend`, `Affine`, `Frame`, `Scenery`, `DataFile`, `DataBins`, `Pair`, `Path`
  (`Color`/`Camera`/`Complex`/`Sphere`/`Circle`/`Segment`/`Plane` unchanged).
- **Functions → snake_case:** color *factories* are lowercase (`red()`/`rgb()`/…
  return a `Color`); pen-*setters* are `set_red()`/`set_rgb()`/…; trig lowercased
  (`sin`/`cos`/…); `infinite_line` (was `Line`), `quarter_turn` (was `J`),
  `root_complex` (was `rootC`); grid/mesh count args are `nx`/`ny`/`nz`.
- **Keyword args (convention A):** notebooks keyword the unclear *and* the
  structural/paired args — `n=`, `size=`, `cull=`, `offset=`/`align=`, `width=`,
  `nx=`/`ny=`/`nz=`, `scale=`, and the point pairs `lower_left=`/`upper_right=`,
  `tail=`/`head=`, `center=`/`radius=`/`normal=`, `coarse=`/`fine=`, `loc=`,
  `a=`/`b=`/`c=`/`d=`. The coordinate constructors and other multi-scalar funcs are
  keyworded **everywhere, even nested in expressions** (`sph(radius=, theta=, phi=)`,
  `cyl(radius=, theta=, z=)`, `polar(radius=, theta=)`, `arc(center=, radius=,
  start=, finish=)`, `latitude(lat=, long_min=, long_max=)`, `riemann_sum(f, a=, b=,
  n=, type=)`, `v_error_bar(loc=, error=, kind=, width=)`, `right_angle`,
  `axis_break`, `surface_rev`, `gray(depth=)`, `dot_size(diameter=)`,
  `affine.scale(factor=)`, `DataBins(low=, high=, n=)`, …).
  **Several cryptic binding `nb::arg` names were renamed** for readability:
  `sw`/`ne` → `lower_left`/`upper_right`; `ctr`/`rad`/`r` → `center`/`radius`;
  `perp` → `normal`; `t` → `theta` (polar/sph/cyl); `d` → `intensity` (color tints);
  `col` → `color`; `wd`/`mk`/`err`/`w` → `width`/`kind`/`error`/`width`;
  `wh` → `size`; `diam` → `diameter`; `scr` → `screen`; `posn` → `position`;
  `s` → `factor`; `lo`/`hi` → `low`/`high`. (Kept where already clear: `r`/`g`/`b`
  rgb channels, `t_min`/`t_max`, `e1`/`e2`/`e3`, parametric `t`, `phi`.) The lone
  anchor point in `label(at, …)`, the plotted `f`/`F`, the domain `R`, and the
  single-number color tints (`black(0.2)`) stay positional.
  (`tasks/archive/2026/06/11/expanded-keyword-arguments.md`.) Even the `Point`
  constructor is keyworded everywhere: `Point(x=…, y=…, z=…)`.
- **Type hints:** every notebook starts with `from __future__ import annotations`
  and annotates `def` params/returns (`-> float`, `-> epix.Point`, `-> epix.Color`,
  `-> None`) plus object-typed locals (`panel: epix.Screen`, `R: epix.Domain`, …).
  Annotations don't execute (PEP 563), so they're byte-identity-free.
- **Notebook structure:** `epix.activated(screen)` is the context manager for
  screen activation (replaces manual `activate`/`deactivate` pairs; used by 11
  notebooks), `color_sep` uses a `separation()` helper, and `log.py` keys its
  segment flag on the `PatchKind` enum
  (`tasks/archive/2026/06/11/notebook-structure-and-types.md`). Open follow-ups
  (dir grouping, distro packaging, the sanitizer gate): see `tasks/`.

Layout:
- `python/epix/` — the package. `_epix.cc` (the nanobind binding), `render.py`
  (`.xp`/`.eepic` → png via `elaps`+ghostscript; `Figure` with `_repr_png_`),
  `figure.py` (the Pythonic scene API + `figure()`/`render()` context helpers +
  `animate()`/`Animation`). `__init__.py` does `from ._epix import *` so the API
  grows automatically. `pyproject.toml` alongside.
- `notebooks/*.py` — jupytext percent-format notebooks (`make notebooks` → ipynb).

How it works:
- ePiX's global-state model maps to module-level functions; `figure()` is a
  context manager wrapping `picture()`/`begin()`. Capture is via
  **`print_eepic(file)`** (a clean named-file output — not stdout) → read back into a
  `Figure` holding the eepic text. The PNG is rasterized **lazily** on first display
  (`elaps` → eps → `gs`; `elaps` also accepts an `.eepic`, not just `.xp`), so building
  a figure — and therefore the byte-identity harness — needs only libepix; displaying
  one needs the rendering stack (absent from the lean image, where `_repr_png_` raises
  `FileNotFoundError` and `animate()` keeps the frames' eepic with an empty gif).
- Extension build (`entrypoint/build_py.sh`, `make py-ext`): standalone g++ of
  `_epix.cc` + nanobind's `nb_combined.cpp`, statically linking libepix. Needs
  `python3-devel` + `nanobind` + nanobind's `ext/robin_map/include`.

**Gotchas / hard-won lessons (read before extending the bindings):**
- **Bind only symbols *defined* in libepix.** Some methods are declared in
  headers but never compiled (e.g. `axis::dec()`); binding one makes the *whole
  module* fail at import with an undefined-symbol error. Check first:
  `nm -C /usr/local/lib64/epix/libepix.a | grep ' T '`.
- **Function-pointer args need a trampoline.** `plot`/`surface`/`backplot_N`/etc.
  take raw C function pointers (`P f(double)`, `P F(double,double)`, two- and
  three-function forms). Bind via a module-global `nb::callable` + a C trampoline
  that calls it (one call at a time — fine for figures). See the `tramp_*` /
  `g_fn*` globals in `_epix.cc`.
- **Global state is reset per figure: `reset_state()`.** ePiX keeps its drawing
  state in process-global singletons (pen, fill, line style, label style, the
  color palette `picture_data::m_palette`, output format, camera, screen stack, …).
  `reset_state()` (`state.{h,cc}`, bound as `epix.reset_state()`) restores all of it
  to the startup values, and `figure()` calls it before `picture()`, so a notebook
  can draw any number of figures in one kernel. Manual-form code
  (`bounding_box`/`picture`/`begin`/`render`) calls `epix.reset_state()` itself.
  `animate()` still forks per frame (build + `print_eepic` in the child, parent
  renders), matching how `flix` runs a fresh process per frame, and the child calls
  `reset_state()` too, since the fork copies the parent's state. (`fork()` inside a
  threaded Jupyter kernel is a runtime caveat to watch.) Not reset, deliberately:
  the canvas/page and bounding-box values (every `picture()` form replaces them),
  `tix` (set per frame), and `the_crop_box()` (read by nothing outside
  `cropping.cc`). Design + verification:
  `tasks/archive/2026/11/04/reset-global-state-between-figures.md`.
- **Verify every port with the harness, not by hand.** `build-aux/verify_ports.py
  NAME` execs `notebooks/NAME.py`, grabs its `fig`/`anim`, and diffs the eepic
  against the sample **compiled with `-DEPIX_FMT_EEPIC`** (forces eepic so
  in-file `pst_format()`/`tikz_format()` don't change the oracle) — per-frame for
  `.flx`. Run one name per process (it predates `reset_state()`, and one process
  per name keeps a failure isolated). Loop in the shell for a batch; `PASS`/`FAIL`
  per demo. This is the inner loop of the whole port effort.
- **The per-session dev loop (operational).** The nested image store is
  ephemeral per sandbox session, so a fresh session rebuilds it: `make image`
  (~minutes — the TeX-Live layer). Then iterate: after each `_epix.cc` change run
  `make py-ext` (nested: `--cgroups=disabled` auto-applies; it relinks only when needed;
  `FORCE=1` to force). Note `py-ext` links the bind-mounted `build/libepix.a`, so
  after editing *libepix* C++ sources (`src/`/`include/`) run `make build` (or
  `make lib`) first — otherwise the extension links a stale library. Run the
  harness in the container —
  `make shell-exec CMD='for n in NAME …; do python3 build-aux/verify_ports.py $n; done'`
  (the venv has `epix` installed editable, so no `PYTHONPATH`); `make shell-exec
  SCRIPT=tools/verify_image.sh` is the saved smoke gate (env + 8-port harness sample + Jupyter settings).
  `make shell-exec CMD='python3 tools/one_process_check.py all'` runs every notebook
  back to back in one process and diffs each against a fresh-process run, proving
  `reset_state()` still covers all global state (`all --no-reset` is the control).
  Verification itself only needs `g++` + `libepix` (it diffs the `.eepic` *text*),
  **not** the TeX/ghostscript render path — that's only for the inline PNGs.
- **Dispatch 2-var vs 3-var callables by `inspect.signature`, never a trial
  call.** Several functions (`plot(f,domain)`, `ode_plot`, `dart_field`,
  `surface(...,color)`) have 2- and 3-argument forms sharing one Python
  signature. Count params via `inspect.signature` — a trial call (`f(0,0)`) can't
  tell "wrong arity" from "the function raised at the probe point" (e.g. `1/0` in
  `dipole`). Caveat: nanobind functions report `(args, kwargs)`→2, so when a
  *nanobind* function needs the 3-var form, the port passes a strict-arity lambda
  (`lambda x,y,z: epix.Point(x,y,z)`). See `callable_takes_two` in `_epix.cc`.
- **Objects that hold a function pointer build eagerly.** `scenery` samples its
  surface (and captures the current fill state) at construction/`add()` time, so
  the Python wrapper builds the C++ object immediately (trampoline set→sample→
  cleared), holding no callable — correct per-surface colors and no GC cycle.
- **The bind-on-demand workflow:** port a demo → it fails on a missing symbol →
  `nm`-check + bind that one thing → re-verify. Don't try to bind the whole API
  up front. (The port is now complete; the bind-list history lives in
  `tasks/archive/2026/06/11/python-bindings-and-notebooks.md`.)
- **Four `.flx` were broken upstream** (`lighting`, `helicoid`, `stereo_proj`,
  `riemann` — all fixed here): they use bare enum names (`tr`, …) without
  `using enum`, so they don't even compile under enum-class. Add
  `using enum epix_label_posn;` to make the oracle build before porting.
- **Byte-identity needs the same float *expression structure*, not just the same
  math.** The harness compares eepic byte-for-byte, so floating-point rounding
  must match C++ exactly: (a) reproduce precomputed constants as the source
  writes them — `5*M_PI_4` is `5*(math.pi/4)`, **not** `5*math.pi/4` (the two
  products round to different last bits); (b) preserve addition associativity /
  grouping — `0.25*(pt1+(pt3+(pt5+pt7)))` must keep that nesting, not be
  re-associated to a flat sum; (c) Python's `^` (the bound cross product) binds
  *looser* than `+`/`-`, so a normal `(b-a)^(d-a)` needs explicit parens. A
  mismatch here is a silent eepic diff, not an error.
- **Figure setup has two idioms.** `figure()` (the context manager) wraps
  `reset_state()` + `picture(sw, ne, size)`. Demos that instead use
  `bounding_box(...)` + `unitlength(...)` + `picture(w,h)` / `picture(P)` build the
  scene with the manual lower-level form: `epix.bounding_box(...)`,
  `epix.unitlength(...)`, `epix.picture(...)`, `epix.begin()`, … ,
  `fig = epix.render()` — as `notebooks/contour.py` / `minkowski.py` / `log.py` do.
- **Painter's-algorithm demos port faithfully.** Demos that build a list of
  facets/patches and draw them back-to-front (`decorate`, `log`, …) sort by
  distance: Python's stable `list.sort(reverse=True)` matches C++ `stable_sort`
  with `>` by construction, and even unstable `std::sort` showed no float-tie
  divergence in practice. Mirror the build/sort/draw order exactly and the eepic
  matches.
- **Adding an overload that could shadow an existing one?** nanobind tries
  overloads in *registration order*. To add `arrow(tail, head, scale)` without
  disturbing the existing 2-arg `arrow(tail, head)` (which routes to the
  labelled-marker form), register the new one *after* it and give `scale` **no
  default**, so a 2-arg call can't fall into the 3-arg overload.

**libepix bugs fixed here (independent of the bindings, worth keeping):**
- `screen::screen()` left the pimpl pointer `m_screen` uninitialized — latent UB
  that only "worked" because the canvas lives in zero-initialized static storage.
  Fixed with a default member initializer `screen_data* m_screen = nullptr;` in
  `screen.h`. Found via AddressSanitizer; the binding's call pattern exposed it.
- `Complex::operator!=` returned `*this != arg` — infinite recursion on any `!=`
  between two `Complex` values. Now `!(*this == arg)`. Found by
  `-Winfinite-recursion` when `warning_level` was raised (2026-10-06).
- `Camera::operator=` copied `the_distance` into `the_clip_range` (so an assigned
  camera had the wrong near-clip plane) and leaked the old lens. It now copies
  `the_clip_range`, deletes the old lens, and guards self-assignment. No sample
  assigns a camera; found writing `reset_state()` (2026-11-04), which does.

**Sanitizer tooling — kept pending `tasks/add-sanitizer-gate.md`:** `libasan`
(Dockerfile), `entrypoint/asan_check.sh`, `build-aux/asan_smoke.cc`, the `asan`
Makefile target. They were added as dev-only aids during the bindings work and
stay until that task decides whether they become a permanent image-build gate
(ASan + trap-UBSan) or are removed. `make asan` builds an ASan libepix and runs
the smoke test (which mirrors the bound surface); keep mirroring new bindings
into `asan_smoke.cc` meanwhile.

## Code layout

Conventional layout: **78 `.cc` in `src/`**, **87 public headers in
`include/epix/`** (~30k LOC, all in `namespace ePiX`); wrapper-script templates
in `scripts/`, man pages in `man/`, build helpers in `build-aux/`; the Python
front-end in `python/` + `notebooks/` (see above). Sources use quote-includes
resolved via `include_directories('include/epix')`.
Rough functional grouping:

- **Core math/value types:** `triples.{h,cc}` — `P`, the ordered-triple /
  3-vector class (operators: `|` dot, `*` cross, `&` componentwise, `%`
  orthogonalize, `J` quarter-turn, `norm`); `Complex`, `interval`, `frac`,
  `length`, `pairs`, `affine` (affine maps), `functions`, `deriv`.
- **Color:** `Color*` family — RGB / CMY / CMYK / Gray models with blending,
  superposition, inversion, and CMYK separation (`Color_Sep`).
- **Drawing state (imperative/global):** `state.{h,cc}` — `pen()`, `fill()`,
  `rgb()`/`cmyk()`, `font_size()`, `line_style()`, `clip_*()`, `crop_*()`,
  arrow/dash/dot parameters. This is the "current pen" model; most drawing
  functions read it.
- **Scene / projection / layout:** `camera`, `screen`, `picture`,
  `active_screen`, `frame`, `domain`, `clipping`, `cropping`, `lens`.
- **Geometric objects:** `circle`, `plane`, `segment`, `sphere`, `halfspace`,
  `facet`, `geometry`, plus non-member `intersections` (return an undrawable
  "malformed" object instead of throwing on a degenerate intersection).
- **Paths / curves / plotting:** `path`, `path_data`, `curves`, `spline`,
  `Sline`, `plots` (+ `plot_algorithms.h`), `surface`, `markers`, `arrow_*`.
- **Annotation:** `axis` (decimal/scientific/fraction/trig/log labels),
  `legend`/`legend_item`/`legend_tile`, `label_*`, `glyph`, `format`,
  `fmt_template`.
- **Data:** `data_file`, `data_bins`, `data_mask` (read/plot external data).
- **Output backends:** `eepic.cc`, `pst.cc` (PSTricks), `tikz.cc`. A backend
  only needs to draw arbitrary line segments, set color, and place a LaTeX box
  at a point — adding a format is self-contained.

## Documentation & examples

- `doc/` — the real manual: `manual.tex` (~195k), `epix.texi` / `epix.info`,
  plus many `.xp` example sources and their rendered `.eepic` output used as
  figures.
- `samples/` — ~70 `.xp` + 11 `.flx` worked examples; **`samples/README`
  annotates every one** and is the best learning index (e.g. `hello.xp`,
  `plotting*.xp`, `koch.xp`, `lorenz.xp`, `steiner.flx`).

## Doc drift — read source, not the 2007 prose

The narrative docs froze years before the code did. When they disagree, the
code/ChangeLog wins:

- `VERSION` / `configure.ac`: **1.2.22**.
- `ChangeLog`: maintained through **2022-05-08**; source headers dated 2017.
- `README` (now `README.md`), `NEWS`, `BUGS`: still stamped "Last Change:
  September 2007", and their external links (the holycross homepage, freshmeat,
  the savannah CVS browser) are dead. `README.md` has been modernized and flags
  those as historical; the others are left as upstream text.

## Conventions for working here

- Don't "fix" generated files (above) — change the source they're built from.
- This is a faithful mirror; keep upstream-authored prose (`NEWS`, `BUGS`,
  `THANKS`, `AUTHORS`, `ChangeLog`) intact unless asked otherwise. **Exception
  (William Emerison Six <billsix@gmail.com>, 2026-06-10): code formatting.** `make format` / `entrypoint/format.sh`
  applies `clang-format` (google style, `.clang-format`) tree-wide — *including*
  the upstream `src/`/`include/` sources — and `ruff` to the Python
  (`python/pyproject.toml [tool.ruff]`, covering `notebooks/` too). So formatting
  churn on upstream files is expected and intended, not a violation of the mirror
  rule. (`.clang-tidy` is present for `run-clang-tidy` against a Meson build dir;
  there is no `make tidy` target.)
- `git config blame.ignoreRevsFile .git-blame-ignore-revs` once per clone: the file
  lists the 2026-06-09 tree-wide reformat commit so `git blame` skips it.
- The maintainer commits; Claude does not (see global CLAUDE.md). Non-trivial work gets a
  `tasks/<slug>.md` doc — see `tasks/` for in-flight items and
  `tasks/archive/` for finished ones.
