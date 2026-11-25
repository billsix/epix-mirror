# Apply the 2026-11-06 naming and scope rules across epix-mirror

**Done:** 2026-11-25 (work commit; the archive move follows). Source: the maintainer (William Emerison Six <billsix@gmail.com>, 2026-11-06), who asked for the survey in geometricalgebra and then "add a similar task to every other python project in opt"; run on 2026-11-25 as the fifth repository of six.

## What this did

Five rules entered the shared Python standard on 2026-11-06 (github.com/billsix/runClaudeInContainer, `tasks/reference/python-coding-standard.md`: "Inline a value used once, and name it in a comment instead of a binding" and "What earns an extraction"): inline a binding used once; a binding used twice does not automatically earn a name; declare a name at the point of use; nest a helper with one caller inside that caller; shadow deliberately when nesting. The task surveyed the Python bindings layer (`python/epix/*.py`), the 81 jupytext notebooks (`notebooks/*.py`) and `tools/*.py`, and refactored without changing behaviour. The upstream C++ and the vendored Emacs tree are out of scope.

## Decisions

- **Survey first, then discretion; a helper a test imports stays at module level** (William Emerison Six <billsix@gmail.com>, 2026-11-24, for the mvp task; applied to all six repositories).
- **Survey:** `tasks/adhoc/apply-2026-11-06-naming-and-scope-rules/discover.py` wrote 159 leads: 54 `ONCE`, 62 `TWICE`, 3 `FAR`, 40 `ONE_CALLER`.
- **Nested (rule 4):** 34 one-caller helpers in 21 notebooks (for example `F` into `build` in `steiner.py`, `pendulum.py`, `riemann.py` and `lighting.py`; `cubic12`, `cubic13`, `cubic23`, `cubic` and `bars` into `draw_frame` in `twisted_cubic.py`; the cascade `sphere_draw` into `icosa` into `build` in `mirrorball.py`), plus `_eps_to_png` and `_flatten` into `_render_via_elaps` in `python/epix/render.py`, and `strip` into `eepics` into `main` in `tools/one_process_check.py`. A second run of both scripts nested nothing.
  - **The notebooks needed their own run.** `nest_one_caller.py` skipped 21 of them with "named in another file", because the same one-letter names (`F`, `f`, `g`, `color`) are each notebook's own helper. The harness `exec`s a notebook, and nothing imports one, so no other file is a caller. `tasks/adhoc/apply-2026-11-06-naming-and-scope-rules/nest_notebooks.sh` nests each notebook alone in a scratch repository, where the tool's cross-file check has nothing to match.
  - **`render` and `fit_bounds` in `figure.py` stay at module level:** they are named in other files (`epix.render`, public API), which is the tool's skip working as intended.
- **A bug in the shared tool was found and fixed.** When a second helper nested into a host that had just received one, the tool inserted it between the first helper's comment and its `def`, so a comment such as `# re cos(u+iv)` ended up above the wrong function. The insertion point is now above the host's first statement and the comments directly above it. The fix is in `tasks/reference/nest_one_caller.py` in github.com/billsix/runClaudeInContainer and in the byte-identical copies in github.com/billsix/runCrushInContainer (its `tasks/reference/` and its baked client copy); staged there, not committed. The mvp repository's cascade into `frame` in `modelviewperspectiveprojection.py` was nested before the fix, so its comments may be misplaced and are worth one look.
- **Inlined (rule 1):** `size` in `fit_bounds` (`python/epix/figure.py`), with a comment naming it.
- **Import blocks:** five import blocks now need one blank line, not two, because the module-level helpers that followed them nested away; `ruff check --select I001 --fix` on the changed files only.
- **Left alone, with the reason:**
  - The notebook scene names (`ambient`, `fog`, `spot`, `chip_color`, `dens_r`, `octagon_points` and the rest of the 40-odd `ONCE` leads in `lighting.py`, `log.py`, `helicoid.py`, `steiner2.py`, `plotting3.py`, `legend.py` and others): `CLAUDE.md` keeps the ported figures readable as scene descriptions that match the C++ samples they port, and a binding that does so may stay.
  - Frame and loop values named after the sample (`t` in `cube.py`, `steiner.py`, `steiner2.py` and `trig.py`).
  - `with ... as f` targets, `mkstemp` and `waitpid` tuple unpacks (`fd`, `status`, `d`).
  - `can_render` in `animate` (computed once before the frame loop; inlining it would run `shutil.which` three times per frame) and `paths` in `animate`.
  - `box_width` and `box_height` in `fit_bounds` (inlined into the size string they would make one unreadable line), `b64` in `_repr_html_`, and `together` and `alone` in `one_process_check.py` (named for the comparison they feed).
  - Every `TWICE` lead: GL-free scene values, loop state and file handles; no `getattr` or `x if x is not None else y` shape remained to collapse.

## Result

- Applied: 38 nestings, one inline, and the import-block spacing; 24 files changed. Behaviour is unchanged (see Verification).
- `python/epix/_epix.cc`, the C++ and the committed stub are untouched.

## Verification (containerized, default flags, 2026-11-25)

- The byte-identity harness (`build-aux/verify_ports.py`, one process per notebook) over all 81 notebooks: 79 PASS before and 79 PASS after, and the same two expected FAILs (`build`, which has no sample, and `hello`, a display demo); the per-notebook results are identical.
- `tools/one_process_check.py all` (every notebook back to back in one process against a fresh run each): 80/80 identical.
- `ruff format` and `ruff check` with `python/pyproject.toml` on the 24 changed files: no findings, nothing left to reformat. The repository's own `make format` was not run, because its `ruff check --fix` and `clang-format` rewrite files tree-wide; the 24 files were checked alone, and they had no findings before the change either.
- Not exercised: the inline PNG path (`_flatten`, `_eps_to_png` now nested in `_render_via_elaps`) and `animate()`, which need the TeX and ghostscript render stack that the harness does not use. They are in the full image; a notebook displayed in JupyterLab is the check.

## Owed

- The maintainer looks at one figure rendered inline in JupyterLab (`make jupyter`, any notebook with a background, for example `hello`) to confirm the nested `_flatten` and `_eps_to_png` still produce a PNG.
- The `nest_one_caller.py` fix is staged in runClaudeInContainer and runCrushInContainer, not committed.
