# Raise `warning_level` in `meson.build` (currently 0) and clear what it surfaces

**Status:** DONE (2026-10-06) — `warning_level=3`, zero warnings, output byte-identical; see Verification.
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
  (`make shell-exec SCRIPT=tools/verify_image.sh` runs a sample;
  `build-aux/verify_ports.py NAME` for any port). Warning fixes must leave every figure byte-identical —
  see runClaudeInContainer `tasks/reference/porting-with-an-output-oracle.md`.
- **Related:** `tasks/add-sanitizer-gate.md` (also wants a stricter build; coordinate so both touch
  `meson.build` once).

## Goal

Make the compiler's warnings part of the build again — at least `-Wall -Wextra` with `-Wswitch` —
without changing a byte of rendered output, so future edits to the `enum class` dispatch sites and
the rest of the library get the checking a modern C++ build is expected to have.

## Plan

- [x] Build once at `warning_level=3` in a scratch Meson dir and **collect the whole truth**:
      categorise by warning class × count × file (keep-going build, full output).
- [x] Decide per class: fix (sign-compare, unused variables, missing `default:` in the 6 switches —
      per the total-dispatch rule, add a `default:` that is explicit, not silent), or suppress
      narrowly with the reason at the site / in `meson.build` (`-Wno-…` only for a class that is
      noise in this codebase, documented).
- [x] Set `warning_level=2` in `meson.build` (3 if the residue is small enough).
- [x] Verify: `make build` warning-clean; `make asan` green; `make examples` → diff every `.eepic`
      against a pre-change render (byte-identical); harness sample PASS.
- [x] Record the chosen level + suppressions in `CLAUDE.md` ("Build").

## Findings (level-3 build, 2026-10-06 — the ad-hoc worklog, removed at archive — the table below is the record)

207 warnings, 7 classes. Every one fixed at the source; **no `-Wno-…` suppression and no
`warning_level` compromise** — `meson.build` now says `warning_level=3` (`-Wall -Wextra -Wpedantic`).

| Class | Count | What it was | Fix |
| --- | --- | --- | --- |
| `-Wignored-qualifiers` | 138 (10 sites) | `const bool` return type on `Color`/`Complex` comparison operators, re-reported from every TU including `Color.h` | drop the `const` (header + definition) |
| `-Wunused-parameter` | 61 | no-op layer methods (`Color_Sep`, `Color_Neutral`), stub functions (`functions.cc` `zero`), backend methods ignoring `attribs`/`len`/`model`, template projections in `map.h` | `/*name*/` at the compiler's column — a one-shot codemod driven by the warning log's file:line:col (removed at archive; idempotent, 2nd run: 0) |
| `-Winfinite-recursion` | 1 | **real bug:** `Complex::operator!=` was `return *this != arg;` — any `!=` on a `Complex` recursed until stack overflow | `return !(*this == arg);` |
| `-Wvexing-parse` | 1 | `double normal_length();` in `intersections.cc` declared a function instead of a variable; never used | line deleted |
| `-Wcatch-value` | 2 | `catch (std::out_of_range)` by value in `data_file.cc` | `catch (const std::out_of_range&)` |
| `-Wunused-but-set-variable` | 1 | `perp_count` in `curves.cc` `rect()` counted but never read (the twin in `quad`/line 326 IS read and stays) | counter removed |
| `-Wparentheses` | 1 | `label_data.cc`: `m_mark == HTICK && A == t \|\| A == b` | parenthesised **as the compiler already read it**: `(m_mark == HTICK && A == t) \|\| A == b` — behaviour unchanged; see open question 1 |

Also fixed in passing: `build-aux/make_header` used `grep -v "\#include"`, which newer grep flags as a
stray backslash; now `"#include"` — proven identical (same md5 of the filtered header stream, 232
`#include` lines removed either way).

## Verification (2026-10-06, nested)

| Gate | Result |
| --- | --- |
| level-3 scratch build (one-shot collector script, removed at archive) | **0 warnings** (was 207) |
| `make image` (full rebuild) | 0 compiler warnings in the in-image Meson build |
| `make examples` before vs after, 96 figures | `diff -r` ignoring only the timestamp header: **0 lines** — byte-identical (`std_F.xp` non-standalone both times, as always) |
| `make build` (lib + extension from the mount) | 0 warnings; `.so` linked |
| harness sample via `make shell-exec SCRIPT=tools/verify_image.sh` | cube koch lorenz steiner stereo_proj plotting3 tori decorate: **PASS** |
| `make asan` | "ASan smoke OK (no memory errors)" |
| `make format` twice | exit 0 both; clang-format reflowed some `/*name*/` signatures (30 source files modified in total by this task) |
| `make_header` grep change | filtered header stream md5-identical |

## Notes / decisions

- Target level 3, not 2: after the fixes the residue at 3 is **zero**, so nothing argued for the lower
  level. The 6 `switch` statements produced no `-Wswitch` warning at level 3 — each already covers its
  `enum class` fully or has a `default:` — so the total-dispatch concern is satisfied as-is.
- `/*name*/` rather than `[[maybe_unused]]` for the unused parameters: the project had no prior idiom,
  and the comment form keeps the signature readable while making "deliberately ignored" visible.

## Open questions

None. The `label_data.cc` precedence question (`(HTICK && t) || b`, the historical reading, vs a
possibly intended `HTICK && (t || b)`) was decided by the maintainer 2026-10-06
(William Emerison Six <billsix@gmail.com>): **keep as-is** — behaviour unchanged, parentheses
only make the existing precedence explicit.
