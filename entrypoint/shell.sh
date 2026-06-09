#!/usr/bin/env bash
#
# shell.sh -- interactive dev shell entry.  The image already ships epix
# installed on PATH (epix/elaps/flix/laps); the live source tree is bind-mounted
# at /epix.  We do NOT auto-build here -- run `make -f Makefile.docker build`
# (or ./build.sh) when you want to rebuild libepix.a from the bind-mounted
# sources.  Seed a couple of handy commands into history for convenience.
cd /epix

history -s "elaps --pdf samples/hello.xp"
history -s "meson setup build && ninja -C build"
history -s "/build.sh"

cat <<'BANNER'
ePiX dev shell.  Installed tools on PATH: epix  elaps  flix  laps
  /build.sh                       (re)build from sources with Meson -> ./build
  meson setup build && ninja -C build      (equivalently, by hand)
  epix   samples/hello.xp         -> hello.eepic  (LaTeX picture macros)
  elaps --pdf samples/hello.xp    -> hello.pdf
BANNER

exec bash
