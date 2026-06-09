# Task: Investigate updating the C++ standard and modernizing syntax

**Status:** proposed — investigation done 2026-06-09, awaiting go-ahead
**Requested:** 2026-06-09 (Bill)
**Owner:** Bill (via Claude)

## Goal

Decide a target C++ standard for ePiX, pin it in the build, and scope which code
should be rewritten with newer syntax. Investigate-and-recommend; the rewrites
themselves are a follow-on once the scope/standard are chosen.

## Scope note: ePiX is C++-only

There is **no C code** — 79 `.cc`, 0 `.c`. So **C23 doesn't apply** to the
library itself. (The only place a C/C++ standard surfaces for *user* code is the
runtime compile of `.xp` files; see "Two standards" below.) The rest of this
task is about the C++ standard.

## Current state (from investigation)

The library is essentially **C++03-era style that happens to compile under
modern compilers** (verified: builds clean under gcc 16). Markers across
`*.cc`/`*.h`:

| Signal | Count | Reading |
|---|---|---|
| `cpp_std` pinned in `meson.build` | none | uses the compiler default (gcc 16 ⇒ gnu++17) — **unpinned = non-reproducible** |
| `virtual` methods | 98 | … |
| `override` | **3** | ~95 overrides **not** marked — top safety/clarity win |
| `nullptr` | 0 | null pointers via `0`/implicit — pre-C++11 |
| `auto` | 0 | no type inference anywhere |
| `typedef` | 11 | → `using` aliases |
| `throw()` / `auto_ptr` / `register` | 0 | good — no *removed* constructs to fix |
| C-style `for(;;)` loops | 254 | a subset are range-`for` candidates |
| `#ifndef` include guards | 87 files | could be `#pragma once` (minor) |
| old-style `enum` | 4 (`enums.h`) | `enum class` candidates **but see risk** |

## Two standards to set (don't forget the second)

1. **The library's own standard** — set `default_options: ['cpp_std=c++NN']` in
   `meson.build`. Pins reproducibility and unlocks the syntax below.
2. **The runtime compile of user `.xp`** — the `epix`/`flix` scripts invoke
   `@RUNTIME_COMPILER@` (g++) with **no `-std`** (confirmed in `epix.in`), so
   user figures compile at the compiler default. **If the installed headers
   (`epix.h`) start using C++NN features, user code must compile at ≥ C++NN
   too** — so the scripts (or a baked default flag) should pass the matching
   `-std`. This coupling is the main correctness gotcha of the whole effort.

## Recommended target

**C++17 as the floor** (`cpp_std=c++17`): universally supported, zero-risk for
this code, and enough for every high-value modernization below. Optionally
C++20 (concepts/ranges/`std::span`) later, but C++20 raises the minimum compiler
and buys little for this codebase. **C++23 is not worth the compiler-version
cost here.** Recommend: pin C++17 now; revisit C++20 after.

## Modernization candidates, by priority

**Tier 1 — mechanical, safe, high value (do first):**
- **`override` on the ~95 unmarked overrides** — compiler-checked correctness,
  catches signature drift. Biggest single win.
- **Pin `cpp_std=c++17`** in `meson.build` (and add the matching `-std` to the
  runtime compile per "Two standards").
- **`nullptr`** for null pointers.
- **`typedef` → `using`** (11 sites).
- **`= default` / `= delete`** for special members where applicable.

**Tier 2 — judgment, per-site review:**
- **Range-based `for`** where a C-style loop just walks a container (subset of
  254); leave index-arithmetic / numeric loops alone.
- **`auto`** for verbose iterator/declaration noise (not as a blanket).
- **`constexpr`** for the constants in `constants.h`.
- **Brace/`{}` init, in-class member initializers.**

**Tier 3 — riskier / API-affecting (separate decision):**
- **`enum class` for `enums.h`** — **API-breaking**: the enumerators (`PATH`,
  `CIRC`, `c`, `r`, `LEFT`, …) are used **unqualified** and are part of the
  public/user-facing API; scoping them forces `epix_mark_type::PATH` everywhere
  including user `.xp`. Treat as a deliberate API change, not a sweep.
- **Smart pointers / `std::array` / structured bindings** — only where they
  clarify; ePiX does little raw-pointer ownership, so likely small.

## Testing

The render oracle is the regression guard: after each tier, local
`meson compile` must stay clean and `make -f Makefile.docker examples` must still
render **96/97** (eepic) with byte-similar output, plus the PDF/manual paths.
Turning **`warning_level` up** during the work (currently 0) surfaces
modernization targets the compiler can flag (e.g. `-Wsuggest-override`).

## Decisions for Bill

1. **Target standard** — C++17 (recommended) / C++20 / C++23?
2. **Scope** — Tier 1 only (safe sweep), or also Tier 2 (loops/`auto`)? Tier 3
   (`enum class`) is opt-in given the API break.
3. **Runtime `-std`** — bake a matching `-std` into the `epix`/`flix` compile so
   user `.xp` matches the library standard? (Recommend yes, same value.)
4. **`warning_level`** — bump from 0 during the work to mine warnings, then
   settle on a level?

## Effort

- Tier 1: **~1 session**, mostly mechanical + the run-the-oracle loop.
- Tier 2: **~1–2 sessions**, per-site judgment.
- Tier 3: separate, decision-gated.

## Out of scope

- Algorithmic/behavioral changes — modernization is syntax/safety only, output
  must stay identical.
- The build-system or file-layout work (separate tasks).
