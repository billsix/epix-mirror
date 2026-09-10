# Lean image for nested-podman builds — what "minimal" means for epix-mirror

**Status:** proposed — research done 2026-09-10 (survey of the Dockerfile + Makefile from the
runClaudeInContainer sandbox); **implementation needs go-ahead**. One of the per-project children of
runClaudeInContainer `tasks/minimal-image-for-nested-podman-standard.md` (the convention: every optional-feature build flag defaults to its lean value when
`NESTED_PODMAN=1`); the fleet-wide findings table is runClaudeInContainer `tasks/reference/minimal-nested-images.md`. Created 2026-09-10 at the maintainer's
request (William Emerison Six <billsix@gmail.com>: "go through all of my projects with CLAUDE.md …
research what a minimal nested podman container would be for them").
**Priority:** 4
**Difficulty:** 4

## BLUF

Make `make image` inside a sandbox (which exports `NESTED_PODMAN=1`) build a lean image that fits
the nested RAM store and still runs this project's gate, while a host `make image` stays
byte-identical — via the idiom `FLAG ?= $(if $(filter 1,$(NESTED_PODMAN)),0,1)` on each optional-feature flag (the `PODMAN_RUN_FLAGS`
pattern applied to build flags; reference implementation: runCrushInContainer `client/Makefile`,
`FULL_TOOLCHAIN`). Done = the flags below carry the nested-aware default, a nested `make image`
builds and passes the gate, both image sizes are measured and recorded here and in `CLAUDE.md`.

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
- [ ] Makefile: both flags with the nested-aware default; `--build-arg`s on `image`; `examples*` and `jupyter`/`notebooks` targets fail with a clear message when built lean (a `test -x /usr/bin/latex` guard, say).
- [ ] The Makefile mentions `PODMAN_RUN_FLAGS` 3× — confirm it is the `?= $(if …)` auto-default form (the 2026-08-29 rollout says it is) while in there.
- [ ] Measure both images; the CLAUDE.md already has a 'Nested-podman caveat' paragraph (line ~96) — replace it with the new default.
- [ ] Record both sizes (host full vs nested lean) here and in `CLAUDE.md`; add the standard's one-line
      rule to `CLAUDE.md` ("nested = lean image automatically; `FLAG=1` overrides").

## Open questions

None — the standard's decisions (dnf-only, gates never trimmed) were the maintainer's on 2026-09-10;
anything project-specific to decide is flagged inline above.
