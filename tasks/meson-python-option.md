# Task: Make the Python bindings + notebooks an optional Meson feature

**Status:** proposed — needs go-ahead. (Investigation/design can proceed; the
build-file changes wait for confirmation.)
**Requested:** 2026-06-11 (Bill)
**Owner:** Bill (via Claude)

## Goal

Bring the **Python layer into the Meson build as an opt-in feature**, so a
packager (Gentoo ebuild, Fedora `.spec`, Debian `dh`) — or any user — can choose
whether to build and install:

1. the **nanobind extension** `_epix` (compiled from `python/epix/_epix.cc`,
   linked against `libepix`), plus the `epix` Python package (`__init__.py`,
   `figure.py`, `render.py`);
2. the **notebooks**: the `notebooks/*.py` jupytext percent files converted to
   real `.ipynb` (via `jupytext`) and installed to a docs/share location.

Today the C++ tools install via Meson but the Python layer is built **outside**
Meson (the `Makefile`/`entrypoint` scripts). A `-Dpython=…` option makes the
Python front-end a first-class, toggleable install target — exactly what a
Gentoo `USE=python` / Fedora `%bcond` / Debian build-profile needs.

## Why now

`tasks/distro-packaging.md` (Debian/Fedora/Gentoo) is unblocked now that the port
is done, and its **open question #1 is precisely this**: "package the Python
bindings as a separate `python3-epix` binary package, or leave it out?" That
decision is only clean if Meson can build+install the Python layer behind a flag.
So this task is the **build-system enabler** the packaging task depends on.

## Current state (what exists, and where)

- **Meson** (`meson.build`) builds `libepix` (static), the wrapper scripts
  (`epix`/`elaps`/`flix`/`laps` from `*.in`), man pages, samples, `epix.info`.
  It does **not** know about `python/` or `notebooks/`.
- **Python extension** is built by `make py-ext` →
  `entrypoint/build_py.sh`: a standalone `g++` of `_epix.cc` + nanobind's
  `nb_combined.cpp`, **statically linking `libepix`**. Needs `python3-devel`,
  `nanobind`, and nanobind's `ext/robin_map/include`.
- **Notebooks** are converted by `make notebooks` →
  `entrypoint/percentToIpynb.sh` (jupytext `.py` → `.ipynb`).
- `meson_options.txt` already carries `epix_el`, `bash_path`, `runtime_compiler`,
  `ps2eps`, `manual` — so adding options here is idiomatic.

## What Meson gives us (the mechanism)

Meson's `python` module covers all of this:

```meson
pymod = import('python')
py = pymod.find_installation('python3', required: get_option('python'))

py.extension_module('_epix',
  'python/epix/_epix.cc',
  link_with        : libepix,         # the in-tree static lib target
  dependencies     : [py.dependency(), nanobind_dep],
  include_directories: include_directories('include/epix'),
  install          : true,
  subdir           : 'epix',          # -> <platlib>/epix/_epix*.so
)

py.install_sources(
  ['python/epix/__init__.py', 'python/epix/figure.py', 'python/epix/render.py'],
  subdir: 'epix',
)
```

Linking the **in-tree `libepix` target** is actually cleaner than
`build_py.sh` (which links the installed/bind-mounted `.a`) — no stale-library
risk. Install location comes from `py.get_install_dir()` (platlib), which
packagers redirect via `--prefix` / the python install scheme.

## Design sketch (proposal — confirm before building)

1. **Option(s)** in `meson_options.txt`:
   - `option('python', type: 'feature', value: 'auto', description: 'Build and install the nanobind Python bindings (epix package + _epix extension)')`
   - `option('notebooks', type: 'feature', value: 'disabled', description: 'Convert notebooks/*.py to .ipynb (jupytext) and install them')` — gated on `python` being enabled (and only meaningful with jupytext present).
   - `feature` (auto/enabled/disabled) over a bare bool: `auto` builds when
     `python3` + `nanobind` are found and skips otherwise — the right default for
     an optional component with external deps. Packagers pass
     `-Dpython=enabled`/`disabled` for determinism.
2. **nanobind discovery** — the real design question. Options:
   - `dependency('nanobind')` if a pkg-config / CMake-config is exposed (nanobind
     is CMake-first; meson can consume its CMake config via `dependency(...,
     method: 'cmake')`), **or**
   - resolve the include dir from the interpreter:
     `run_command(py, '-m', 'nanobind', '--include-dir')` + compile
     `nb_combined.cpp` as part of the extension (what `build_py.sh` does today),
     plus the `robin_map` include. Pick whichever is robust across the target
     distros; document the build-dep.
3. **Extension target** — `py.extension_module('_epix', …, link_with: libepix)`
   as above; `install: true`, `subdir: 'epix'`.
4. **Package sources** — `py.install_sources([...], subdir: 'epix')` for the three
   `.py` files (+ `pyproject.toml`? decide if a real wheel/dist-info is wanted, or
   just file install — file install is enough for a distro package).
5. **Notebooks target** — `jupytext = find_program('jupytext', required: get_option('notebooks'))`;
   a `custom_target` per `notebooks/*.py` → `.ipynb`, installed to
   `share/epix/notebooks` (or `doc`). Or install the `.py` percent files directly
   and leave conversion to the user. Decide install form + location.
6. **Keep the dev fast-loop** — `make py-ext` (relink-only, `FORCE=1`) is the inner
   dev loop; decide whether it stays as-is (standalone g++) or routes through
   `meson compile` for the extension. Recommend: keep `make py-ext` for dev speed,
   make the **Meson path the canonical build+install** for packaging — and note
   the two must stay consistent (same compile flags, same `libepix`).
7. **Docs** — update `CLAUDE.md` (Build / Python sections), `README.md`
   (Installation + Python interface), and `meson_options.txt` comments.

## Open questions (resolve with Bill before building)

- **nanobind discovery method** (CMake-config dependency vs interpreter
  `--include-dir` + `nb_combined.cpp`) — which is most portable for Gentoo/Fedora/
  Debian packaging? (Gentoo has `dev-python/nanobind`; Fedora `python3-nanobind`.)
- **Notebooks install**: convert to `.ipynb` and install, or install the `.py`
  percent files (lighter, no jupytext build-dep)? And where — `share/epix/notebooks`
  vs a doc dir?
- **Package metadata**: plain `install_sources` (enough for distros) vs a proper
  wheel/`dist-info` (so `pip`/`importlib.metadata` see it). Distro packages
  usually want the former + their own metadata.
- **Replace or coexist** with `entrypoint/build_py.sh`? (Recommend coexist: Meson
  for install, the script for the dev relink loop — but keep them in sync.)
- **`feature` default**: `auto` (convenient) vs `disabled` (explicit, predictable
  for packagers)? Recommend `auto` for `python`, `disabled` for `notebooks`.

## Process

1. Confirm the open questions (esp. nanobind discovery + notebook install form).
2. Add the option(s) + `python` module wiring to `meson.build` /
   `meson_options.txt`; link the extension against the in-tree `libepix`.
3. **Build-test nested** (per global CLAUDE.md): `meson setup build -Dpython=enabled`
   → `meson compile` → `meson install` into a DESTDIR; confirm the `_epix*.so` +
   `epix/` package land in platlib and `python3 -c "import epix"` works against the
   install. Then `-Dpython=disabled` builds the C++ tools only (no Python touched).
4. Re-run the port harness against the Meson-built extension (sanity: same
   byte-identical output as the `make py-ext` build).
5. `make format`; update CLAUDE.md + README + `meson_options.txt`.

## Relationship to other tasks

- **Directly enables `tasks/distro-packaging.md`** (its open question #1). The
  Gentoo ebuild would map this to a `python` USE flag; Fedora to a `%bcond`;
  Debian to a build profile / separate `python3-epix` binary package.
- Builds on the completed `python-bindings-and-notebooks.md` (the bindings + ports)
  and the Meson port (`tasks/archive/2026/06/09/port-to-meson.md`) — review the
  latter for how options/targets are wired before adding new ones.
- The `runtime_compiler` option nuance (the wrapper scripts compile `.xp` at
  runtime) is orthogonal here — this option is about *build-time* compilation of
  the binding.
