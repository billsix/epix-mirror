# Explainer: how port "byte compatibility" is verified, and how nested containers were used

**Status:** reference / retrospective (not deferred work) — explains the method
used while porting demos in `python-bindings-and-notebooks.md`.
**Written:** 2026-06-10 (Bill asked for this writeup)

---

## 1. First, a terminology clarification

The goal isn't *binary* (ABI) compatibility — it's **byte-for-byte identical
output**. A ported notebook "passes" when the figure it builds in Python produces
**exactly the same eepic text** as the original C++ sample. "eepic" is one of
ePiX's output backends: the LaTeX `\picture`-style macros (line segments, color
commands, boxed labels) that *are* the figure, as plain text.

We compare that **text**, not the rendered PNG. Text is deterministic; pixel
rasterization (LaTeX → dvips → ghostscript) has fuzz that would make pixel
comparison brittle. Comparing the eepic text is what makes "identical" a hard,
checkable claim.

## 2. The one insight that makes byte-identity *possible*

The Python bindings are **not a reimplementation of ePiX**. They are
[nanobind](https://nanobind.readthedocs.io) bindings that call straight into the
**same compiled `libepix`** the C++ samples link against. So when a notebook calls
`epix.plot(...)`, the eepic that comes out is produced by the *exact same C++ code
path* as the original sample — there is no second implementation that could drift.

Consequence: a port can be byte-identical because **the same code is generating the
bytes on both sides.** The only ways to diverge are therefore *porting bugs*, not
fundamental incompatibilities:

- wrong **call sequence** (ePiX is global-state — order of `pen`/`fill`/draw calls
  matters),
- wrong **arguments**,
- **floating-point expression-structure** differences in the *Python-side*
  arithmetic that feeds the calls — e.g. `5*M_PI_4` must be `5*(math.pi/4)`, not
  `5*math.pi/4` (different last bit), and `a + b + c + d` must keep the C++
  associativity. (See the "byte-identity" gotcha in `CLAUDE.md`.)

That's why a mismatch is almost always a one-line porting fix, and why the whole
effort is tractable.

## 3. How the original and the Python version are actually run and compared

This is the part that was unclear. It's all in **`build-aux/verify_ports.py`**, run
as `verify_ports.py NAME`. For a static figure `NAME` it does three things:

**(a) Run the Python candidate.** It `exec`s `notebooks/NAME.py` as ordinary
Python (a jupytext percent notebook *is* a valid `.py` — the markdown is comments,
the code cells just run). It grabs the resulting object from the notebook's
namespace (`fig` for a figure, `anim` for an animation) and reads its captured
eepic — which the binding produced via libepix's `print_eepic()` into a temp file.

**(b) Build + run the C++ oracle.** The original `samples/NAME.xp` is a complete
C++ `main()`. The harness:
  1. compiles it with `g++ -std=c++20 -DEPIX_FMT_EEPIC -I/usr/local/include
     -lepix` to a temp binary, then
  2. runs that binary and captures its **stdout** — which is the figure's eepic.

  The `-DEPIX_FMT_EEPIC` forces the eepic backend even for samples that call
  `pst_format()`/`tikz_format()` in-file, so the oracle is uniformly eepic and
  matches what the notebook captures.

**(c) Diff.** Strip the few volatile header lines (the "Generated …" /
"Automatically …" timestamp comments) from both, then do an exact string `==`.
`PASS` iff identical, else `FAIL (eepic mismatch)`.

```
 samples/NAME.xp ──g++ -DEPIX_FMT_EEPIC + run──▶ eepic_oracle ┐
                                                              ├─ strip headers, == ?
 notebooks/NAME.py ──exec → fig.eepic (print_eepic)──▶ eepic_py┘
```

**Animations (`.flx`)** are the same idea per frame: compile the `.flx` once, then
run `./bin i count` for each frame `i` (which sets the `tix()` counter to
`i/count`, exactly as the `flix` tool does), and diff each frame's eepic against
the Python `anim.frames[i].eepic`. The Python side mirrors this by `fork()`-ing a
fresh process per frame (ePiX accumulates global state with no public reset, so a
fresh process per frame is required — same reason `flix` runs a new process per
frame).

**One demo per process.** Because libepix accumulates global state across renders
(the color palette, etc.), each `verify_ports.py NAME` runs in its **own fresh
Python process** and `os._exit`s at the end. You verify one name per process; to do
several, you loop in the shell (see §5).

## 4. Why containers at all

None of the toolchain the harness needs lives on the bare host / Claude sandbox:

- `g++` + `libepix` installed at `/usr/local` (to compile and link the oracle),
- the nanobind extension `.so` built against the container's Python,
- (only for the *inline PNG render*, not for verification) the heavy TeX-Live +
  ghostscript + ImageMagick stack.

The project's `Makefile` bakes all of that into a Fedora 44 OCI image
(`make image`). I run that image **nested** (podman-in-podman) inside the Claude
sandbox. Two nested-specific things (from the global `CLAUDE.md`): inner runs carry
`--cgroups=disabled` (auto-applied via `PODMAN_RUN_FLAGS` since 2026-08-29; originally
hand-added because the sandbox's `/sys/fs/cgroup` was read-only), and the
inner image store is an **ephemeral tmpfs** — the image doesn't survive the
session, so it's a `make image` rebuild (~minutes, the TeX-Live layer) at the start
of each session.

## 5. Were the nested containers helpful — or inefficient? (the batching question)

**Helpful: yes, and effectively mandatory.** The entire toolchain is containerized;
the image bakes `epix` installed at `/usr/local`, so the oracle's `g++ … -lepix`
compile "just works" with no host setup. There was no realistic non-container path.

**But my first runs *were* inefficient, and I fixed it by batching — which is
exactly what you're asking about.** The cost breakdown of a single verify is:

- container **startup** (overlay mount + process init): the dominant cost,
- the **g++ compile** of the oracle (~a second),
- the actual run + diff: sub-second.

My very first verifications spun up a **separate `podman run` per demo** — including
running `butterfly` three times as three separate `podman run`s for a stability
check. That pays the container-startup cost *every single time*, which dwarfs the
sub-second verify.

I switched to **one `podman run` with a shell loop** over the demos:

```sh
podman run --rm --cgroups=disabled -v /epix:/epix:Z -e PYTHONPATH=/epix/python \
  epix -c 'for n in butterfly shadeplot clipping legend …; do
             python3 /epix/build-aux/verify_ports.py $n; done'
```

Each demo still gets a **fresh Python process** (required for libepix state
isolation — `verify_ports.py` `os._exit`s), but they all **share one container**.
That collapsed the overhead from *N container starts* to *1* per batch, and it's how
every batch after the first couple was run. So: the containers were helpful, my
initial usage was inefficient, and the fix was to batch the verifies inside a single
container.

**Other efficiency facts worth recording:**

- **Verification doesn't need TeX.** It only needs `g++` + `libepix` (it diffs eepic
  *text*). The TeX/ghostscript/ImageMagick stack is only for the inline PNG render
  (`_repr_png_`). So a verify-only loop is fast; the heavy stack is a one-time
  *image-build* cost, not a per-verify cost.
- **The `.so` rebuild is separate and cheap-ish.** After each `_epix.cc` change I
  ran `make py-ext PODMAN_RUN_FLAGS=--cgroups=disabled` (one container, a single
  `g++` of the binding) — independent of the verify loop.
- **The one unavoidable big cost** is `make image` once per session (ephemeral
  store). A persistent image cache, if the sandbox allowed it, would remove the
  per-session TeX-Live rebuild.

**What I could still do better:** after each new binding I rebuilt the `.so` and
re-ran the *entire* regression set (all ported-this-session demos), not just the new
one. That's some redundant re-verification on big batches. A tighter loop would
verify the **new** demo first, and run the full regression sweep **once at the end**
of a batch — trading a little early-regression-detection for fewer total runs. I
leaned toward the safer "re-verify everything each time" because the runs were cheap
once batched and it caught any cross-binding regression immediately.

## 6. TL;DR

- "Byte compatibility" = the notebook's **eepic text** is identical to the C++
  sample's, because **both run the same `libepix`** — the bindings aren't a
  reimplementation, so there's no second implementation to drift.
- `verify_ports.py NAME`: `exec` the notebook → eepic; `g++ -DEPIX_FMT_EEPIC` +
  run the `.xp` → oracle eepic; strip timestamp lines; exact string compare. One
  demo per process (libepix global state); `.flx` compared per frame.
- Nested containers were necessary and helpful (they hold the whole toolchain).
  The inefficiency was one `podman run` *per demo*; the fix was **batching all
  verifies into a single container's shell loop** (fresh Python process each,
  shared container). Verification needs only `g++` + `libepix`, not the TeX render
  path; the only big one-time cost is the per-session image rebuild.

## 6. TL;DR

- "Byte compatibility" = the notebook's **eepic text** is identical to the C++
  sample's, because **both run the same `libepix`**; the bindings aren't a reimpl.
- `verify_ports.py NAME`: `exec` the notebook → eepic; `g++ -DEPIX_FMT_EEPIC` +
  run the `.xp` → oracle eepic; strip timestamps; exact string compare. One demo
  per process (libepix global state); `.flx` compared per frame.
- Nested containers were necessary and helpful (they hold the whole toolchain).
  Running one `podman run` per demo was the inefficiency; **batching all verifies
  into a single container's shell loop** (fresh Python process each, shared
  container) was the fix. Verification needs only `g++` + `libepix`, not the TeX
  render path; the only big one-time cost is the per-session image rebuild.
```
