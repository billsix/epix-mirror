#!/usr/bin/env bash
#
# shell.sh -- interactive dev shell entry (and, with a `-c '...'` payload from
# `make shell-exec`, the batch entry). The image ships epix installed on PATH
# (epix/elaps/flix/laps); the live source tree is bind-mounted at /epix. We do NOT
# auto-build the C++ here -- run `make build` (or /build.sh) to rebuild libepix.a
# from the bind-mounted sources.
#
# Fail-fast setup: a failed step aborts rather than dropping you into a half-set-up
# tree. The final `exec bash` is a FRESH bash not under -e. `set -e` only (no -u).
set -e
export VIRTUAL_ENV_DISABLE_PROMPT=1
source /venv/bin/activate
cd /epix
# The project's own Python package, editable + offline against the live mount
# (idempotent; the _epix*.so from `make py-ext` sits inside python/epix/).
uv pip install --python "$(which python)" --no-deps --no-index --no-build-isolation -e python

if [ "$#" -eq 0 ]; then
cat <<'BANNER'
ePiX dev shell.  Installed tools on PATH: epix  elaps  flix  laps
  /build.sh                       (re)build from sources with Meson -> ./build
  meson setup build && ninja -C build      (equivalently, by hand)
  epix   samples/hello.xp         -> hello.eepic  (LaTeX picture macros)
  elaps --pdf samples/hello.xp    -> hello.pdf

On `exit`, the tree is auto-formatted (clang-format + ruff); run /format.sh by hand any time.
BANNER
fi

# No args -> interactive shell. Args (a `-c '...'` payload from `make shell-exec`)
# -> run them after the same setup, in a fresh bash not under -e.
exec bash "$@"
