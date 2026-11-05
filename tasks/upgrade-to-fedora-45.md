# Upgrade the container base from Fedora 44 to Fedora 45 (done)

Done 2026-11-05 (William Emerison Six <billsix@gmail.com>). Part of a fleet-wide
Fedora 44 → 45 bump.

## What changed

- `Dockerfile`: `registry.fedoraproject.org/fedora:44` → `fedora:45`.

No source changes were needed. Built green nested on the 45 base BOTH ways:
- `make image MINIMAL_IMAGE=1` — libepix (C++20, `warning_level=3`) + the drivers
  + the nanobind Python extension + the gates. This is the real toolchain test.
- `make image` (full, 2.77 GB) — adds the TeX-Live/ghostscript/ImageMagick render
  stack + JupyterLab.

The C++ library and the nanobind `_epix.cc` extension compiled clean under GCC 16
/ clang LLVM 23 at `warning_level=3` with no new diagnostics, and the newer
nanobind pulled under Python 3.15 built the extension without API breakage. Image
`localhost/epix:latest`.
