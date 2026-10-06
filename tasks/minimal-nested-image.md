# Lean image on `MINIMAL_IMAGE=1` — what "minimal" means for epix-mirror

**Status:** DONE (2026-10-06); archived 2026-10-06. Full 2.49 GB / lean 1.13 GB, every gate green on both; see Verification.
Research done 2026-09-10 (survey of the Dockerfile + Makefile from the runClaudeInContainer sandbox). One of the per-project children of
runClaudeInContainer `tasks/minimal-image-for-nested-podman-standard.md` (the convention: every optional-feature build flag defaults to its lean value when
`MINIMAL_IMAGE=1` is passed, full otherwise — host or nested); the fleet-wide findings table is runClaudeInContainer `tasks/reference/minimal-nested-images.md`. Created 2026-09-10 at the maintainer's
request (William Emerison Six <billsix@gmail.com>: "go through all of my projects with CLAUDE.md …
research what a minimal nested podman container would be for them").
**Updated:** 2026-10-06 (William Emerison Six <billsix@gmail.com>, via agent) — reworded from the
retired `NESTED_PODMAN`-keyed auto-lean idiom to the opt-in `MINIMAL_IMAGE` flag (renamed 2026-09-27,
when the nested store moved to disk and a full image fits nested). Priority lowered 4 → 7: the lean
image had become a convenience for export/airgap, not a prerequisite for nested work.
**Priority:** 7
**Difficulty:** 4

## BLUF

Made `make image MINIMAL_IMAGE=1` build a lean image (library + Python extension + ASan, no TeX /
ghostscript / ImageMagick / JupyterLab) that still runs this project's gates, while a plain `make image`
— host or nested — stays the full image with an unchanged package set — via the idiom
`FLAG ?= $(if $(filter 1,$(MINIMAL_IMAGE)),0,1)` on `BUILD_DOCS` and `USE_JUPYTER` (reference
implementation: runCrushInContainer `client/Makefile`, `FULL_TOOLCHAIN`). "Done" was: the flags carry
that default, a `MINIMAL_IMAGE=1` build passes the gates, both sizes measured and recorded here and in
`CLAUDE.md` — met: **2.49 GB full, 1.13 GB lean**. The dnf-group-script split it needed landed the same
day (`tasks/archive/2026/10/06/dockerfile-to-standard-template.md`). One code change rode along: the
lean gate exposed that `epix.render()` rasterized its PNG eagerly (the harness needed `gs` although it
compares eepic text), so `Figure` rasterizes lazily on first display from this change on.

## Context — read first

- runClaudeInContainer `tasks/reference/minimal-nested-images.md` — the standard, the idiom, the rules (a project's *gates* and *product build deps* are never
  trimmed; only editors, docs toolchains, notebooks, GUI extras), and every project's row.
- This repo's `Dockerfile`, `Makefile` (flag block + `image` target), `entrypoint/*install*.sh`.
- The flag-coverage rule (cross-project `CLAUDE.md` › "Verification gates in nested containers"): a
  lean build verifies nothing about the layers it skips — when a change touches what a skipped layer
  consumes, build with that flag ON.

## Chronology (harvested from the branch history before the squash)

1. Filed 2026-09-10 from the fleet survey (NESTED_PODMAN-keyed auto-lean); reworded 2026-10-06 to the
   opt-in `MINIMAL_IMAGE` idiom, priority 4 → 7, and made dependent on the dnf-group-script split.
2. Go-ahead 2026-10-06 ("do the minimal image task next") once that split had landed. Flags, guards
   and docs written; full build proven package-identical to the previous image; lean build proven
   free of the sentinel packages.
3. Two false starts in the lean verification, both the agent's: `make shell-exec` without `MINIMAL_IMAGE=1`
   rebuilt the full image under the lean checks, and `$n` inside a `CMD=` was expanded by make. Re-run
   with the flag and a direct `podman run`.
4. The honest lean harness then failed on `gs` — an accidental dependency of the gate on the render
   stack. Fixed in `python/epix/render.py` + `figure.py` (lazy PNG; gif only when the tools exist);
   verified on both images.
5. Archived 2026-10-06.

## Survey findings (2026-09-10 — the Dockerfile as it was before this task)

**What the image installed then.** No build flags. Unconditional: meson/ninja/gcc-c++/binutils/libasan/clang-tools-extra + python3(-devel,-pip) (the library and the nanobind extension need only these), **plus** ghostscript, ImageMagick, six `texlive-*` collections, and a pip layer of jupyterlab/jupytext/ipython/ipykernel/nanobind/ruff. The `lib`/`build`/`py-ext`/`asan`/`format` targets use only the first group; `examples`/`examples-anim` need TeX+ghostscript+ImageMagick; `jupyter`/`notebooks` need the pip layer.

**What "minimal" was to be.** Two flags: `BUILD_DOCS` (TeX collections, ghostscript, ImageMagick — the figure renderers) and `USE_JUPYTER` (jupyterlab/jupytext/ipython/ipykernel); nanobind and ruff to stay in the base install (as built: ruff via dnf, nanobind alone in the venv). The lean image would build and ASan-check libepix and the Python extension — the paths an agent verifies nested. That is what was built.

**Notes.** This was the biggest saving in the fleet after the two sandbox images: the TeX collections + ImageMagick + jupyterlab were most of the image and none of them are needed to build or test the library (measured outcome: 2.49 → 1.13 GB).

## Plan

- [x] Dockerfile: `ARG BUILD_DOCS=0` / `ARG USE_JUPYTER=0`; `02-install-render.sh` runs only under `BUILD_DOCS=1`; the venv always gets nanobind, the JupyterLab packages + the two Jupyter config commands only under `USE_JUPYTER=1` (dnf + uv cache mounts kept).
- [x] Makefile: both flags with the `MINIMAL_IMAGE` default (gacalc's comment block adapted); `--build-arg`s on `image`; `examples.sh`/`examples-anim.sh` guard on `command -v latex`, `jupyter.sh`/`percentToIpynb.sh` on `jupyter`/`jupytext`, each exiting 2 with a message naming `MINIMAL_IMAGE=1` and the flag to re-enable.
- [x] `PODMAN_RUN_FLAGS` confirmed in the `?= $(if $(filter 1,$(NESTED_PODMAN)),--cgroups=disabled)` form.
- [x] Measured both images; sizes recorded here, in `CLAUDE.md` ("Lean image (opt-in)" paragraph) and
      in the README's target list, with the one-line rule.

## Verification (2026-10-06, nested in the runClaudeInContainer sandbox)

| Check | Result |
| --- | --- |
| `make image` (default flags) vs the pre-change image | **2.49 GB**, 1392 rpms, 97 venv packages; rpm list identical except `libdnf5-plugin-expired-pgp-keys`, which arrived with the fresh `dnf upgrade` base layer (not flag-related); pip list identical |
| full-image gate | `make build` + `make shell-exec SCRIPT=tools/verify_image.sh`: result 0 (venv, 8-port harness PASS, Jupyter settings) |
| `make image MINIMAL_IMAGE=1` | **1.13 GB**, 194 rpms, 3 venv packages (nanobind + its two deps); sentinels absent: texlive-collection-basic, texlive-eepic, ghostscript, ImageMagick, jupyterlab, jupytext, ipykernel; kept: meson, gcc-c++, libasan, clang-tools-extra, uv, ruff, python3-devel, nanobind; 1198 rpms dropped, 0 added |
| lean gates: `make build`, `make asan` | `.so` linked against the lean image's toolchain; "ASan smoke OK" |
| lean harness sample (direct `podman run … /shell.sh -c`, 8 ports) | **PASS** after the lazy-PNG change (before it: 8 × `FileNotFoundError: gs`) |
| lean display / animation | `fig._repr_png_()` raises `FileNotFoundError: gs` (expected, documented); `animate()` → 24 frames with eepic, gif 0 bytes |
| lean offline import (`--network=none`) | editable install re-ran offline; import OK |
| lean guarded targets | `make examples`, `make examples-anim`, `make notebooks` exit 2 with the "built without …; rebuild with plain 'make image' or FLAG=1" message |
| full image after the lazy-PNG change | harness 8/8 PASS; koch PNG 16.6 KB; steiner 24 frames + 815 KB gif; `tools/verify_image.sh` result 0 |
| full image restored as the session default | 2.49 GB, TeX present |

Gotcha met (the agent's own): a `make shell-exec …` run against the lean image must also pass
`MINIMAL_IMAGE=1`, because its `image` prerequisite otherwise rebuilds the **full** image first — the first
lean harness run silently ran on a full image and the guard checks "passed" with exit 0. Any target with
an `image` prerequisite inherits the flag question; `examples*`/`notebooks`/`asan`/`build` have none.
Second gotcha in the same step: make expands `$n`/`$st` inside `CMD='...'` (they must be `$$n`), so the
harness loop ran `verify_ports.py` with no argument; the lean harness was therefore run by a direct
`podman run … /shell.sh -c` instead.

## Open questions

None — the standard's decisions (dnf-only, gates never trimmed) were the maintainer's on 2026-09-10;
nothing project-specific needed a decision.
