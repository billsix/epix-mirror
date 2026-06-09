#!/usr/bin/env bash
#
# build.sh -- (re)build libepix.a + the generated epix.h + the four driver
# scripts from the (bind-mounted) source tree with Meson.  Run inside the
# container by `make build`.  Everything lands in ./build
# (out-of-source), so the source tree stays clean.  The image already ships
# epix installed on PATH; this target is for rebuilding after editing sources.
set -euo pipefail

cd /epix

if [ ! -d build ]; then
    meson setup build --prefix=/usr/local
fi
meson compile -C build

echo
echo "Built (meson): build/libepix.a, build/epix.h, build/{epix,elaps,flix,laps}."
echo "  meson install -C build   # to install into the prefix"
