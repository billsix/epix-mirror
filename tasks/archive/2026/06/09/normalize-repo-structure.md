# Task: Reorganize the flat repo into a conventional source layout

**Status:** complete
**Completed:** 2026-06-09
**Requested:** 2026-06-09 (Bill)
**Owner:** Bill (via Claude)

## Done (2026-06-09)

Layout now: **`src/`** (78 `.cc`), **`include/epix/`** (87 headers), **`scripts/`**
(epix/elaps/flix/laps `.in` + epix.el.in + epix-lib.sh + bash_completions),
**`man/`** (epix.1.in + the 3 `.so` stubs), **`build-aux/`** (gen_header.sh,
build_manual.sh, make_header). Per the decisions: **deleted** `fmt_template.cc`,
`fmt_template.h`, `debug.h` (dead) and `files` + `INSTALL` (obsolete);
kept `TODO`. All via `git mv`/`git rm` (history preserved).

Build updates (single root `meson.build`): `_epix_cc`/`_epix_h` name lists
prefixed to `src/`…/`include/epix/`… via a `foreach`; added
`include_directories('include/epix')` to the library (sources use quote-includes);
`configure_file` inputs point at `scripts/`/`man/`; `install_data` at
`scripts/`; dropped `INSTALL` from the notefiles; `gen_header.sh` now `cd`s into
`include/epix` and runs `build-aux/make_header`. Docs (`README.md`, `CLAUDE.md`)
updated.

**Validation:** local `meson setup/compile/install` to staging — compile clean
(src↔include split resolves), `epix.h` regenerates (3946 lines), **identical
install layout** (4 scripts `#!/usr/bin/bash`, `lib64/epix/libepix.a`, 87
headers, epix-lib.sh, 70 samples, epix.info, **9 notes** (INSTALL gone), 4 man,
**0 unsubstituted tokens**). Container: `make -f Makefile.docker image` rebuilds
from the new tree and `examples` renders **96/97** — identical oracle.

## Goal

ePiX is a flat repo: ~78 library `.cc` + 87 public `.h` + the script/man
templates all sit at the root alongside docs and meta files. Move to a
conventional layout (`src/`, `include/epix/`, `scripts/`, `man/`) and update the
**Meson** build to match. **Pure relocation + build-file edits — no behavior
change and no change to the installed layout**, so consumers (and the runtime
`epix`/`elaps` scripts) are unaffected.

## Why this is low-risk (from investigation)

- **All project includes are quote-style** (`#include "triples.h"`, never
  `<triples.h>`). So moving headers needs only an `include_directories()` on the
  library target — **no source edits**. Headers that include each other resolve
  same-dir within `include/epix/`.
- **The runtime scripts bake _install_ paths** (`-I@includedir@`,
  `-L@pkglibdir@`, `@pkgdatadir@`), not source-tree paths. The reorg doesn't
  touch the install layout, so the generated `epix`/`elaps`/`flix`/`laps` and
  every downstream `#include "epix.h"` keep working unchanged.
- **`git mv` preserves history**; the container path (`COPY . /epix`) is
  layout-agnostic.

## Proposed target layout

```
include/epix/     87 public headers (Color.h, triples.h, …)
src/              78 library .cc
scripts/          epix.in elaps.in flix.in laps.in epix.el.in
                  epix-lib.sh bash_completions
man/              epix.1.in  elaps.1  flix.1  laps.1
build-aux/        gen_header.sh  build_manual.sh  make_header   (move make_header here)
doc/              (unchanged)
samples/          (unchanged)
meson.build  meson_options.txt                                 (root)
README.md CLAUDE.md COPYING AUTHORS BUGS ChangeLog NEWS POST-INSTALL THANKS VERSION  (root meta)
```

Optionally introduce `src/meson.build` + `include/meson.build` via `subdir()`,
or keep one root `meson.build` with path-prefixed `files()` lists (simpler;
recommend this first).

## Build changes (all in `meson.build` + `build-aux/gen_header.sh`)

Current references that move (from grep): `epix_sources`/`epix_headers`
`files()` lists, `install_headers(subdir:'epix')`, the `gen_header` custom_target
(uses `meson.current_source_dir()`), the `configure_file` inputs (`s+'.in'`,
`epix.1.in`, `epix.el.in`), and `install_data` for `epix-lib.sh` /
`bash_completions` / the `.so` man stubs.

1. **`epix_sources`** → prefix `src/` (or `subdir('src')`).
2. **`epix_headers`** → prefix `include/epix/`.
3. **Add `include_directories('include/epix')`** to the `static_library` so the
   `src/*.cc` quote-includes resolve. (Header-to-header includes resolve
   same-dir already.)
4. **`install_headers`** — still target `includedir/epix`; with sources at
   `include/epix/*.h` use `preserve_path`/`subdir` so basenames land directly in
   `includedir/epix` (verify no double `epix/epix`).
5. **Umbrella `epix.h`** — `make_header` reads **bare** header names from its
   CWD. Move `make_header` to `build-aux/` and update `gen_header.sh` to
   `cd include/epix && <build-aux>/make_header "$out"` (pass the header dir +
   make_header path). The `HEADERS=` list inside `make_header` stays bare names.
6. **`configure_file` inputs** → `scripts/epix.in` … `scripts/epix.el.in`,
   `man/epix.1.in`.
7. **`install_data`** → `scripts/epix-lib.sh`, `scripts/bash_completions`;
   `install_man` → `man/elaps.1` `man/flix.1` `man/laps.1` (+ generated
   `epix.1`). Notefiles stay at root.
8. Sanity: `Dockerfile` `COPY . /epix` and `.dockerignore` need no change;
   `Makefile.docker` + `entrypoint/` are layout-agnostic.

## Decisions to make (recommendation first)

1. **Dead files — delete or relocate?** `fmt_template.cc` + `fmt_template.h`
   (`#include`s a nonexistent `fmt.h`; never in the library) and `debug.h`
   (included by **no** source). *Recommend delete* (they're already out of the
   build; git keeps history). If relocating instead, they'd go to `src/`.
2. **Obsolete meta files** — `INSTALL` (generic autotools instructions, now
   wrong), the `files` manifest (old autotools file list), empty `TODO`.
   *Recommend remove* `files` + `INSTALL`; leave/curate `TODO`.
3. **Public-header dir** — `include/epix/` (mirrors the `includedir/epix`
   install; recommended) vs flat `include/`.
4. **Scripts dir name** — `scripts/` (recommended) vs `tools/`.
5. **`make_header` home** — `build-aux/` with the other build helpers
   (recommended) vs root.
6. **Subdir meson files** — single root `meson.build` with prefixed paths
   (recommended, simplest) vs `subdir('src')`/`subdir('include')`.

## Testing (reuse the existing oracle)

The Phase-1/2 validation harness applies unchanged: local
`meson setup/compile/install` to a staging dir must still yield the **same
install layout** (4 scripts, `lib64/epix/libepix.a`, `epix.h` at includedir, **87
headers** at includedir/epix, `epix-lib.sh`, 70 samples, `epix.info`, 10
notefiles, shebang `#!/usr/bin/bash`, **0 unsubstituted `@tokens@`**); and the
container path (`make -f Makefile.docker image` + `examples`) must still render
**96/97**, with `-Dmanual=true` building the 93-page PDF. Identical outputs =
success, since nothing but file locations changed.

## Effort

Mostly mechanical: a batch of `git mv` + path edits in one `meson.build` + a
small `gen_header.sh` tweak. **~1 focused session.** The only thinking is the
`install_headers`/`make_header` path plumbing (items 4–5) and the dead-file
decision.

## Out of scope

- No C++ source changes (the quote-includes make this unnecessary).
- No install-layout change, no script/runtime change, no new build features.
- Splitting headers into finer public/detail tiers — keep the single
  `include/epix/` for now.
