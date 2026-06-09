#!/bin/sh
# build_manual.sh -- build doc/manual.pdf from manual.tex + the committed .eepic
# figures, via the pstricks-safe route (latex -> dvips -> ps2pdf) that the
# autotools `laps --pdf` used.  No epix run is needed: the figures ship in doc/.
#
# $1 = doc source dir, $2 = output .pdf path.  We build in a scratch dir so the
# source tree stays clean; latex runs in nonstopmode (the manual emits the usual
# undefined-reference passes) and the final `test -s` is the real success gate.
set -e

srcdir="$1"
out="$2"
case "$out" in
  /*) ;;
  *)  out="$PWD/$out" ;;
esac

work="$(mktemp -d)"
trap 'rm -rf "$work"' EXIT

cp -a "$srcdir"/. "$work"/
cd "$work"

latex -interaction=nonstopmode manual >/dev/null 2>&1 || true
latex -interaction=nonstopmode manual >/dev/null 2>&1 || true
makeindex manual.idx       >/dev/null 2>&1 || true
latex -interaction=nonstopmode manual >/dev/null 2>&1 || true
dvips -q -o manual.ps manual.dvi
ps2pdf manual.ps manual.pdf

test -s manual.pdf
cp -f manual.pdf "$out"
