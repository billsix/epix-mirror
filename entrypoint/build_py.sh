#!/usr/bin/env bash
# build_py.sh -- build the nanobind extension module python/epix/_epix*.so,
# statically linking libepix (preferring the bind-mounted build/ tree so local
# source edits are reflected; see the link-source selection below). Run in the
# container via `make py-ext`.
#
# Standalone build (nanobind's "no CMake" path): compile the binding together
# with nanobind's nb_combined.cpp runtime, at the library's C++20 standard.
set -euo pipefail
cd /epix

PYINC=$(python3 -c 'import sysconfig; print(sysconfig.get_path("include"))')
NBDIR=$(python3 -c 'import nanobind, os; print(os.path.dirname(nanobind.__file__))')
NBINC=$(python3 -c 'import nanobind; print(nanobind.include_dir())')
NBSRC="$NBDIR/src/nb_combined.cpp"
EXT=$(python3 -c 'import sysconfig; print(sysconfig.get_config_var("EXT_SUFFIX"))')

OUT="python/epix/_epix${EXT}"

# Link against the bind-mounted Meson build outputs (build/libepix.a + the
# generated, self-contained build/epix.h) when they exist, so the extension
# reflects locally-edited C++ sources after `make build`.  Each `make` target is
# a separate `podman run --rm`, so a `meson install` from `make build` would NOT
# survive into this container -- linking the persistent, bind-mounted build/ tree
# is what makes edits actually reach the binding.  Fall back to the image's
# installed libepix for a fresh image (py-ext run before any `make build`).
if [ -f build/libepix.a ] && [ -f build/epix.h ]; then
    EPIX_INC=build
    LIBEPIX=build/libepix.a
    echo "linking bind-mounted build/libepix.a (reflects local sources)"
else
    EPIX_INC=/usr/local/include
    LIBEPIX=/usr/local/lib64/epix/libepix.a
    echo "no build/ -- linking installed libepix; run 'make build' to pick up edited sources"
fi

# Freshness skip: don't relink if the .so is newer than both of its inputs.
# Override with FORCE=1.
if [ "${FORCE:-0}" != 1 ] && [ -f "$OUT" ] \
   && [ "$OUT" -nt python/epix/_epix.cc ] && [ "$OUT" -nt "$LIBEPIX" ]; then
    echo "up to date: $OUT (newer than _epix.cc and $LIBEPIX; set FORCE=1 to relink)"
    exit 0
fi

# nanobind bundles tsl/robin_map under ext/robin_map/include (CMake adds it; we do too).
g++ -O2 -shared -fPIC -std=c++20 -fvisibility=hidden \
    -I"$PYINC" -I"$NBINC" -I"$NBDIR/ext/robin_map/include" -I"$EPIX_INC" \
    python/epix/_epix.cc "$NBSRC" \
    "$LIBEPIX" -lm \
    -o "$OUT"

echo "built $OUT"
