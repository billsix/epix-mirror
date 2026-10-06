# Lean image on `MINIMAL_IMAGE=1` — what "minimal" means for epix-mirror

**Status:** proposed — research done 2026-09-10 (survey of the Dockerfile + Makefile from the
runClaudeInContainer sandbox); **implementation needs go-ahead**. One of the per-project children of
runClaudeInContainer `tasks/minimal-image-for-nested-podman-standard.md` (the convention: every optional-feature build flag defaults to its lean value when
`MINIMAL_IMAGE=1` is passed, full otherwise — host or nested); the fleet-wide findings table is runClaudeInContainer `tasks/reference/minimal-nested-images.md`. Created 2026-09-10 at the maintainer's
request (William Emerison Six <billsix@gmail.com>: "go through all of my projects with CLAUDE.md …
research what a minimal nested podman container would be for them").
**Updated:** 2026-10-06 (William Emerison Six <billsix@gmail.com>, via agent) — reworded from the
retired `NESTED_PODMAN`-keyed auto-lean idiom to the opt-in `MINIMAL_IMAGE` flag (renamed 2026-09-27,
when the nested store moved to disk and a full image fits nested). Priority lowered 4 → 7: the lean
image is now a convenience for export/airgap, not a prerequisite for nested work.
**Priority:** 7
**Difficulty:** 4

## BLUF

Make `make image MINIMAL_IMAGE=1` build a lean image (library + Python extension + ASan, no TeX /
ghostscript / ImageMagick / JupyterLab) that still runs this project's gate, while a plain `make image`
— host or nested — stays byte-identical to today — via the idiom
`FLAG ?= $(if $(filter 1,$(MINIMAL_IMAGE)),0,1)` on each optional-feature flag (reference
implementation: runCrushInContainer `client/Makefile`, `FULL_TOOLCHAIN`). Done = the flags below carry
that default, a `MINIMAL_IMAGE=1` build passes the gate, both image sizes are measured and recorded
here and in `CLAUDE.md`. Depends on `tasks/dockerfile-to-standard-template.md` (the dnf-group-script
split), which is where the flag-gated install groups get their files; do that first.

## Context — read first

- runClaudeInContainer `tasks/reference/minimal-nested-images.md` — the standard, the idiom, the rules (a project's *gates* and *product build deps* are never
  trimmed; only editors, docs toolchains, notebooks, GUI extras), and every project's row.
- This repo's `Dockerfile`, `Makefile` (flag block + `image` target), `entrypoint/*install*.sh`.
- The flag-coverage rule (cross-project `CLAUDE.md` › "Verification gates in nested containers"): a
  lean build verifies nothing about the layers it skips — when a change touches what a skipped layer
  consumes, build with that flag ON.

## Findings (2026-09-10)

**What the image installs today.** No build flags. Unconditional: meson/ninja/gcc-c++/binutils/libasan/clang-tools-extra + python3(-devel,-pip) (the library and the nanobind extension need only these), **plus** ghostscript, ImageMagick, six `texlive-*` collections, and a pip layer of jupyterlab/jupytext/ipython/ipykernel/nanobind/ruff. The `lib`/`build`/`py-ext`/`asan`/`format` targets use only the first group; `examples`/`examples-anim` need TeX+ghostscript+ImageMagick; `jupyter`/`notebooks` need the pip layer.

**What "minimal" is here.** Two flags: `BUILD_DOCS` (TeX collections, ghostscript, ImageMagick — the figure renderers) and `USE_JUPYTER` (jupyterlab/jupytext/ipython/ipykernel); nanobind and ruff stay in the base pip install. The lean image builds and ASan-checks libepix and the Python extension — the paths an agent verifies nested.

**Notes.** This is the biggest saving in the fleet after the two sandbox images: the TeX collections + ImageMagick + jupyterlab are most of the image and none of them are needed to build or test the library.

## Plan

- [ ] Dockerfile: `ARG BUILD_DOCS=0` / `ARG USE_JUPYTER=0`; split the single `dnf install` and the pip line accordingly (keep the dnf cache mounts).
- [ ] Makefile: both flags with the `MINIMAL_IMAGE` default; `--build-arg`s on `image`; `examples*` and `jupyter`/`notebooks` targets fail with a clear message when built lean (a `test -x /usr/bin/latex` guard, say).
- [ ] The Makefile mentions `PODMAN_RUN_FLAGS` 3× — confirm it is the `?= $(if …)` auto-default form (the 2026-08-29 rollout says it is) while in there.
- [ ] Measure both images (full vs `MINIMAL_IMAGE=1`); record both sizes here and in `CLAUDE.md`
      next to the "Nested-podman note" paragraph, plus the standard's one-line rule
      ("`make image MINIMAL_IMAGE=1` builds lean; `FLAG=1` re-enables one group").

## Open questions

None — the standard's decisions (dnf-only, gates never trimmed) were the maintainer's on 2026-09-10;
anything project-specific to decide is flagged inline above.
