#!/usr/bin/env bash
#
# 01-install-base.sh -- the always-needed Fedora packages for ePiX.
#
# One package group per script, no options: WHICH optional groups also get installed
# is decided by the Dockerfile's ARG `if` (this base group always; 02-install-render.sh
# only when BUILD_DOCS=1, which `make image MINIMAL_IMAGE=1` turns off). The same
# packages install during `podman build`, on a bare Fedora host, or in a guest with
# no container runtime. `dnf upgrade` stays in the Dockerfile (its own earlier layer).
#
# What is here and why:
#   Build (Meson): meson + ninja + gcc-c++ + binutils (ar/ranlib for the static lib);
#     bash/sed/findutils/which/diffutils for make_header + the driver scripts.
#   libasan: runtime for AddressSanitizer builds (`make asan`), which catch latent
#     libepix memory bugs. Dev-only for now; tasks/add-sanitizer-gate.md decides whether
#     it becomes a permanent image-build gate or is removed.
#   clang-tools-extra: clang-format + clang-tidy for `make format` and `run-clang-tidy`
#     (pairs with .clang-format / .clang-tidy).
#   Python: the interpreter + headers (python3-devel, for the nanobind extension),
#     setuptools + wheel (the editable install of python/ runs with
#     --no-build-isolation, so the build backend must already be importable -- the
#     /venv is created --system-site-packages and sees these), uv (the package
#     installer the Dockerfile and entry scripts use instead of pip), ruff (Python
#     formatter/linter for `make format`).
#
# A single dnf call, so its own exit status is the gate.
set -uo pipefail

if ! command -v dnf >/dev/null 2>&1; then
    echo "01-install-base.sh: needs 'dnf' (this installs Fedora packages), not found." >&2
    echo "Run on a Fedora host/guest, or inside the project's Fedora-based image." >&2
    exit 1
fi

dnf install -y \
    meson ninja-build gcc-c++ binutils \
    libasan \
    clang-tools-extra \
    findutils sed which diffutils bash \
    python3 python3-devel python3-setuptools python3-wheel \
    uv ruff
