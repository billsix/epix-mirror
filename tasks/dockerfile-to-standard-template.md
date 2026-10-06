# Bring the container build onto the standard template: dnf group scripts, uv-in-venv, Jupyter config

**Status:** in-progress
**Priority:** 3
**Difficulty:** 4
**Started:** 2026-10-06
**Requested:** 2026-10-06 (William Emerison Six <billsix@gmail.com>, from a written note: "Do updates for
jupyter Dockerfile, as done in modelviewprojection and geometricalgebra, such as turning off those
notifications, and make py files open as notebooks by default. Epix 1) dockerfile change to be like
standard, dnf install script, etc 2) use uv in venv instead of pip --break-system-packages, or do
whatever will be compatible with geometricalgebra and mvp")

## BLUF

Make epix-mirror's `Dockerfile` / `Makefile` / `entrypoint/` conform to the maintainer's
container-per-project template the way geometricalgebra and modelviewprojection already do: system
packages move into host-runnable `entrypoint/0N-install-*.sh` group scripts the Dockerfile dispatches;
the Python stack moves from `pip install --break-system-packages` into a `/venv` populated with
`uv pip`; JupyterLab gets the two standard config tweaks (announcements off, `.py` opens as a
notebook); `shell.sh` becomes the `set -e` + `exec bash "$@"` launcher with a `shell-exec` Makefile
twin. Done = a nested `make image` builds, `make build` + the byte-identity harness still PASS,
`make notebooks` / `make format` run, the Jupyter settings are verified in-image, and the editable
install works with `--network=none`.

## Context

- **Read first:** this repo's `CLAUDE.md` ("Containerized build", "Python bindings + notebooks", the
  per-session dev loop), `Dockerfile`, `Makefile`, `entrypoint/*.sh`, `python/pyproject.toml`.
- **The template** is the "My project layout" section of the cross-project `CLAUDE.md`
  (runClaudeInContainer, personal overlay): Makefile contract (`shell`/`shell-exec` sharing
  `SHELL_RUN_FLAGS`, `REPO_MOUNT`, `PODMAN_RUN_FLAGS`), Dockerfile contract (dnf-cache idiom, group
  scripts dispatched by ARG ifs), entrypoint contract (`shell.sh` = `set -e`; setup; `exec bash "$@"`),
  and "Self-contained images + live source" (deps baked into committed layers at build; only the
  project's own source re-installed editable + offline from the bind mount at runtime).
- **Reference implementations:** github.com/billsix/geometricalgebra `Dockerfile` +
  `entrypoint/{01-install-base.sh,shell.sh,jupyter.sh,format.sh,entrypoint.sh}` (venv at `/venv`,
  `uv pip install --python $(which python) --no-deps --no-index --no-build-isolation -e .` on entry,
  `jupytext-config set-default-viewer python`, `jupyter labextension disable
  "@jupyterlab/apputils-extension:announcements"`); github.com/billsix/modelviewprojection `Dockerfile`
  (same, with `--level=user` on the labextension disable and a note on why).
- **State before this task:** one inline `dnf install` RUN; `pip install --break-system-packages
  jupyterlab jupytext ipython ipykernel nanobind ruff` into the system Python; the `epix` package
  reached via `PYTHONPATH=/epix/python` (never installed); `shell.sh` runs a non-exec'd `bash` so it
  can run `format.sh` on exit; no `shell-exec`; `format.sh` swallows every failure (`|| true`).
- **Decisions (2026-10-06, agent, per the maintainer's note):**
  - Follow gacalc literally (venv at `/venv`, `--system-site-packages`, `uv` from dnf, scripts
    `source /venv/bin/activate`), since the note asks for compatibility with gacalc and mvp.
  - Install the Python package **editable** into the venv (build-time against the `COPY`'d tree, and
    again, offline, on every entry) instead of `PYTHONPATH`; the bind mount shadows the baked copy at
    the same path (`/epix/python`), so host edits win — the template's fixed-path invariant.
  - Two dnf groups: `01-install-base.sh` (build + Python + format toolchain) and
    `02-install-render.sh` (TeX-Live, ghostscript, ImageMagick — only `examples*` and inline PNGs need
    them). **Not flag-gated yet**: both always run. Gating them (`BUILD_DOCS`/`USE_JUPYTER` with the
    `MINIMAL_IMAGE` idiom) is `tasks/minimal-nested-image.md`, which now depends on this split.
  - Format-on-exit moves from `shell.sh`'s non-exec `bash` to an `EXIT` trap in
    `entrypoint/dotfiles/.extrabashrc` (interactive shells only), so `shell.sh` can `exec bash "$@"`.
  - `format.sh` propagates failures (`status=0; cmd || status=1; exit $status`) and no longer hides
    `ruff check` output.

## Goal

The epix-mirror image is built, entered, and formatted the same way as geometricalgebra and
modelviewprojection — group install scripts a bare Fedora host can run, a uv-managed `/venv`
holding JupyterLab/jupytext/nanobind with the standard Jupyter tweaks baked in, the project's own
Python installed editable and offline from the live mount, and the `shell`/`shell-exec` launcher
pair — without changing what the C++ build produces or what the byte-identity harness verifies.

## Plan

- [x] `entrypoint/01-install-base.sh` + `entrypoint/02-install-render.sh` (dnf guard, status
      accumulation where several dnf calls); Dockerfile `COPY` + dispatch; package-set parity diff
      old-RUN vs union-of-scripts recorded below (intentional deltas: `python3-pip` dropped,
      `uv` + `python3-setuptools` + `python3-wheel` + `ruff` added).
- [x] Dockerfile: `/venv` (`python3 -m venv --system-site-packages`), `uv pip install` of
      jupyterlab/jupytext/ipython/ipykernel/nanobind, `jupytext-config set-default-viewer python`,
      `jupyter labextension disable --level=user "@jupyterlab/apputils-extension:announcements"`,
      build-time editable install of `/epix/python`, `COPY entrypoint/dotfiles/ /root/`.
- [x] Entry scripts: `shell.sh` (`set -e`, activate, cd, editable install, `exec bash "$@"`),
      `entrypoint.sh`, `jupyter.sh`, `build_py.sh`, `asan_check.sh`, `percentToIpynb.sh`,
      `format.sh` (guarded activate; status propagation); `dotfiles/.extrabashrc` exit trap.
- [x] Makefile: `SHELL_RUN_FLAGS` shared by `shell` + `shell-exec` (`REPO_MOUNT=/epix`,
      `SHELL_EXEC_ARGS`), `shell-exec: image`.
- [x] Repoint `PYTHONPATH` mentions (`CLAUDE.md` harness line, `build-aux/verify_ports.py` docstring,
      `jupyter.sh`).
- [ ] Verify nested: `make image`; `make build`; harness on a sample of names via `make shell-exec`;
      `make notebooks`; `make format` twice (idempotent, exit 0); Jupyter settings listed in-image;
      `--network=none` entry + `import epix`; `git ls-files -s -- '*.sh'` modes.
- [x] Docs: `CLAUDE.md` + `README.md` build sections; stage by path.

## Notes / decisions

- **Package-set parity (2026-10-06):** old inline `dnf install` (25 names) vs the union of the two
  group scripts (28 names) — `diff` shows exactly the intended deltas: `python3-pip` dropped (uv
  replaces it), `python3-setuptools` + `python3-wheel` (build backend for the `--no-build-isolation`
  editable install, visible through the venv's `--system-site-packages`), `uv`, `ruff` (was pip-installed)
  added. Nothing else moved.
- **Python packages:** jupyterlab, jupytext, ipython, ipykernel, nanobind → `/venv` via `uv pip`;
  ruff → dnf. The uv cache mount only speeds rebuilds; the packages land in the committed `/venv` layer.
- **`.dockerignore`** gained `__pycache__`, `*.so`, `*.ipynb`, `*.tar` (an exported image tar in the
  repo root would otherwise enter the build context and be baked by `COPY . /epix`).
- **ruff gate made real** (`format.sh` now propagates failures; previously every step was `|| true`,
  which hid 17 existing errors): `__init__.py` re-exports → redundant aliases (no `__all__`, so
  `from epix import *` still re-exports everything `_epix` provides); `verify_ports.py` `l` → `line`;
  `stereo_proj.py` `O` → `origin` (variable names never reach the eepic; harness-checked); three
  un-reflowable LaTeX label strings carry `# noqa: E501`; one comment reflowed. `pyproject.toml`
  adopted the shared tier: `N` added, preview-only `E231`/`E302` dropped, per-file ignores with
  reasons (`notebooks/*.py`: N802/N803/N806/N816 — the ports keep the C++ sample's `F`/`Phi`/`P_n`
  names so they read side by side with the oracle; `build-aux/*.py`: T201, a CLI gate prints).
  `tasks/` excluded from ruff: ruff ≥ 0.14 formats Python code blocks inside Markdown and reflowed the
  aligned example comments in four archived task docs (reverted).
- **Format-on-exit** is now an `exit()` shell function in `entrypoint/dotfiles/.extrabashrc` (the
  mvp/gacalc idiom); it fires on `exit`, not on Ctrl-D, and never for a `shell-exec` payload (non-
  interactive bash does not read `.bashrc`).
- **Not done here, by design:** flag-gating the render/Jupyter groups (`tasks/minimal-nested-image.md`);
  a `ty` type-check step (no type checker was in the gate before; gacalc runs `ty check`); renaming the
  notebooks' C++-mirroring names (mvp's "Python naming wins over math shorthand" would say rename —
  the maintainer's call, it touches ~30 notebooks and needs a full harness sweep).

## Open questions
