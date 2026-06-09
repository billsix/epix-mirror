#!/usr/bin/env bash
# asan_check.sh -- build an AddressSanitizer-instrumented libepix and run the
# smoke test (build-aux/asan_smoke.cc) over it, to catch latent memory bugs in
# the libepix surface the Python bindings cover. Run via `make asan`.
#
# DEV-ONLY (needs libasan, baked into the image for now).
# REMOVE this + libasan + the `asan` Makefile target in the final cleanup phase
# -- see the python-bindings task doc.
set -euo pipefail
cd /epix

BUILD=/tmp/epix-asan
meson setup "$BUILD" -Db_sanitize=address -Dcpp_std=c++20 >/dev/null 2>&1 || \
    meson setup "$BUILD" -Db_sanitize=address -Dcpp_std=c++20 --reconfigure >/dev/null
meson compile -C "$BUILD" >/dev/null

g++ -std=c++20 -fsanitize=address -g -I"$BUILD" \
    build-aux/asan_smoke.cc "$BUILD/libepix.a" -lm -o /tmp/asan_smoke

echo "running asan_smoke under AddressSanitizer ..."
ASAN_OPTIONS=detect_leaks=0 /tmp/asan_smoke
echo "ASan smoke OK (no memory errors); eepic: $(wc -l < /tmp/asan_smoke.eepic) lines"
