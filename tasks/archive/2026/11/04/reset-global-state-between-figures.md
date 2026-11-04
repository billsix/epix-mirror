# Reset ePiX's global state at the start of every `epix.figure()` — done-record

**Completed:** 2026-11-04. Requested by William Emerison Six <billsix@gmail.com>; implemented by the
agent. Found while building live-rendered ePiX notebooks for gacalc (github.com/billsix/geometricalgebra,
whose `tasks/frame-notebooks-epix.md` was blocked on this).

## The problem

libepix keeps its drawing state in process-global singletons, and nothing reset them between figures.
Drawing two figures in one Python process made the second inherit the first's pen, line style, fill,
label style, color palette, output format and camera. In the 2026-11-04 repro (a red, dashed line
figure, then a plain one), the second figure's eepic was byte-identical to the first. Calling
`pen`/`line_style("-")`/`nofill`/`label_color`/`label_angle` by hand at the top of the second figure
fixed its drawing commands but not the leftover `\xdefinecolor{rgb_ff0000}`, because no public call
cleared the palette. `animate()` had worked around this by forking per frame, which `figure()` cannot
do: the body of a `with` block runs in the caller's process.

## The inventory

Every mutable singleton is a function-local `static auto* x(new T())` returned by reference; a grep of
`src/` for that shape found these, and nothing else:

| accessor | file | holds | reset? |
|---|---|---|---|
| `the_picture()` | `picture.cc` | `picture_data`: canvas, page, unitlength, offsets, output format, palette, pre/post writes | by member (see below) |
| `xmin()`…`ysize()` | `picture.cc` | bounding box | no: `picture()` sets them |
| `the_tix` | `picture.cc` | animation parameter | no: `set_tix`/`animate()` sets it |
| `the_label_style()` | `label_style.cc` | font, text color, angle, mask, padding, border | yes |
| `the_path_style()` | `path_style.cc` | line style, dash size | yes |
| `the_paint_style()` | `paint_style.cc` | pen, base pen, fill | yes |
| `the_arrowhead_style()` | `arrow_style.cc` | arrowhead shape | yes |
| `the_mark_size()` | `marker_style.cc` | dot and tick sizes | yes |
| `the_angle_style()` | `angle_units.cc` | angle units | yes |
| `the_clip_box()` | `clipping.cc` | clip region | yes |
| `the_crop_box()` | `cropping.cc` | crop region | no: nothing outside its own file reads it |
| `the_screen_stack()` | `active_screen.cc` | `screen_broker`, seeded with `&the_picture().the_canvas` | yes, `reset()` |
| `cam()` | `camera.cc` | the 3D `Camera` | yes |

The `Color.cc` neutrals (`Neutral()`, `RGB_Neutral()`, …) also match the shape but return a copy, so
they cannot carry state.

## What was built

- **`void reset_state()`** (`include/epix/state.h`, `src/state.cc`) assigns a freshly constructed value
  to each style singleton marked "yes" above, then calls two new members:
  - **`screen_broker::reset()`** (`active_screen.{h,cc}`): back to the canvas alone, dropping any screen
    a figure activated and didn't deactivate;
  - **`picture_data::reset()`** (`picture_data.{h,cc}`): a fresh `eepic` output format, an empty palette
    and pre/post writes, and default unitlength and offsets. The canvas and page are left alone, since
    every `picture()` form replaces them.
- **Bound as `epix.reset_state()`** (`python/epix/_epix.cc`; `epix/__init__.py` re-exports it through
  its existing `from ._epix import *`). **`figure()` calls it before `picture()`**, and **`animate()`'s
  per-frame child calls it too**, because the fork copies whatever state the parent has accumulated.
  Manual-form code (`bounding_box`/`picture`/`begin`/`render`) calls `epix.reset_state()` itself, as
  `figure.py`'s module docstring says.
- **An upstream bug fixed along the way: `Camera::operator=`** (`src/camera.cc`) copied `the_distance`
  into `the_clip_range` and leaked the old lens, so a reset camera would have kept a near-clip plane at
  infinity instead of `MIN_CLIP`. It now copies the right field, deletes the old lens, and guards
  self-assignment. No sample, doc figure or notebook assigns a camera, so no existing output changed.
  Listed under `CLAUDE.md`'s "libepix bugs fixed here".
- `build-aux/asan_smoke.cc` draws a second picture after `reset_state()` (after moving the camera and
  switching to `tikz_format()`), so the reassignments run under the sanitizer.
- `CLAUDE.md` gained the "Global state is reset per figure" gotcha, and its port count was corrected
  from "80/81 byte-identical" to 79/81: `hello.py` is a display demo, not a port of `hello.xp`, and
  `histogram` was never ported.

## Decisions

1. **An explicit `reset_state()`, called by the Python `figure()`**, rather than an implicit reset
   inside C++ `picture()`. A `.xp` file may set styles before `picture()`, and an implicit reset would
   discard them (the maintainer: "whatever you recommend", William Emerison Six <billsix@gmail.com>,
   2026-11-04).
2. **Reset `picture_data` by member, not by assigning a fresh `picture_data()`** as the plan first
   proposed. A fresh `picture_data` has null-pimpl screens, and copying one dereferences null.
3. **`reset_state()` lives in `state.{h,cc}`**, not the new `src/reset.cc` the plan proposed: `state.h`
   already declares the imperative drawing-state calls (`pen()`, `fill()`, …), so the reset sits beside
   them and no new file enters `meson.build` or `make_header`.
4. **`the_crop_box()` was left out** rather than given a header declaration only so it could be reset.
5. **`animate()` keeps its fork**, now with a reset in the child: the fork still isolates `tix` and
   keeps one bad frame from poisoning the next.
6. **No `NEWS` entry.** `NEWS` is the upstream ePiX 1.2 file (last changed 2007) and records nothing
   about the Python layer; `CLAUDE.md` is where this repo documents the bindings.
7. **`picture_data::set_format` still leaks the previous format** (it assigns without deleting). It is
   upstream, harmless at the scale of one figure, and partly masked now that `reset()` deletes the format
   it finds. It was left as it is.

## Verification (in a rebuilt `epix` image, 2026-11-04)

The image had to be rebuilt (`make image`): the previous one predated the `/venv` layout, so
`make py-ext` failed.

- `make build`: `libepix` and the extension built with 0 warnings at `warning_level=3`.
- **The repro** (`tasks/adhoc/reset-global-state-between-figures/repro.py`): the plain figure drawn after
  the red, dashed one was byte-identical to the same figure drawn in a fresh process, palette lines
  included. PASS.
- **The byte-identity port harness** (`build-aux/verify_ports.py`, one notebook per process): **79/79
  ports PASS**. The two other notebooks are demos without an oracle and fail the harness regardless of
  this change: `build.py` (no `samples/build.xp`) and `hello.py` (an `epix.show` display demo whose
  `../samples/...` path resolves only from `notebooks/`).
- **The one-process check**
  (`tools/one_process_check.py all`, then under `tasks/adhoc/`): all 80 figure-building
  notebooks, run one after another in **one** process, were each byte-identical (every frame, for
  animations) to the same notebook run alone. **80/80**.
- **The control** (`one_process_check.py all --no-reset`, with `reset_state` stubbed out): the first
  notebook passed (nothing ran before it), the next **48 all differed** from their fresh runs, and then
  `pendulum`'s animation crashed ("animation frame 0 failed to render") on inherited state. So the check
  detects leaks, and the 80/80 is the reset's doing.
- `make asan`: "ASan smoke OK (no memory errors)".
- `make format` (clang-format + ruff; epix-mirror has no CI workflows, so this is the gate): passed and
  changed nothing; `clang-format --dry-run --Werror` was also clean on every edited C++ file.

## Archive

The archive move was its own commit, after the work commit. It `git rm`ed the one-shot
`repro.py` and `run_checks.sh` from `tasks/adhoc/reset-global-state-between-figures/`, and promoted
`one_process_check.py` to `tools/one_process_check.py` (at the maintainer's request) as a reusable
in-process-isolation check, documented in `CLAUDE.md` beside `tools/verify_image.sh` and in
`tasks/reference/verification-and-container-method.md`. It is not wired into any gate.
