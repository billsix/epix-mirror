# Add an ASan + UBSan(trap) build gate to the image

**Status:** proposed — needs go-ahead
**Created:** 2026-06-16

## Goal

Gate the container image build on an **AddressSanitizer (ASan)** + **UndefinedBehaviorSanitizer
(UBSan, trap mode)** build of epix's own C++ that exercises `libepix` under
both sanitizers and **fails the image on any memory error or undefined
behavior**. This makes the bug class that ASan already caught here once
(`screen::screen()` left its pimpl `m_screen` uninitialized — latent UB,
found via ASan; see `CLAUDE.md` "A libepix bug fixed here") a build-breaker
going forward, not a thing someone happens to notice.

This is the eePiX analogue of the gate the sibling **spimulator** project just
gained. Read that project's archived primer + rationale first — it explains
what ASan/UBSan are, why UB is dangerous even when the program "works," why
**trap mode is the reliable gate** (diagnostic UBSan *under-reports*), and how
the gate was wired into `Dockerfile`/`Makefile` behind a `RUN_SANITIZERS`
build-arg:
`/billopt/spimulator/tasks/archive/2026/06/16/ubsan-sweep.md`
(+ search `RUN_SANITIZERS` in spimulator's `Dockerfile`/`Makefile` for the
exact wiring this should mirror).

## The crucial difference from spimulator: what is there to run?

spimulator wires its gate onto an **existing `meson test` suite** — the image
already runs `meson test` and fails on any regression, so the gate is just "build
a second time under sanitizers and run that same suite." **eePiX has no such
suite.** Concretely:

- `meson.build` only builds `libepix.a` (a **static library**, no `main()`) and
  installs the four driver scripts. There are **no meson `test()` targets**.
- A static library can't be "run." Sanitizers only do anything when libepix is
  linked into an **executable** that then executes. So the gate needs to pick an
  executable that (a) links libepix and (b) exercises a broad swath of it.

The de-facto regression corpus in this repo is twofold, and neither is a meson
suite:

1. **The ~108 `samples/`+`doc/` figures** (`.xp` / `.flx`). The Python-port
   verifier `build-aux/verify_ports.py` already compiles each `.xp` with
   `g++ -DEPIX_FMT_EEPIC -lepix`, runs it, and diffs the eepic. Each compiled
   `.xp` is exactly an executable that links libepix and drives a lot of API.
2. **The DEV-ONLY ASan smoke** (`make asan` → `entrypoint/asan_check.sh` +
   `build-aux/asan_smoke.cc`): one hand-written `main()` that deliberately
   touches the whole bound-API surface, built `-Db_sanitize=address` and run.
   This *already is* a single-binary ASan gate — it's just (a) ASan-only, (b)
   not UBSan, (c) not wired into the image build, and (d) **marked for removal**
   when the bindings task closes (`CLAUDE.md` cleanup checklist).

**Recommendation — gate on the samples corpus, not (only) the smoke.** Building
each `samples/*.xp` (and `doc/*.xp`) under sanitizers and running it gives far
broader, real-usage coverage than the single smoke `main()`, and it's the same
set of programs the port already treats as the oracle. The smoke `.cc` is a
narrower, curated touch of the bound surface; it's a fine *additional* binary to
keep running, but on its own it under-covers.

## The Python-binding complication (call this out — it's the honest catch)

The prompt asks specifically about sanitizing the library when it's exercised
*via the Python bindings*. This is real and worth being explicit about, because
the naive "just sanitize the `.so`" does **not** work cleanly:

- The bindings (`python/epix/_epix.cc` + nanobind) build a **Python extension
  module** that statically links libepix (`entrypoint/build_py.sh` / `make
  py-ext`). To sanitize *that path* you'd build the `.so` with
  `-fsanitize=address`, then `import` it from the system `python3`.
- **The ASan runtime must be the first thing loaded in the process.** A normal
  `python3` is not ASan-instrumented, so importing an ASan `.so` into it fails
  unless you `LD_PRELOAD` the ASan runtime
  (`LD_PRELOAD=$(gcc -print-file-name=libasan.so) python3 …`). Get this wrong
  and you get the classic *"ASan runtime does not come first in initial library
  list"* abort, or silent non-instrumentation.
- Even with the preload, ASan will then watch **all of CPython**, not just
  libepix — and CPython's own allocator/interning/at-exit teardown produces
  noise (leaks especially) that has nothing to do with libepix. You'd be
  triaging interpreter noise, not library bugs.
- UBSan-**trap** through Python is worse for diagnosis: a trap is a `SIGILL`
  that aborts the interpreter with no Python traceback and no source line — you
  can't tell *which* bound call tripped it without a separate gdb run.

**Conclusion:** the Python-binding path is the **wrong thing to gate on.** It
adds an `LD_PRELOAD` dance, drags the whole interpreter under the sanitizer, and
gives worse diagnostics — all to instrument the *same* `libepix` C++ that the
compiled `.xp` oracles already instrument directly and cleanly. Gate on the
**pure-C++ executables** (the `.xp` samples, optionally the smoke `.cc`); the
Python layer is a thin call-through to the same library, so sanitizing the C++
side covers the code that matters. (If a *binding-specific* C++ bug is ever
suspected — e.g. in trampolines or lifetime handling in `_epix.cc` — that's a
targeted, manual ASan-via-`LD_PRELOAD` investigation, not a standing gate.)

## Meson C++ sanitizer flags (scoped to epix's own C++)

Mirror spimulator's two-build approach, in **C++** terms, in a throwaway build
dir so nothing lands in the tree:

- **UBSan trap (the gate):**
  ```sh
  CC=clang CXX=clang++ meson setup /tmp/san-ubsan /epix --buildtype=debug \
      -Dcpp_std=c++20 \
      -Dcpp_args='-fsanitize=undefined -fsanitize-trap=undefined' \
      -Dcpp_link_args='-fsanitize=undefined -fsanitize-trap=undefined'
  meson compile -C /tmp/san-ubsan          # builds libepix.a under UBSan-trap
  ```
- **ASan:**
  ```sh
  CXX=clang++ meson setup /tmp/san-asan /epix --buildtype=debug \
      -Dcpp_std=c++20 -Db_sanitize=address
  meson compile -C /tmp/san-asan           # builds libepix.a under ASan
  ```
  (`-Db_sanitize=address` is exactly what `entrypoint/asan_check.sh` already
  uses today.)

**Scope it to epix's own C++.** Like spim's gate scoped to `src/` and excluded
the `-nostdlib` examples, here:
- The flags go on **libepix's compile** (`src/`, `include/epix/`) — that's all
  first-party epix C++; there is no vendored C++ in the library to worry about.
- **nanobind's `nb_combined.cpp`** (vendored, pulled in only by `build_py.sh`)
  is *not* part of the meson `libepix` build, so the meson-scoped flags never
  touch it — good. Do **not** sanitize the Python extension build (per the
  complication above).
- The driver scripts (`epix`/`elaps`/`flix`/`laps`) are shell, not compiled —
  irrelevant.

Because the gate uses a **separate `meson setup`** with sanitizer args, the
normal install build in the Dockerfile (`meson setup /tmp/build … && meson
install`) is unaffected — the shipped `libepix.a` stays uninstrumented.

## What the gate actually runs

Build the sanitized `libepix.a`, then for each gate-binary compile it against
that sanitized lib and run it under the sanitizer, failing on any abort:

```sh
# pseudo, run for BOTH /tmp/san-ubsan (trap) and /tmp/san-asan (address):
for xp in samples/*.xp doc/*.xp ; do
    clang++ -std=c++20 -DEPIX_FMT_EEPIC -Iinclude/epix \
        <sanitizer flags> "$xp" "$BUILD/libepix.a" -lm -o /tmp/gate.bin
    /tmp/gate.bin >/dev/null            # SIGILL (UB) or ASan abort => fail image
done
# plus optionally build-aux/asan_smoke.cc as one extra binary.
```

Notes:
- This reuses the **same compile recipe `verify_ports.py` already uses** for the
  oracle (`-DEPIX_FMT_EEPIC`, link `libepix`), just with sanitizer flags and
  linking the *sanitized* `libepix.a` instead of the installed one. Consider
  factoring a tiny driver script (`entrypoint/sanitizer_gate.sh`) that loops the
  corpus, rather than open-coding a long `RUN` in the Dockerfile.
- The **three `.flx` broken upstream** and the `histogram` sample that can't even
  compile/run as a C++ oracle (see `CLAUDE.md`) must be **excluded** from the
  loop, exactly as they're excluded from the port verification — otherwise the
  gate fails on pre-existing non-sanitizer breakage. Enumerate the runnable set
  the same way the port does.
- Run **UBSan-trap as the authoritative pass/fail gate** (no runtime to link in
  the Fedora image — low friction); use diagnostic `-Db_sanitize=undefined` only
  when *locating* a site (it under-reports, per the spim sweep, and its runtime
  fought the link there). Pair traps with gdb backtraces to pinpoint.

## ASan leak note (LSan default-off)

libepix accumulates global state with **no public reset** (the color palette
`picture_data::m_palette`, etc. — see `CLAUDE.md` "Animations must `fork()` per
frame") and the `.xp` programs exit without tearing it down, so LeakSanitizer
(bundled in ASan) will report **intentional exit-time leaks** that are not the
corruption bugs we're gating on. `entrypoint/asan_check.sh` already sidesteps
this with `ASAN_OPTIONS=detect_leaks=0`.

Mirror spim's approach for robustness: default LSan **off** so the gate is about
corruption, not leaks. Two equivalent options —
- the **env** form the smoke already uses: run the gate binaries with
  `ASAN_OPTIONS=detect_leaks=0`, **or**
- the **in-binary, ASan-guarded weak hook** spim uses, added to a small gate
  TU (or `asan_smoke.cc`): `extern "C" const char*
  __asan_default_options(){ return "detect_leaks=0"; }` under
  `#if defined(__SANITIZE_ADDRESS__) || __has_feature(address_sanitizer)`.

The env form is simplest for a loop over many `.xp` binaries; prefer it.

## In-container-only constraint

All of this runs **inside the epix image**, per the working arrangement — never
on the host. Nested-podman caveats apply: every inner `podman run`/`build` needs
`PODMAN_RUN_FLAGS=--cgroups=disabled` / `PODMAN_BUILD_FLAGS=--cgroups=disabled`
(already threaded through the `Makefile`), and the inner image store is a small
tmpfs (`podman image prune -f` between rebuilds). The gate adds **no new image
dependency**: `libasan` is already in the `Dockerfile` and trap-UBSan needs no
runtime. clang is the one thing to confirm — the image installs
`clang-tools-extra` (clang-format/tidy) but **not necessarily `clang`/`clang++`
the compilers**; verify, and if absent add `clang` to the `Dockerfile` dnf list
(small, permanent — it's a real toolchain dep for this gate). gcc's
`-fsanitize=undefined -fsanitize-trap=undefined` also works if we'd rather not
add clang; pick one and note it.

## Wiring (mirror spimulator)

- **`Dockerfile`:** add `ARG RUN_SANITIZERS=0` (lean default for a bare
  `podman build`), and a `RUN if [ "$RUN_SANITIZERS" = "1" ]; then … fi` block
  that does the two sanitized `meson setup`/`compile` + the gate loop, then
  `rm -rf` the throwaway build dirs. Place it after the existing
  `meson install` so a failure clearly attributes to the gate.
- **`Makefile`:** add `RUN_SANITIZERS ?= 1` and pass
  `--build-arg RUN_SANITIZERS=$(RUN_SANITIZERS)` on the `image` target (so
  `make image` enforces the gate; `make image RUN_SANITIZERS=0` skips). Keep the
  `0` Dockerfile default / `1` Makefile default split, per the family template.
- Optionally keep a standalone `make sanitize` (or fold into/replace the
  existing dev-only `asan` target) for running the gate without a full image
  rebuild during development.

## Relationship to the existing DEV-ONLY ASan tooling

`make asan` + `entrypoint/asan_check.sh` + `build-aux/asan_smoke.cc` + the
`libasan` Dockerfile line are currently **marked for removal** when the bindings
task closes (`CLAUDE.md`; the bindings task doc cleanup checklist). This gate
**supersedes and generalizes** that tooling (ASan→ASan+UBSan, one curated
`main()`→the whole samples corpus, dev-only→image gate). Decide explicitly:
- **promote**: keep `libasan` (now permanent, not dev-only), retire/replace the
  smoke-only `asan` target with the broader gate, and update the "DEV-ONLY /
  REMOVE" notes in `CLAUDE.md` + `Dockerfile` accordingly; **or**
- **keep both**: gate = the new image lane, `make asan` = a fast local smoke.

Whichever, the `CLAUDE.md` removal note for the ASan tooling must be reconciled
so it doesn't say "remove libasan" while the gate depends on it.

## Acceptance

- `make image` (with `RUN_SANITIZERS=1`, the default) builds the gate's
  sanitized `libepix.a` twice (UBSan-trap + ASan), compiles the runnable
  samples/doc corpus against each, runs them, and **fails the image** on any
  SIGILL (UB) or ASan abort. `make image RUN_SANITIZERS=0` skips it.
- The corpus excludes the known-unrunnable demos (the 3 broken `.flx`,
  `histogram`) the same way port verification does — the gate fails only on
  *sanitizer* findings, not pre-existing breakage.
- LSan defaulted off (corruption-only gate); the rationale recorded here.
- Any UB/memory site the first run surfaces is fixed smallest-diff-first
  (behavior-preserving) and noted (file:line, what + why) in this doc before
  archiving — same discipline as the spim sweep.
- Decision recorded on promote-vs-keep-both for the existing `make asan`
  tooling, and on clang-vs-gcc for the UBSan build, with `CLAUDE.md` /
  `Dockerfile` notes reconciled.
- Normal (non-sanitized) install build and the Python bindings/notebooks are
  unaffected — the shipped `libepix.a` stays uninstrumented.

## References

- spimulator gate (template): `/billopt/spimulator/tasks/archive/2026/06/16/ubsan-sweep.md`
  + `RUN_SANITIZERS` in spimulator's `Dockerfile`/`Makefile`.
- epix verification method (the corpus + compile recipe this gate reuses):
  `tasks/verification-and-container-method.md`, `build-aux/verify_ports.py`.
- Existing dev ASan tooling: `entrypoint/asan_check.sh`, `build-aux/asan_smoke.cc`,
  `make asan`, the `libasan` line in `Dockerfile` (all marked DEV-ONLY today).
- The libepix bug ASan already caught: `screen.h` `m_screen` initializer
  (`CLAUDE.md`, "A libepix bug fixed here").
