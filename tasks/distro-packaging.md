# Task: Distro packaging — Debian, Fedora 44, Gentoo ebuild

**Status:** proposed — **DEFERRED until the port is complete.** Draft the files,
build-test where feasible, then Bill verifies actual installs.
**Requested:** 2026-06-10 (Bill)
**Owner:** Bill (via Claude)

## Goal

After porting is complete, create the packaging files to build/install ePiX on:

- **Debian** — a `debian/` dir (`control`, `rules`, `changelog`, `copyright`,
  `compat`/source-format, install lists, maybe `watch`).
- **Fedora 44** — an RPM `epix.spec` (Bill's host is Fedora 44).
- **Gentoo** — an `epix-<ver>.ebuild`.

## Why deferred

Package once the Python bindings layer is stable, so we can decide whether/how to
package it (see open questions) rather than re-cutting packages mid-port.

## Build-system context (already in place — favorable)

- **Meson** build (autotools retired): `meson setup build && meson compile &&
  meson install`. Installs `epix`/`elaps`/`flix`/`laps` + `libepix` + headers +
  `epix-lib.sh` + man pages + `samples` + `epix.info`. Default prefix `/usr/local`
  (packaging overrides to `/usr`).
- `meson_options.txt`: `epix_el`, `bash_path`, `runtime_compiler`, `ps2eps`,
  `manual` (off by default — the LaTeX manual is heavy).
- `VERSION` = **1.2.22**. License **GPL v2+** (`COPYING`).
- All three distros have first-class Meson support: debhelper `dh` with the meson
  buildsystem; Fedora `%meson_*` macros; Gentoo `meson.eclass`.

## Critical subtlety: the runtime dependencies are unusual

ePiX needs tooling at **runtime**, not just build time — the `epix`/`elaps`/`flix`
wrapper scripts **compile the user's `.xp` figure on the fly**:

- **A C++ compiler (`g++`) is a RUNTIME dependency** (Depends/Requires/RDEPEND),
  not just build-time. The `runtime_compiler` meson option sets which compiler the
  scripts invoke.
- **bash** (the wrapper scripts), **TeX/LaTeX** (texlive: `latex`, `eepic`,
  `dvips`, `pstricks`, `pgf`), **ghostscript** (`ps2epsi`/`ps2eps`/`ps2pdf`),
  **ImageMagick** (`convert`, for `flix` png/mng/gif).
- Map to each distro's package names. **The `Dockerfile` already enumerates the
  exact Fedora 44 build+runtime set** — reuse it as the authoritative starting
  point for the `.spec`, and translate to Debian (`g++`, `texlive-*`,
  `ghostscript`, `imagemagick`) and Gentoo (`sys-devel/gcc`, `app-text/texlive`,
  `app-text/ghostscript-gpl`, `media-gfx/imagemagick`).

## Open questions (resolve before/while building)

1. **Python bindings package?** Package the nanobind `python/epix` layer +
   notebooks as a separate binary package (`python3-epix`), or leave it out of the
   initial packaging (C++ tools only)? **Recommend:** C++ tools first; Python as a
   follow-on subpackage once stable.
2. **Manual:** build the LaTeX manual (`-Dmanual=true`) into a `-doc` subpackage,
   or ship only `epix.info`/`.texi`? The manual build pulls full TeX; probably ship
   the info/texi and make the PDF manual optional.
3. **Source:** package this local checkout (the mirror) or an upstream 1.2.22
   tarball? Affects the Debian orig tarball / RPM `Source0` / ebuild `SRC_URI`.
4. **Compliance bar:** lintian-clean + Fedora packaging guidelines + Gentoo QA for
   *submission*, or just locally buildable? Sets how much polish is needed.

## Deliverables (proposed layout)

- `debian/` — Debian conventionally lives at the repo root.
- `packaging/fedora/epix.spec` and `packaging/gentoo/app-text/epix/epix-1.2.22.ebuild`
  (or similar) — a `packaging/` dir to hold the non-Debian files.

## Process

1. Confirm the open questions with Bill (esp. Python-package scope + compliance
   bar) and the chosen source.
2. Draft each distro's files, reusing the `Dockerfile` dep list.
3. **Build-test where feasible nested** (per the global CLAUDE.md nested-podman
   setup): the `.spec` via `rpmbuild` in a Fedora 44 image (Bill's host target),
   Debian via an `sbuild`/`pbuilder` image, the ebuild via a stage3 image. I can
   build/package locally; **Bill verifies the actual installed tools run** (display
   /TeX /runtime-compiler behavior — the build-vs-runtime split).
4. Iterate.

## Relationship to other tasks

- Deferred behind `python-bindings-and-notebooks.md`.
- The runtime-compiler nuance ties to the meson `runtime_compiler` option and the
  (archived) `modernize-cxx-standard.md` runtime `-std` coupling — review those so
  the packaged scripts invoke the compiler with the right standard.
