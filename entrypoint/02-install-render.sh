#!/usr/bin/env bash
#
# 02-install-render.sh -- the figure-rendering toolchain: everything ePiX needs
# beyond the compiler to turn a scene into eps/pdf/png, i.e. what `make examples`,
# `make examples-anim`, and the notebooks' inline PNGs use. The library itself and
# the Python extension build without any of this (01-install-base.sh is enough).
#
#   (La)TeX: ePiX runs latex even for eepic output (to measure label boxes), and
#     `elaps` renders through latex -> dvips -> ghostscript. The default backend is
#     eepic, but some samples emit PSTricks or TikZ, so those packages are here too.
#   ghostscript: ps2epsi / ps2pdf, the eps and pdf steps of `elaps`.
#   ImageMagick: `convert`, the eps->png and png->mng/gif steps of `flix` animations.
#
# A single dnf call, so its own exit status is the gate.
set -uo pipefail

if ! command -v dnf >/dev/null 2>&1; then
    echo "02-install-render.sh: needs 'dnf' (this installs Fedora packages), not found." >&2
    exit 1
fi

dnf install -y \
    ghostscript ImageMagick \
    texlive-collection-basic \
    texlive-collection-latexrecommended \
    texlive-collection-pictures \
    texlive-collection-fontsrecommended \
    texlive-eepic \
    texlive-epstopdf \
    texlive-dvips \
    texlive-collection-pstricks \
    texlive-pgf
