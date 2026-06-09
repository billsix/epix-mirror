#!/usr/bin/env bash
#
# build.sh -- build libepix.a + the driver scripts from the (bind-mounted)
# source tree, using the committed GNU autotools.  Run inside the container by
# `make -f Makefile.docker build`.
#
# `all-am` is automake's current-directory (non-recursive) build target: it
# produces libepix.a, the generated umbrella header epix.h, and the four
# wrapper scripts (epix/elaps/flix/laps + their *-local build-dir variants),
# WITHOUT recursing into samples/ and doc/ (doc/ would try to build the whole
# manual).  The image already has epix installed on PATH; this target is for
# rebuilding after editing the C++ sources.
set -euo pipefail

cd /epix

if [ ! -f config.status ]; then
    ./configure
fi
make all-am

echo
echo "Built: libepix.a + epix/elaps/flix/laps (and *-local).  Headers: epix.h"
