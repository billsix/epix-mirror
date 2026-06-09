# Task: Containerized build tooling for ePiX (Makefile + Dockerfile + entrypoints)

**Status:** ready to implement — decisions resolved 2026-06-09, awaiting "go"
**Requested:** 2026-06-09 (Bill)
**Owner:** Bill (via Claude)

## Goal

Give this `/epix` mirror a self-contained, reproducible build environment that
follows **Bill's standard container-dev pattern** (canonical example:
`/gacalc/Makefile` + `Dockerfile` + `entrypoint/`). The host stays clean; all
toolchain lives in a pinned Fedora OCI image; the Makefile is a thin UX layer
wrapping `podman run`. **No Emacs** machinery this time (gacalc's vendored
elpa tree / `USE_EMACS` / `update-emacs-packages` are all omitted).

## Requirements (from the request)

From the Makefile, Bill wants to be able to:

1. **Build the image** (`make image`).
2. **Get a shell** in an ephemeral container (`make shell`).
3. **Build the project** — compile `libepix.a` + the driver scripts via the
   committed autotools (`./configure && make`).
4. **Build the examples** (`make examples`) — render the sample figures into an
   **output folder on the host that is bind-mounted into the container during
   that stage** (mirrors gacalc's `dist` target: `-v $(OUTPUT_DIR):/output:Z`).

## Context that shapes the design (from studying the repo)

ePiX is GNU-autotools C++. `./configure` is already committed (generated), so a
build only needs a compiler + make + binutils — **not** autoconf/automake.
The user-facing CLI is four bash scripts generated from `*.in` at build time:
`epix` (.xp→eepic), `elaps` (.xp→eps/pdf), `laps` (TeX→PS), `flix` (.flx→png/mng).

Toolchain each stage needs (Fedora package guesses in parens — pin at impl time):

| Stage                    | External tools needed                                  |
|--------------------------|--------------------------------------------------------|
| build `libepix.a`        | `gcc-c++`, `make`, `binutils` (ar/ranlib), `findutils`, `sed`, `which`, `bash` |
| `epix` → `.eepic`        | `g++` (runtime-compiles the .xp against libepix.a)     |
| `elaps` → `eps`/`pdf`    | + `latex`, `dvips`, `ps2epsi` (ghostscript), `epstopdf` (a TeX stack) |
| `flix` → `png`/`mng`     | + ImageMagick (`convert`)                               |

The in-tree `make` build emits both installed-path scripts (`epix`, …) and
build-dir variants (`epix-local`, …). `doc/Makefile` and `samples/Makefile`
drive figures with `../epix-local`. Two clean options for the examples stage:
(a) `make install` epix into the image so `elaps` is on `PATH`, or
(b) use the `*-local` scripts straight from the bind-mounted build tree.

## Proposed file set (all NEW; none exist yet)

- `Dockerfile` — Fedora 44 base, BuildKit dnf cache mounts, install the build
  toolchain (+ the example-rendering stack per the decision below), then
  `./configure && make && make install` so the image ships a working `epix`/
  `elaps`/`flix`. Thin `ENTRYPOINT`. Selective `COPY` (not `COPY . .`).
- `.dockerignore` — trim context: `.git`, `output/`, `*.o`, `*.eepic`, `*.eps`,
  `*.pdf`, autotools build litter, `tasks/`.
- `Makefile` — the wrapper (targets below). gacalc-style: `.DEFAULT_GOAL := help`,
  self-documenting `## ` help target, `CONTAINER_CMD = podman`,
  `CONTAINER_NAME = epix`, conditional host-file mounts (gitconfig, tmux — drop
  gnupg/pypirc; no releases here), `OUTPUT_DIR ?= $(CURDIR)/output`.
- `entrypoint/` —
  - `entrypoint.sh` — `exec bash` (thin, like gacalc).
  - `shell.sh` — `cd /epix`; (optionally `./configure && make` if not built);
    `exec bash`.
  - `build.sh` — `./configure && make` (build `libepix.a` + scripts).
  - `examples.sh` — iterate the sample `.xp` (and `.flx`) files, render each to
    `/output` (format per decision below), continue-on-error with a summary.

## Proposed Makefile targets

| Target      | Does                                                                 |
|-------------|---------------------------------------------------------------------|
| `help`      | default goal; greps `## ` descriptions (gacalc's awk/ANSI printer)   |
| `image`     | `podman build -t epix .`                                             |
| `shell`     | ephemeral `-it --rm` shell, live tree bind-mounted, entrypoints mounted in |
| `build`     | run `build.sh` in the container against the bind-mounted tree        |
| `examples`  | `-v $(OUTPUT_DIR):/output:Z`, run `examples.sh` → artifacts on host  |
| `clean`     | (optional) remove `output/` + in-tree build artifacts               |
| `all`       | `image shell` (gacalc convention)                                    |

Bind-mount the entrypoint scripts in (`-v ./entrypoint/build.sh:/build.sh:Z`,
etc.) rather than relying only on the baked copies, so they can be edited
without an image rebuild — exactly the gacalc approach.

## Nested-podman handling (we're podman-in-podman)

This repo's tooling will be run **both** on Bill's host **and** nested inside
the claudecontainer sandbox. Per the global CLAUDE.md, every inner `podman run`
needs `--cgroups=disabled` when nested, but that flag should **not** be baked
into the committed Makefile (it's a no-op-to-undesirable on a normal host).

**Plan:** thread an empty-by-default variable through every `podman run`/`build`,
e.g. `PODMAN_RUN_FLAGS ?=`, so:
- Host (Bill): `make examples` — flag absent, normal behavior.
- Nested (Claude testing here): `make examples PODMAN_RUN_FLAGS=--cgroups=disabled`.

Also confirm nested support before relying on it: `test -e /dev/fuse && podman
info` (the `NESTED_PODMAN=1` tell). Note `podman build` nested may need its own
treatment — verify during implementation. This keeps the committed file
host-correct while letting the nested case pass the flag at invocation.

## Decisions (resolved 2026-06-09)

1. **Example output format → Both (`.eepic` always, PDF on `RENDER=pdf`).**
   `examples.sh` always emits `.eepic` (needs only `g++`); when `RENDER=pdf` is
   passed it also renders PDF via `elaps`. The image therefore **does** carry the
   full TeX-Live + ghostscript stack. `.flx` animations (ImageMagick) stay a
   **separate target** (`examples-anim` or similar) so the movie path doesn't
   gate the common case.
2. **Examples scope → `samples/` + `doc/`.** Build the ~70 `samples/*.xp` **and**
   the ~30 `doc/*.xp` figure sources. (Keep their outputs separated under
   `output/samples/` and `output/doc/` to avoid name clashes and stay legible.)
3. **Install-in-image.** The Dockerfile runs `./configure && make && make
   install` so `epix`/`elaps`/`flix` are on `PATH` in the image; `make -f
   Makefile.docker build` rebuilds from the bind-mounted source for dev.
4. **`output/` gitignored.** Add a top-level `.gitignore` (new file in this
   mirror) ignoring `output/` (and the in-tree build litter: `*.o`, `libepix.a`,
   the generated `epix`/`elaps`/`flix`/`laps` + `*-local`, `epix.h`, `*.eepic`).

**Naming:** the wrapper is **`Makefile.docker`** (not `Makefile`) to avoid the
collision with the autotools-generated `Makefile`; invoked as
`make -f Makefile.docker <target>`. Porting the underlying build to **meson** is
planned later — `Makefile.docker` is the interim wrapper.

## Testing plan (nested, here)

1. `make image PODMAN_RUN_FLAGS=…` — confirm the image builds nested
   (verify `/dev/fuse` first; add `--cgroups=disabled` where needed).
2. `make build …` — confirm `libepix.a` + `epix`/`elaps` scripts are produced.
3. `make examples …` — confirm artifacts land in host `./output/`.
4. `make shell …` — sanity-check the interactive environment.
   (Bill verifies anything needing a real display; these are headless renders so
   Claude can run them — see the build-vs-runtime split.)

## Out of scope

- No Emacs / vendored elpa / `USE_EMACS` / `update-emacs-packages`.
- No PyPI/release machinery (epix isn't a Python package).
- Not modifying the committed autotools (`Makefile.am`, `configure.ac`, etc.) —
  the wrapper `Makefile.docker` wraps them; it does not replace them. The
  name-clash with the autotools-generated `Makefile` is sidestepped by naming
  the wrapper `Makefile.docker` (resolved above); `configure`/`make` run inside
  the container against the bind-mounted tree.
- The future **meson** port is out of scope for this task; `Makefile.docker`
  targets the existing autotools build for now.
