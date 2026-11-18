# Dockerfile: silence uv's hardlink warning; pin the venv packages (done)

**Done:** 2026-11-18 (William Emerison Six <billsix@gmail.com>)

The venv `RUN` in `Dockerfile` installed `nanobind` and, under `USE_JUPYTER`,
`jupyterlab jupytext ipython ipykernel` with `uv pip install` under a uv cache mount. That printed
uv's "Failed to hardlink files" warning, and none of the versions were pinned.

**Decision** (2026-11-18, William Emerison Six <billsix@gmail.com>, taking the task's
recommendation): lock the whole set, both nanobind (the extension is built against it) and the
Jupyter stack, per the "lock everything" rule in the `python-coding-standard.md` reference doc in
github.com/billsix/runClaudeInContainer ("Pin direct dependencies").

What changed:
- `entrypoint/image-requirements.in` lists the five direct packages.
- `constraints.txt` (95 pins) was generated from it with `uv pip compile` for the image's
  Python 3.15. It was first generated in the previous image, since the new `Dockerfile` needs it to build.
- `make update-pins` regenerates it. Layout as in github.com/billsix/geometricalgebra.
- The `Dockerfile` `COPY`s `constraints.txt` just before the venv `RUN`, installs both groups with
  `-c`, and sets `ENV UV_LINK_MODE=copy`. The cache mount is a different filesystem from the layer,
  so uv has to copy, and the setting says so.
- `CLAUDE.md` describes the lock and the target.

Verified nested on 2026-11-18: `make image` (nanobind 3.1.0 and jupyterlab 4.6.6, the pinned
versions; no hardlink warning), `make build` (libepix + the `_epix.cpython-315` extension), and
`make shell-exec SCRIPT=tools/verify_image.sh` (result 0).

A bump to `constraints.txt` re-runs the venv layer and the layers after it (the `COPY . /epix`
source bake). The slow dnf/TeX layers above it are unaffected.
