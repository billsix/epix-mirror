# Task: Fix "Pending"-instead-of-figure for heavy notebooks (LaTeX main_memory)

**Status:** DONE (2026-06-11). Fixed via **runtime env vars passed to the `elaps`
subprocess in `render.py`** — NOT a Dockerfile/`texmf.cnf` edit (Bill pushed back on
modifying a distro-managed config; a cleaner runtime override exists). All 5
affected figures now render, and **byte-identity is preserved** (the change touches
only the PNG render subprocess, not `print_eepic`). So we keep eepic everywhere +
byte-identical *and* everything renders — the outcome Bill preferred.
**Requested:** 2026-06-11 (Bill)
**Owner:** Bill (via Claude)

## RESOLUTION (2026-06-11)

- **Survey (current image) found 5 offenders:** `bowl`, `levelset3`, `spherical`,
  `surface_shade`, `stereo_proj` (2/9 frames). (Guessed `helicoid`/`lighting`/`log`
  do **not** overflow — empirical beat the guess. `hello` ERROR'd in the survey
  harness only because `runpy` from `/epix` mis-resolves its `../samples` path; it
  is a Phase-1 `show()` demo, not a figure.)
- **The clean fix (no config edit, no format rebuild, no `main_memory` change):**
  `extra_mem_top`/`extra_mem_bot` extend TeX's pool **at runtime**, and kpathsea
  reads them straight from the **environment**. Verified: `extra_mem_top=…
  extra_mem_bot=…` makes `surface_shade` render in ~6 s on the *unmodified* image.
  Landed in `python/epix/render.py` as `_TEX_MEM_ENV` passed to the `elaps`
  `subprocess.run(env=…)` — so it travels with the package (works for a pip install
  on any host, not just our container).
- **Verified:** all 5 → render OK with only the `render.py` change (no shell env, no
  rebuild). Byte-identity untouched (eepic capture is independent of the render
  subprocess).
- **Dockerfile reverted** — the earlier `texmf.cnf` + `fmtutil-sys` approach was
  removed in favor of the env-var override.
- **Fallback repr (optional):** not done; the figures now render, so the confusing
  `_Pending` repr no longer appears. Could still add a friendly message for any
  future pathological figure — left as a possible follow-up, not needed now.

---
_Original investigation below (kept for the cause/`main_memory` writeup)._

## Symptom

Some notebooks display the text `<epix.figure._Pending object at 0x…>` (the
"Pending" text) instead of an image/animation.

## Cause (confirmed)

The `with epix.figure(...) as fig:` context manager renders **synchronously** on
block exit — so by the time `fig` is shown, the render has already finished. It is
**not** a "wait longer" situation.

For a few **very heavy** figures (tens of thousands of facets; `surface_shade`'s
eepic is ~129 K lines), the rasterization step (`elaps` → LaTeX → dvips → eps →
ghostscript → PNG) overflows TeX's statically-sized **`main_memory`** pool and dies
with *"TeX capacity exceeded … [main memory size=…]"*. Rather than crash the
notebook, `render()` returns a `Figure` with the **correct eepic but an empty PNG**
(`b""`); `Figure._repr_png_` then returns `None`, so Jupyter falls back to the
`_Pending` object's default repr — the "Pending" text.

The byte-identity port harness is unaffected (it diffs **eepic text**, not pixels),
which is why these notebooks still PASS verification.

## Why the C++ originals don't hit this

ePiX has three output backends — eepic, **PSTricks**, **TikZ**. eepic renders the
figure as a giant pile of LaTeX `picture` macros (`\drawline`/`\put`); PSTricks/TikZ
push the drawing down to PostScript / a compact graphics layer. The heavy demos'
C++ sources call `pst_format()` (PSTricks), so run natively they go through
PostScript and render fine. **Our notebook path forces eepic** (`-DEPIX_FMT_EEPIC`,
for byte-identity with the oracle), so it's the path that feeds LaTeX the giant
macro pile.

## What `main_memory` is

TeX (what LaTeX runs on) has statically-sized memory pools set in `texmf.cnf` and
baked into the precompiled "format" files. `main_memory` is the big pool holding the
document's boxes/macros/token-lists. A 129 K-line eepic `picture` builds an enormous
box/token structure that overflows the default. Raising it (and friends:
`extra_mem_top/bot`, `save_size`, `pool_size`) and rebuilding the formats
(`fmtutil-sys`) lets these figures rasterize.

Image facts (probed 2026-06-11): `texmf.cnf` =
`/usr/share/texlive/texmf-dist/web2c/texmf.cnf`; current `main_memory = 6000000`,
`extra_mem_top = 0`, `save_size = 200000`.

## Affected notebooks (from the empty-PNG survey, 2026-06-11)

> Method: render every `notebooks/*.py` through the full TeX path in the container
> and flag any whose `fig.png` (or animation frames) come back empty.

**(filled in after the survey completes — see EXECUTION LOG)**

Known from the port history: `surface_shade` (129 K-line eepic), and the heavy
`.flx` animations `helicoid` / `stereo_proj`; likely also dense-facet demos
(`lighting`, `decorate`, `log`).

## Fix (chosen: bump main_memory in the Dockerfile)

After the TeX-Live install in `Dockerfile`, raise the pools and rebuild the latex
format:

```dockerfile
# Heavy eepic figures (e.g. surface_shade's ~129K-line picture) overflow TeX's
# default main_memory when rasterized via elaps; raise the pools + rebuild the
# format so they render inline. (C++ originals sidestep this via PSTricks.)
RUN cnf="$(kpsewhich texmf.cnf)" && { \
      echo 'main_memory = 60000000'; \
      echo 'extra_mem_top = 10000000'; \
      echo 'extra_mem_bot = 60000000'; \
      echo 'save_size = 1000000'; \
      echo 'pool_size = 12500000'; \
    } >> "$cnf" && fmtutil-sys --byfmt latex
```

(`--byfmt latex` because `elaps` runs DVI `latex` → dvips; broaden to `--all` if a
path needs pdflatex.)

### Complementary (recommended, cheap): friendlier fallback repr

Even with a bigger pool, a pathological figure could still fail. Give `Figure`
(and/or `_Pending`) a `__repr__`/`_repr_html_` that, when the PNG is empty, prints a
clear message instead of the cryptic object repr — e.g.
`"⚠ figure too large to rasterize at dpi=N (eepic captured, M lines) — try a lower
dpi"`. Decide whether to include this now or as a follow-up. (Not required by the
main_memory fix, but it converts a confusing state into an explained one.)

## Plan

1. **Survey** (done) — list the affected notebooks (above).
2. **Dockerfile bump** — add the `texmf.cnf` + `fmtutil-sys` step.
3. **Rebuild** the image (`make image`; nested-podman tmpfs caveat — `podman image
   prune -f` if space runs low) and **re-survey** — confirm the affected list
   shrinks (ideally to empty); record any that still fail (candidates for the
   fallback repr / a lower default dpi).
4. (Optional) add the fallback repr.
5. Docs: note the image's raised TeX memory in `CLAUDE.md` (Build / container) and
   `README.md` if relevant. `make format` if any `.py` changed.

## Verification / build-vs-runtime

The survey + re-survey are **automated** (they check whether `fig.png` is empty
bytes — no visual inspection), so Claude can run them in the container end to end.
Bill's eyeball is only needed if we want to confirm the *rendered images look right*
(separate from "the PNG is now non-empty").

## Relationship to other tasks

- Independent of the open notebook/packaging tasks. Touches only the `Dockerfile`
  (+ optionally `figure.py`/`render.py` for the fallback repr).
- The port history's "future polish — bump LaTeX main_memory" note
  (`tasks/archive/2026/06/11/python-bindings-and-notebooks.md`) is exactly this.
