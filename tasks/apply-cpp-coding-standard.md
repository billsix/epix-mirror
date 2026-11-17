# Apply the shared C++ coding standard to the whole C/C++ codebase

**Status:** proposed — needs go-ahead (open questions below)
**Priority:** 5
**Difficulty:** 8
**Created:** 2026-11-17 (William Emerison Six <billsix@gmail.com>)
**Related:** `tasks/full-repo-audit.md` (should run first); the cpp-coding-standard.md,
cpp-build-modernization.md, cpp-ownership-migration.md and porting-with-an-output-oracle.md reference
docs from github.com/billsix/runClaudeInContainer (`tasks/reference/`)

## BLUF

Bring every C/C++ source in the repo up to the maintainer's shared C++ coding standard
(github.com/billsix/runClaudeInContainer `tasks/reference/cpp-coding-standard.md`): tier (a), what
clang-format, clang-tidy and the compiler's warnings enforce, and tier (b), the judgment calls
(initialize and `const` by default, RAII and `unique_ptr` with raw pointers never owning, the four
parameter shapes, `enum class` and casts and integer types, one error strategy, modern spellings,
comments in the present tense). Done means the standard's tier (a) gates are configured and green, each
tier (b) area has been swept, and every sample's output is byte-identical to before.

## Context

- **Size:** 78 files in `src/`, 87 headers in `include/epix/` (about 16,900 lines of `.cc`), the nanobind
  binding `python/epix/_epix.cc`, and 86 files in `samples/` (`.xp`/`.flx`, the C++ the notebook ports
  are checked against).
- **Today's tooling:** `meson.build` sets `warning_level=3` and `cpp_std=c++20`; `.clang-format` (Google
  style) runs tree-wide from `entrypoint/format.sh`; `.clang-tidy` only disables three checks and turns
  nothing on, and no gate runs clang-tidy.
- **Behaviour must not change.** ePiX's output is eepic text, so the oracle is exact: compile every
  sample against the library before and after each step and diff the eepic byte for byte (the
  porting-with-an-output-oracle method). The port harness (`build-aux/verify_ports.py`) only shows the
  Python port and the C++ still agree with each other, so it is not enough on its own.
- **The public API is externally defined.** `.xp` figures, the samples, the manual, and the Python
  binding call ePiX's function and class names, so renaming a public name breaks every caller. The
  standard's "an externally-defined name wins" rule applies; names change only inside implementations.
- **It is a mirror.** A tree-wide restyle makes upstream diffs unreadable; record the mechanical
  reformat commits in `.git-blame-ignore-revs` (cpp-build-modernization.md).
- **Known gate state:** `make format` exits 1 on `HEAD` because of six ruff findings in
  `tools/one_process_check.py`; `tasks/full-repo-audit.md` covers it, and it should be green before
  this sweep relies on the gate.

## Proposed shape (an umbrella with step tasks, per the task conventions)

1. **The oracle first:** a script that builds every sample against a given libepix and stores its
   eepic, and a diff of two such runs. Every later step must leave the diff empty.
2. **Tier (a):** enable the clang-tidy checks the standard names (`modernize-*`, the relevant
   `cppcoreguidelines-*`, `bugprone-*`, `readability-*` subsets), add a `make tidy` gate, fix or
   narrowly suppress (with the reason at the site) what it reports.
3. **Ownership:** the tile and screen classes clone with `new` and own through raw pointers
   (`pen_arrow::clone`, `screen_data`'s `std::list<tile*> m_tiles`); migrate to `unique_ptr` per cpp-ownership-migration.md.
4. **Enums, casts, integers:** `enum class`, named casts, signed/unsigned boundaries under
   `-Wconversion -Wsign-conversion`, decided per boundary.
5. **The rest of tier (b):** initialization and `const`, parameter shapes, `auto` with intent,
   `constexpr`/`noexcept`, comments.

## Open questions

1. **Scope:** `src/`, `include/epix/` and `python/epix/_epix.cc`, leaving `samples/` alone (they are the
   byte-identity oracle and the user-facing examples)? Recommended: yes, samples excluded.
2. **Structure:** an umbrella task with one step task per area above, each committed and archived on its
   own? Recommended: yes; this is too large for one doc.
3. **Public names:** keep every public function and class name as it is, applying naming rules only
   to locals and private helpers? Recommended: yes.
4. **Order:** start only after `tasks/full-repo-audit.md` has produced its record? Recommended: yes.
