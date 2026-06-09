#!/bin/sh
# gen_header.sh -- Meson wrapper around ./make_header.
#
# make_header reads its curated, ordered public-header list relative to the
# current directory and (with this port's one-line tweak) writes the assembled
# umbrella header to the path given as $1.  Meson custom_target commands run
# from the build directory, so we cd into the source dir for the header reads
# but resolve the output path to absolute first, keeping the source tree clean.
set -e

srcdir="$1"
out="$2"

case "$out" in
  /*) ;;                      # already absolute
  *)  out="$PWD/$out" ;;      # relative to the build-dir CWD Meson invoked us in
esac

# make_header reads its bare-named header list from CWD, so run it from the
# public-header dir; it now lives in build-aux/.
cd "$srcdir/include/epix"
exec "$srcdir/build-aux/make_header" "$out"
