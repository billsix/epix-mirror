# Task: Investigate updating the C++ standard and modernizing syntax

**Status:** complete
**Completed:** 2026-06-09
**Summary:** C++20 + Tier 1/2 modernization + enum-class fix, output identical
**Requested:** 2026-06-09 (Bill)
**Owner:** Bill (via Claude)

## Decisions (2026-06-09)

- **Standard: C++20** (started at C++17, then bumped — C++20 gives `using enum`,
  which makes the enum-class fix keep terse call sites; fine for nanobind, which
  only needs ≥C++17). Pinned via `default_options: ['cpp_std=c++20']` in
  `meson.build`. Cost: raises the minimum build compiler (~GCC 11+) — fine here.
- **Scope: Tier 1 + Tier 2 + Tier 3 (`enum class`)** — all output-neutral.
- **Runtime `-std=c++20`** baked into the `epix`/`flix` compile of user `.xp`
  (`DEFAULT_STD` in `epix.in`; inline in `flix.in`); user flags still override.

## Implementation (2026-06-09)

Done mechanically with **clang-tidy modernize** via the meson compile DB
(`run-clang-tidy -fix`, header-filter `include/epix/`), which applies the fixes
correctly (it knows which methods actually override, which `0` are pointers,
etc.) — far safer than sed/hand-edits:

- **Tier 1** (`use-override`, `use-nullptr`, `use-using`, `use-equals-default`,
  `use-equals-delete`): **262 `override`**, 11 `typedef`→`using`, 10
  `=default`/`=delete`, 1 `nullptr`. Compiles clean at c++17.
- **Tier 2** (`loop-convert`, `use-auto`): **65 range-`for`**, **123 `auto`**
  (mostly iterator declarations + `new` expressions). Compiles clean.

Verified the library compiles clean at **strict** `c++17` (not just gnu++17) —
only 1 pre-existing warning remains: `intersections.cc:225` `double
normal_length();` is a most-vexing-parse (a behavioral oddity, **left alone** —
out of scope for syntax modernization).

**Verified:** rebuilt the container image (modernized lib at c++17) and
`make examples` rendered **96/97** — identical to the pre-modernization oracle;
the image's `epix` bakes `-std=c++17` for the user `.xp` compile. `warning_level`
left at 0 (mining was done via clang-tidy, not compiler warnings).

## Tier 3 — `enum class` (done 2026-06-09, C++20)

Converted all four `enums.h` types to `enum class` (kills the footgun: the
single-letter `epix_label_posn` values `c/r/t/l/b` no longer leak into user
scope via `using namespace ePiX;` and can't silently `int`-shadow a `double t`).

**Library:** `using enum <T>;` at namespace scope in the 9 `.cc` that use the
values (terse, no user pollution); qualified the default-args in the public
headers (`epix_label_posn POSN=epix_label_posn::none`, etc.).

**Samples + doc (.xp):** purely additive — the `.xp` are kept and still terse.
22 non-conflict samples got a one-line `using enum`; the rest were qualified
**only at the exact compiler-flagged occurrences** (so a genuine `double t`
variable like `log.xp:169` was left untouched). Done with a **clang** pass
(`-ferror-limit=0` reports *all* occurrences in one shot — gcc suppresses
repeats, forcing iteration), mapping each undeclared identifier to its enum and
inserting the qualifier at clang's byte-precise column: **61 sites across 24
files + 9 in `doc/marker.xp`**, 0 column mismatches.

**Output-identity proof:** rendered every figure to `.eepic` before (baseline)
and after, stripped the generation-timestamp header, and diffed —
**96 figures, 0 content differences** (scoped enums keep the same underlying
values, so geometry is byte-identical). `std_F.xp` is the only non-render (it's
non-standalone, as always).

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
  including user `.xp`. The real motivation is a genuine footgun: the single-
  letter `epix_label_posn` values (`none, c, r, t, l, b, …`) leak into every
  user's scope via `using namespace ePiX;` and collide with ordinary variables
  (the samples already declare `double t`/`double r`, which silently shadow the
  enum — plain enums convert to int, so it compiles wrong). **But** because the
  port is *additive* — the 70 `.xp` samples are **kept and must keep compiling**
  (clarified 2026-06-09) — the break can't be absorbed by the Python work; doing
  it means qualifying all 70 samples + ~13 library files, or a compat shim that
  re-introduces the pollution. So: deliberate, opt-in, compat-preserving change
  **decoupled from the bindings** — or leave it as the long-standing sharp edge.
- **Smart pointers / `std::array` / structured bindings** — only where they
  clarify; ePiX does little raw-pointer ownership, so likely small.

## Testing

The render oracle is the regression guard: after each tier, local
`meson compile` must stay clean and `make examples` must still
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

## Relationship to other tasks

This task couples tightly with `python-bindings-and-notebooks.md` — decide the
two together where they touch:

- **Standard choice gates the binding tech.** `nanobind` requires **C++17**;
  pinning C++17 here unlocks it (the leaner option for ePiX's large API).
  Make the "target standard" decision jointly with the bindings' "pybind11 vs
  nanobind" decision. Whatever is pinned is also the floor the binding module
  compiles at.
- **`enum class` (Tier 3) is DECOUPLED from bindings (corrected 2026-06-09).**
  pybind11/nanobind expose enums as **scoped** Python enums (`MarkType.PATH`)
  anyway, so the Python side gets scoping *for free in the binding layer* —
  without touching the C++ `enum`. And because the port is **additive** (the 70
  `.xp` samples are kept and must keep compiling), the earlier idea that "the
  Python rewrite absorbs the API break" is **void**: converting the C++ enums
  would break the retained samples. So `enum class` is **not** a binding
  prerequisite and shouldn't be sequenced with the bindings — it's a separate,
  opt-in, compat-preserving decision (qualify all call sites, or keep the bare
  names as aliases), or just leave it.
- **Bindings retire the "two standards" gotcha — for approach A only.** With
  real bindings, Python users never invoke `g++`, so the runtime `-std`
  coupling disappears for the Python path (it remains under codegen/approach B).
- **Lock output first.** The bindings task verifies each ported demo against the
  render oracle, so finish (and freeze) this task's *output-identical* work
  before the 81-demo port verifies against it — don't make them chase a moving
  baseline.
- **Shared inventory.** The public-API enumeration this task needs is the same
  one the bindings' Stage-0 demo audit needs — do it once.
- Minor: `normalize-repo-structure.md` is output-neutral and independent, but a
  clean `include/epix/` layout (that task) makes both this work and the bindings'
  includes tidier; structure-first is mildly favorable.

## Out of scope

- Algorithmic/behavioral changes — modernization is syntax/safety only, output
  must stay identical.
- The build-system or file-layout work (separate tasks).
