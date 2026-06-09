# Task: Study ePiX and write orientation docs

**Status:** complete
**Completed:** 2026-06-09
Owner: Bill (via Claude)

## Goal

This `/epix` checkout is a mirror of an upstream project that is no longer
actively maintained. Get (re)acquainted with it and leave behind durable
orientation docs:

1. Study the repo enough to describe it accurately.
2. Write a `CLAUDE.md` summarizing the project in relevant detail.
3. Convert the plain-text `README` to Markdown (`README.md`) and update it to
   what's actually true today.

## What ePiX is (one-paragraph summary)

ePiX is a C++ **library + command-line toolchain** for producing
mathematically accurate figures, plots, and simple animations for LaTeX.
You write a short C++ "scene description" (a `.xp` file: `#include "epix.h"`,
`using namespace ePiX;`, a `main()` that calls drawing functions between
`begin()`/`end()`), and the toolchain compiles + runs it to emit LaTeX
picture output (eepic / PSTricks / TikZ) or raster/vector images
(eps/pdf/png/mng/gif). Author: Andrew D. Hwang (College of the Holy Cross),
GPL-2+.

## Key facts established while studying

- **Version is 1.2.22.** `VERSION` and `configure.ac` agree. The *prose* docs
  (`README`, `NEWS`, `BUGS`) are frozen at "Last Change: September 2007", but
  `ChangeLog` runs to **2022-05-08** and source-file headers are dated 2017.
  So: code maintained sporadically through 2022, narrative docs stale since
  2007. This drift is the main thing the new docs needed to correct.
- **Build:** GNU autotools (`./configure && make && make install`). Sources
  listed in `Makefile.am` (`libepix_a_CXXSOURCES`). ~79 `.cc` + ~89 `.h`,
  ~30k LOC. Builds a static lib `libepix.a` and installs headers to
  `$(prefix)/include/epix`.
- **Generated/vendored, do not hand-edit:** `configure`, `Makefile.in`,
  `doc/Makefile.in`, `samples/Makefile.in`, `config.guess`, `config.sub`,
  `depcomp`, `install-sh`, `missing`, `mkinstalldirs`. Also `epix.h` itself is
  **generated** by `./make_header` from a curated header list; the runnable
  `epix`/`elaps`/`flix`/`laps` scripts are generated from the `*.in` templates
  by `configure`/`make` (sed substitution of `@bindir@` etc.).
- **Public API:** everything in `namespace ePiX`. Core value types: `P`
  (ordered triple / 3-vector, `triples.h` — operators: `|` dot, `*` cross,
  `&` componentwise, `%` orthogonalize, `J` quarter-turn), `Complex`,
  `Color` (RGB/CMY/CMYK/Gray models w/ blending, inversion, separation).
  Drawing state is global/imperative (`state.h`): `pen()`, `fill()`, `rgb()`,
  `font_size()`, `clip_*()`, `crop_*()`, etc. Higher-level objects: `axis`,
  `legend`, `path`/`curves`, `circle`/`plane`/`segment`/`sphere` (+ non-member
  `intersections`), `surface`, `plots`, `camera`/`screen`/`picture` for 3-D
  projection and page layout.
- **Output backends** are pluggable: `eepic.cc`, `pst.cc` (PSTricks),
  `tikz.cc`. Adding a backend needs only: arbitrary line segments, color, and
  placing a LaTeX box at a point.
- **Driver scripts** (the user-facing CLI), generated from `*.in`:
  - `epix`  — `.xp` → eepic (LaTeX picture)
  - `elaps` — `.xp`/eepic → eps or pdf
  - `laps`  — (La)TeX → PostScript
  - `flix`  — `.xp`/`.flx` → png or mng animation
  Each takes `-h`. A `.flx` file is an animation scene (uses the `tix()` frame
  counter).
- **Docs:** `doc/` holds the real manual — `manual.tex` (~195k),
  `epix.texi`/`epix.info`, plus dozens of `.xp` example sources and their
  `.eepic` output used as figures.
- **Samples:** `samples/` has ~70 `.xp` + 11 `.flx` worked examples;
  `samples/README` annotates each one (good learning index).

## Steps (all done)

- [x] Read prose docs (README, NEWS, BUGS, THANKS, AUTHORS, INSTALL, ChangeLog).
- [x] Read build files (Makefile.am, configure.ac, doc/ + samples/ Makefile.am).
- [x] Read make_header, epix.in/elaps.in to understand the generated artifacts.
- [x] Sample core headers (triples.h, state.h) + a sample .xp + samples/README.
- [x] Write `CLAUDE.md`.
- [x] `git mv README README.md` and rewrite as Markdown, modernized.
- [x] Point `Makefile.am`'s notefile list at `README.md` so the source build
      stays consistent with the rename (note: generated `Makefile.in` still
      says `README` until automake is re-run — expected for a mirror).

## Open questions / notes for Bill

- I left `NEWS`, `BUGS`, `INSTALL`, `THANKS`, `AUTHORS`, `ChangeLog` as-is
  (plain text, upstream-owned). Only `README` was converted, per the request.
- If you ever re-run autotools here, `make_header` regenerates `epix.h` and
  `configure` regenerates the four driver scripts — don't commit those as
  hand-edits.
- Upstream homepage in the old README (math.holycross.edu/~ahwang) and several
  links (freshmeat, savannah CVS) are long dead; the modernized README.md flags
  them as historical rather than repeating them as live.
