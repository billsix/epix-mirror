# Raise `warning_level` in `meson.build` (currently 0) and clear what it surfaces

**Status:** proposed — needs go-ahead to implement (filed on the maintainer's say-so, 2026-10-06).
**Priority:** 5
**Difficulty:** 3
**Created:** 2026-10-06 (William Emerison Six <billsix@gmail.com>: "yes file it")

## BLUF

`meson.build` pins `default_options: ['warning_level=0', 'cpp_std=c++20']`, so the library builds
with no compiler warnings at all — in particular no `-Wswitch`, which would check the six `switch`
statements over the `enum class` types that the 2026-06-09 modernization introduced. Raise the level
(target: `warning_level=2`, i.e. `-Wall -Wextra`), fix or deliberately suppress what it reports, and
keep the output oracle byte-identical. Done = the level is raised in `meson.build`, `make build` and
`make asan` are warning-clean, every sample still renders identically, and the decision is recorded.

## Context

- **Read first:** `meson.build` (line 5), `tasks/archive/2026/06/09/modernize-cxx-standard.md`
  (its open decision 4 — "bump `warning_level` from 0 during the work to mine warnings" — was never
  closed; the modernization mined warnings via clang-tidy instead and left the level at 0), and the
  cross-project `code-style-conventions.md` "Prefer total dispatch over an open-ended conditional
  chain" (why `-Wswitch` on a scoped enum is worth having).
- **Current state:** 6 `switch (` sites in `src/` + `include/epix/`; four `enum class` types in
  `include/epix/enums.h`. The library is ~26k lines of 2000s-era C++ that compiled warning-free only
  because warnings were off; expect sign-compare, unused-parameter, and shadowing noise.
- **The oracle:** `make examples` (96/97 figures, `.eepic` text) and the Python harness
  (`make shell-exec SCRIPT=tasks/adhoc/dockerfile-to-standard-template/verify_image.sh` runs a sample;
  `build-aux/verify_ports.py NAME` for any port). Warning fixes must leave every figure byte-identical —
  see runClaudeInContainer `tasks/reference/porting-with-an-output-oracle.md`.
- **Related:** `tasks/add-sanitizer-gate.md` (also wants a stricter build; coordinate so both touch
  `meson.build` once).

## Goal

Make the compiler's warnings part of the build again — at least `-Wall -Wextra` with `-Wswitch` —
without changing a byte of rendered output, so future edits to the `enum class` dispatch sites and
the rest of the library get the checking a modern C++ build is expected to have.

## Plan

- [ ] Build once at `warning_level=3` in a scratch Meson dir and **collect the whole truth**:
      categorise by warning class × count × file (keep-going build, full output).
- [ ] Decide per class: fix (sign-compare, unused variables, missing `default:` in the 6 switches —
      per the total-dispatch rule, add a `default:` that is explicit, not silent), or suppress
      narrowly with the reason at the site / in `meson.build` (`-Wno-…` only for a class that is
      noise in this codebase, documented).
- [ ] Set `warning_level=2` in `meson.build` (3 if the residue is small enough).
- [ ] Verify: `make build` warning-clean; `make asan` green; `make examples` → diff every `.eepic`
      against a pre-change render (byte-identical); harness sample PASS.
- [ ] Record the chosen level + suppressions in `CLAUDE.md` ("Build").

## Notes / decisions

## Open questions

None yet — the target level (2 vs 3) is decided by the size of the residue in step 1.
