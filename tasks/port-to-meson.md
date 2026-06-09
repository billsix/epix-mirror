# Task: Port ePiX's build from GNU autotools to Meson

**Status:** COMPLETE — all 3 phases done + validated 2026-06-09 (ready to archive)
**Requested:** 2026-06-09 (Bill)
**Owner:** Bill (via Claude)

## Phase 1 — DONE (2026-06-09)

Files added: `meson.build`, `meson_options.txt`, `build-aux/gen_header.sh`; plus a
one-line backward-compatible tweak to `make_header` (`HEADER_FILE="${1:-epix.h}"`
so Meson can pass an output path — the autotools build still calls it with no
arg). The autotools build is **left intact and parallel** (no `.in` template edits;
shebang kept as `#! /bin/bash`).

**What it builds/installs:** `libepix.a` → `libdir/epix`; the 87 public headers →
`includedir/epix`; generated `epix.h` → `includedir`; the four scripts (via
`configure_file` @var@ substitution) → `bindir`; `epix.1` (+ the three `.so`
stubs) man pages; `epix-lib.sh` → `datadir/epix`; notes → `docdir/notes`;
`bash_completions`+`epix.el` → `docdir/config`. Options mirror the autotools
switches (`epix_el`, `bash_path`, `runtime_compiler`, `ps2eps`, + a `manual`
stub for Phase 2). The vestigial autoconf feature checks are dropped.

**Key gotcha found:** `ls *.cc` ≠ the library source set. **`fmt_template.cc` is
in the tree but NOT in `libepix_a_CXXSOURCES`** (it `#include`s a nonexistent
`fmt.h` — dead/incomplete code). The authoritative lists come from `Makefile.am`:
**78 sources**, **87 headers** (the install set also excludes `debug.h` +
`fmt_template.h`). Don't derive these from `ls`.

**Validation (nested, in the `epix` container image + meson/ninja):**
`meson setup && compile && install` over `/usr/local` succeeds; `epix --version`
→ ePiX 1.2.22; layout + 87 headers correct; **eepic render = 96/97** (only the
genuinely non-standalone `samples/std_F.xp` fails — *identical to the autotools
oracle*). All four generated scripts have **zero leftover `@tokens@`**; `elaps`
correctly bakes `/usr/local/bin/epix`, `/usr/bin/ps2epsi`, and the
`epix-lib.sh` path; `epix.1` resolves `@docdir@`.

## Phase 2 — DONE (2026-06-09)

Files added: `samples`/`doc` wiring in the top `meson.build` + `doc/meson.build`
+ `build-aux/build_manual.sh`.

- **samples/** → `install_subdir('samples', install_dir: docdir/samples,
  exclude_files: Makefile.am/Makefile.in/meson.build)` — installs the sample
  **sources uncompressed** (more useful than the autotools `sample_src.tar.gz`).
- **doc/epix.info** (committed Texinfo) → `infodir`, always (no makeinfo run).
- **Printable manual** behind `-Dmanual=true` (default **off**). Key realization:
  the **29 `.eepic` figures are committed** and `manual.tex` `\input`s them, so
  the manual builds with **no epix run / no build-local script** — just
  `latex`×3 → `makeindex` → `dvips` → `ps2pdf` (the pstricks-safe route, in a
  scratch dir via `build_manual.sh`).

**Validation (container):** with manual **off**, `meson install` stages 70 `.xp`
samples + `epix.info`, and `manual.pdf` is correctly **absent**. With
`-Dmanual=true`, `meson compile manual-pdf` produces a **~635 KB manual.pdf**
(multi-page, verified). No extra TeX packages were needed beyond the image's
existing collections.

## Phase 3 — DONE (2026-06-09): cutover + autotools retirement

- **`Makefile.docker` → Meson.** Dockerfile now installs `meson`+`ninja-build`
  (dropped `make`), `COPY . /epix` (trimmed by `.dockerignore`), and builds via
  `meson setup /tmp/build && meson install`. `entrypoint/build.sh` rebuilds with
  `meson compile`; `shell.sh` banner updated. The `examples`/`examples-anim`
  targets are unchanged (they run the installed `epix`).
- **Shebang substitution.** The four `*.in` first lines became `#!@bashpath@`
  (safe now that autotools is gone), so `bash_path` is fully wired; generated
  scripts get `#!/usr/bin/bash`.
- **Out-of-source win.** Meson keeps everything in `build/`, so the source tree
  no longer gets littered with `.o`/scripts/`epix.h`. `.gitignore` shrank to
  just `/output/` + `/build/`.
- **Autotools retired** (recoverable from git history): `configure`,
  `configure.ac`, `Makefile.am`, `Makefile.in`, `config.guess`, `config.sub`,
  `install-sh`, `missing`, `mkinstalldirs`, `depcomp`, and the `samples/`+`doc/`
  `Makefile.am`/`Makefile.in`. **Kept `make_header`** (still the umbrella-header
  list source of truth). Docs (`README.md`, `CLAUDE.md`) rewritten for Meson;
  `meson.build` is now the authoritative source/header manifest.

**Validation:** local `meson setup/compile/install` to a staging dir (shebang
`#!/usr/bin/bash`, lib in `lib64/epix`, 70 samples + `epix.info` installed);
then the full container path — `make -f Makefile.docker image` (Meson-based
Dockerfile) builds, and `make … examples` renders **96/97** (identical oracle),
`make … build` works (out-of-source `build/`, tree stays clean). The PDF/anim
paths use the same generated `epix`/`elaps` already verified.

## Outcome

ePiX now builds with Meson only; autotools is gone. `meson setup build &&
meson compile -C build && meson install -C build` (add `-Dmanual=true` for the
93-page LaTeX manual). The `Makefile.docker` container wrapper drives the same
Meson build. Render parity with the old autotools build confirmed (96/97).

## Goal

Replace the GNU-autotools build (`configure.ac` + `Makefile.am` +
generated/committed `configure`/`Makefile.in`/support scripts) with a **Meson**
build that produces byte-for-byte-equivalent *runtime behavior*: the same
`libepix.a`, the same generated `epix.h`, the same four driver scripts wired to
the same install paths, installed to the same layout so `epix`/`elaps`/`flix`/
`laps` work identically. `Makefile.docker` (the container wrapper just added)
should then drive Meson instead of autotools.

This is a **build-system swap only** — no C++ source changes, no behavior
changes to the tools themselves.

## What the current build actually does (inventory)

Investigated `configure.ac`, `Makefile.am`, `doc/`+`samples/` `Makefile.am`,
`make_header`, and all `*.in` templates. The build has six moving parts:

### 1. Static library `libepix.a`
~79 `.cc` → one static archive. **Plain default compile** — no custom
`CXXFLAGS`/`CPPFLAGS`/`-std` in `Makefile.am`. Source list lives in
`libepix_a_CXXSOURCES`. → trivial Meson `static_library('epix', sources, …)`.

### 2. Generated umbrella header `epix.h`
Produced by the `make_header` POSIX-sh script: it concatenates a **curated,
ordered subset of 33 headers** (the `HEADERS=` list inside `make_header` — NOT
all `.h`, and order matters), strips `#include` lines, and wraps them in a
prelude (standard includes + `EPIX_H` guard). → Meson `custom_target` that runs
`make_header` (keep it as the source of truth for the list) writing to the build
dir. Gotcha: `make_header` hard-writes `./epix.h`; wrap it to emit to `@OUTPUT@`.

### 3. Four driver scripts from `*.in` (sed substitution)
`epix elaps flix laps` are generated from `epix.in`/… by substituting `@VAR@`
placeholders. **Exact per-template surface** (confirmed by grep — ignore the
huge `Makefile.in` placeholder set, that file disappears entirely):

| Template     | Placeholders it needs                                        |
|--------------|-------------------------------------------------------------|
| `epix.in`    | `@RUNTIME_COMPILER@ @includedir@ @pkgdatadir@ @pkglibdir@`   |
| `elaps.in`   | `@PS2EPS_BIN@ @bindir@ @pkgdatadir@`                          |
| `flix.in`    | `@RUNTIME_COMPILER@ @includedir@ @pkgdatadir@ @pkglibdir@`   |
| `laps.in`    | `@pkgdatadir@`                                               |
| `epix.1.in`  | `@docdir@`                                                   |
| `epix.el.in` | `@bashpath@ @bindir@ @infodir@`                              |

Plus the **shebang**: templates contain a literal `#! /bin/bash`; the Makefile
rewrites it to `#! $(BASH_PATH)` via `sed`. Meson's `configure_file` does `@VAR@`
replacement, so either (a) change the template first lines to `#!@BASH_PATH@`
(one-char edit ×4), or (b) keep `/bin/bash` and drop the bash-path option.

Meson `configure_file(configuration: conf)` matches the `@var@` syntax directly.
Build a `configuration_data()` with the keys mapped from Meson dir options
(below). Note Meson conf keys are case-sensitive and must match the token case
(`pkglibdir`, not `PKGLIBDIR`).

### 4. The `*-local` build-dir script variants
`make` also emits `epix-local`/`elaps-local`/… with paths pointed at the **build
tree** (`@bindir@=$(CURDIR)`, `@pkglibdir@=$(CURDIR)`, runtime compiler `$(CXX)`)
so `doc/` can render its figures against the just-built (un-installed) lib
(`doc/Makefile` calls `../epix-local`). → In Meson, either emit a second
`configure_file` set with build-dir values, or render docs/samples via a
`custom_target` that calls the installed-style script with explicit
`-I$builddir -L$builddir` flags. (The `Makefile.docker` `examples` target already
sidesteps this by using the *installed* `epix` — same idea.)

### 5. Man pages
`epix.1` is generated from `epix.1.in` (`@docdir@`). `elaps.1`/`flix.1`/`laps.1`
are **static `.so man1/epix.1` redirect stubs** (install as-is). → Meson
`install_man()` for all four (the generated one via its `configure_file` output).

### 6. Install layout (must be preserved exactly)
| Item                         | Autotools dest            | Meson equivalent                              |
|------------------------------|---------------------------|-----------------------------------------------|
| `epix elaps flix laps`       | `bindir`                  | `install_dir: bindir` (scripts)               |
| `libepix.a`                  | `libdir/epix` (pkglib)    | `install_dir: libdir/'epix'`                  |
| `epix-lib.sh` (pkgdata)      | `datadir/epix`            | `install_data(install_dir: datadir/'epix')`   |
| `epix.h` (include)           | `includedir`              | `install_headers()` (top)                     |
| all other `.h` (pkginclude)  | `includedir/epix`         | `install_headers(subdir: 'epix')`             |
| `epix.1`+3 stubs             | `mandir/man1`             | `install_man()`                               |
| notefiles (AUTHORS, …)       | `docdir/notes`            | `install_data(install_dir: docdir/'notes')`   |
| `bash_completions`, `epix.el`| `docdir/config`           | `install_data(install_dir: docdir/'config')`  |

`pkglibdir = libdir/epix`, `pkgdatadir = datadir/epix`, `docdir =
datadir/doc/epix`, and the scripts must be substituted with the **absolute,
prefix-joined** forms (`get_option('prefix') / get_option('libdir') / 'epix'`,
etc.). Meson has no built-in `docdir` option — compute it.

### Configure-time checks → Meson
`configure.ac` does `AC_PROG_CXX`, finds `bash`/`g++`/`ps2epsi`, and offers
`--enable-epix-el`, `--with-bash`, `--with-runtime`, `--with-ps2eps`. The
feature checks `AC_HEADER_STDC`/`AC_C_CONST`/`AC_C_INLINE`/`AC_CHECK_FUNCS(strtod)`
are **vestigial — confirmed no `config.h`/`HAVE_*`/`STDC_HEADERS` is referenced
in any `.cc`/`.h`** — so Meson drops them entirely (no `configure_file` for a
config header at all).

→ `find_program('bash')`, `find_program('g++')`, `find_program('ps2epsi')`, and
a `meson.options` file:
```
option('epix_el',    type: 'boolean', value: true)   # install emacs mode
option('bash_path',  type: 'string',  value: '')      # '' => find_program('bash')
option('runtime_compiler', type: 'string', value: '') # '' => find_program('g++')
option('ps2eps',     type: 'string',  value: '')       # '' => find_program('ps2epsi')
option('manual',     type: 'boolean', value: false)   # build the LaTeX manual (heavy)
```

## Proposed approach (phased)

**Phase 1 — core build (the 90%).** One top-level `meson.build`:
project (`version: '1.2.22'`), `meson.options`, the `find_program` resolutions,
`static_library`, the `epix.h` `custom_target`, the six `configure_file`s
(4 scripts + man + el) driven by one `configuration_data`, and all the
`install_*` calls. Goal: `meson setup build && meson compile && meson install`
yields a working `epix`/`elaps`. **Test by reusing `Makefile.docker`**: swap the
Dockerfile's `./configure && make …` for the Meson commands and re-run
`make -f Makefile.docker examples` / `examples RENDER=pdf` — the existing
96/97 + 11/11 render results are the regression oracle.

**Phase 2 — docs & samples subdirs.** `doc/meson.build` + `samples/meson.build`
via `subdir()`. The manual build (`latex` ×2 → `makeindex` → `laps --pdf`,
plus `makeinfo` for `.info`) is heavy and fragile; put it behind the `manual`
option (default **off**) as a `custom_target`, or defer entirely and just
install a pre-built manual if present. Samples: install the sample sources
(skip the legacy `sample_src.tar.gz` tarball unless wanted).

**Phase 3 — switch over.** Point `Makefile.docker` at Meson, update
`README.md`/`CLAUDE.md` build sections, and decide the fate of the autotools
files (keep in parallel for one release, or delete `configure.ac`/`Makefile.am`/
`configure`/`Makefile.in`/`config.*`/`depcomp`/`install-sh`/`missing`/
`mkinstalldirs`/`make_header`-invocation once Meson reaches parity — `make_header`
the *script* stays, it's still the header-list source of truth).

## Decisions to make (recommendation first)

1. **Library install location** — keep non-standard `libdir/epix` (faithful, and
   the scripts already expect `-L…/epix`) **(recommended for a pure port)**, or
   modernize to plain `libdir` (cleaner; requires the scripts' `@pkglibdir@` to
   become plain `libdir`). Pure port first; modernize later.
2. **Header location** — keep `epix.h` at `includedir` + others at
   `includedir/epix` (faithful), vs. move all under `includedir/epix`. Keep.
3. **Version single-sourcing** — the version is **hardcoded in `epix-lib.sh`**
   (`"$1 (ePiX 1.2.22)"`), separate from `VERSION`/`project()`. Optional
   improvement: template `epix-lib.sh.in` with `@VERSION@` so `meson.build` is
   the single source. (Recommend doing this small win during the port.)
4. **Shebang** — edit the 4 template first lines to `#!@BASH_PATH@` (recommended,
   keeps the `--with-bash` capability) vs. hardcode `/bin/bash`.
5. **Autotools coexistence** — keep both build systems for a transition release,
   or cut over immediately. Recommend parallel until Meson passes the render
   oracle, then remove.

## Risks / gotchas

- **`make_header` output capture** — it writes `./epix.h`; needs a wrapper to
  emit to the Meson build dir. Low risk.
- **`*-local` doc rendering** — the build-dir script variant pattern doesn't map
  1:1; Phase 2 needs a small custom_target with explicit `-I/-L`. Medium.
- **The LaTeX manual** is the only real scope risk — heavyweight toolchain,
  fragile. Keep it optional/off by default; it is **not** on the critical path.
- **Path absoluteness** — Meson `configure_file` substitutions must be the
  absolute prefix-joined paths, matching what the autotools scripts bake in.
- Everything else (lib, scripts, installs, options) is mechanical.

## Testing strategy

The `Makefile.docker` container is the ready-made test harness — its image has
the full toolchain, and `examples`/`examples-anim` are a concrete behavioral
oracle (96/97 figures, 11/11 animations, valid PDFs incl. PSTricks). A correct
Meson port reproduces those exact results. Add `meson` + `ninja` to the
Dockerfile (Fedora: `meson ninja-build`), swap the build invocation, and diff
the render output and the install tree against the autotools build.

## Effort estimate

- Phase 1 (core): the bulk of the value, a single ~150–250-line `meson.build`;
  **~1 focused session.** Well-bounded — the substitution surface is tiny (6
  templates, ~7 distinct vars) and the feature checks are droppable.
- Phase 2 (docs/samples, manual optional): **~0.5–1 session**, most of it the
  optional manual `custom_target`.
- Phase 3 (cutover + doc updates): **small.**

## Out of scope

- Any C++ source change or tool-behavior change.
- The future-future option of replacing the runtime-compile model (epix shells
  out to `g++` per figure) — orthogonal to the build system.
- Removing the vendored autotools support scripts before Meson reaches parity.
