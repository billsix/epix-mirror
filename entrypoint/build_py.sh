#!/usr/bin/env bash
# build_py.sh -- build the nanobind extension module python/epix/_epix*.so,
# statically linking the installed libepix. Run in the container via `make py-ext`.
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

# nanobind bundles tsl/robin_map under ext/robin_map/include (CMake adds it; we do too).
g++ -O2 -shared -fPIC -std=c++20 -fvisibility=hidden \
    -I"$PYINC" -I"$NBINC" -I"$NBDIR/ext/robin_map/include" -I/usr/local/include \
    python/epix/_epix.cc "$NBSRC" \
    -L/usr/local/lib64/epix -lepix -lm \
    -o "python/epix/_epix${EXT}"

echo "built python/epix/_epix${EXT}"
