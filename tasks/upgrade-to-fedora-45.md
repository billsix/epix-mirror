# Upgrade the container base from Fedora 44 to Fedora 45

**Status:** in progress
**Priority:** 3
**Difficulty:** 5

## BLUF

Bump `Dockerfile` from `registry.fedoraproject.org/fedora:44` to `fedora:45` and
get the image building green nested on the 45 base. "Done" = the fedora-45 base
builds libepix + the drivers + the nanobind Python extension (the `make build`
gate), with any full-image (`BUILD_DOCS` TeX / `USE_JUPYTER`) gaps documented.

## Context (read first)

Part of a fleet-wide Fedora 44 → 45 bump (William Emerison Six
<billsix@gmail.com>, 2026-11-05). Fedora 45 ships **GCC 16.2 / clang from LLVM
23, glibc 2.44, Python 3.15**. epix is C++ built at `warning_level=3` (the
2026-10-06 rework took it 0 → 3 warnings), plus a **nanobind** Python extension.
Primary risk: new C++ `-Werror`/warning diagnostics from GCC 16 / LLVM 23, and
the nanobind extension compiling against Python 3.15. Secondary: the TeX-Live
render stack (`BUILD_DOCS`) on the 45 base.

## Plan / build strategy (discretion)

1. `Dockerfile`: `fedora:44` → `fedora:45`.
2. **Build `make image MINIMAL_IMAGE=1` first** — builds libepix + drivers + the
   Python extension and runs the gates without the heavy TeX/Jupyter layers;
   this is the real test of the C++ toolchain bump.
3. Then attempt the **full** `make image` (adds TeX + Jupyter). Document any
   upstream-only gap rather than chasing it tonight.
4. Fix any in-scope C++ toolchain breakage minimally; log decisions here.
5. Stage by path; archive.

## Notes / findings

(appended as the build runs)
