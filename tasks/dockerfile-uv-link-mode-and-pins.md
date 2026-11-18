# Dockerfile: silence uv's hardlink warning; decide whether to pin the venv packages

**Status:** proposed — needs go-ahead
**Priority:** 8
**Difficulty:** 2 (the warning) / 4 (adding a `constraints.txt` lock)
**Started:** 2026-11-18 (William Emerison Six <billsix@gmail.com>)

## BLUF

The venv `RUN` in `Dockerfile` installs `nanobind` and, under `USE_JUPYTER`, `jupyterlab
jupytext ipython ipykernel` with `uv pip install` under a uv cache mount, so `make image`
prints uv's "Failed to hardlink files" warning. Those installs are also completely
unpinned: no `==` pins and no `constraints.txt`. Done = `ENV UV_LINK_MODE=copy` is set and the
warning is gone. Adding the lock is optional and waits on the open question.

## Context

- This is the same cleanup as github.com/billsix/geometricalgebra
  `tasks/dockerfile-pin-every-venv-install.md`. The rule is in the `python-coding-standard.md`
  reference doc in github.com/billsix/runClaudeInContainer, "Pin direct dependencies".
- **The hardlink warning:** the uv cache mount is a different filesystem from `/venv`, so uv
  has to copy. Copying is required anyway, since the cache mount is discarded after the build.
- **Unpinned:** each rebuild without cache gets whatever PyPI serves that day. geometricalgebra
  builds epix-mirror through its own `entrypoint/install-epix.sh` and pins nanobind through its
  own `constraints.txt`, so this only affects epix-mirror's own image.
- There is no double install here (no later constrained layer), so the `-`/`+` downgrades
  geometricalgebra shows can't happen.

## Plan

1. Add `ENV UV_LINK_MODE=copy` (with a one-line why) before the venv `RUN`; `make image`; no warning.
2. Only if question 1 is answered yes: add `constraints.txt` (`uv pip compile` for the image's
   Python) and a `make update-pins` target, install with `-c constraints.txt`, using
   geometricalgebra's layout as the model.

## Open questions

1. Add a `constraints.txt` lock and `make update-pins` for nanobind and the Jupyter stack?
   Recommendation: yes for nanobind (the Python extension is built against it). The Jupyter
   front-end is a dev convenience, so it is fine either way.
