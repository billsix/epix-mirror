# Reset ePiX's global state at the start of every `epix.figure()`

**Status:** proposed — needs go-ahead
**Priority:** 3
**Difficulty:** 4
**Created:** 2026-11-04 (William Emerison Six <billsix@gmail.com>). Found while building live-rendered
ePiX notebooks for gacalc (github.com/billsix/geometricalgebra, its
`tasks/frame-class-orthonormal-invertible-function.md`).

## BLUF

Drawing two figures in one Python process makes the second one inherit the first one's pen, line style,
fill, label style, color palette, camera, and so on, because libepix keeps all of that in process-global
singletons and nothing resets them between figures. Fix: a C++ `reset_state()` (bound as
`epix.reset_state()`) that restores every singleton to its startup default, called by `epix.figure()`
before `picture()`. Done = a notebook can draw any number of figures in one kernel, each byte-identical
to drawing it alone in a fresh process, and the existing byte-identity port harness still passes.

## Context (read first)

- `python/epix/figure.py`: `figure()` calls `_epix.picture()` + `_epix.begin()`, then captures with
  `print_eepic`. Its docstring says "One figure at a time (matches ePiX's global model)". `animate()`
  already works around the problem by **forking a child per frame** ("ePiX accumulates per-render state
  (the color palette, etc.) with no public reset"). Forking is not available to `figure()`, because the
  body of a `with` block runs in the caller's process.
- gacalc's renderer (`tools/render_epix_figures.py` there) runs each figure in a fresh interpreter for
  the same reason.
- `CLAUDE.md` › "Python bindings + notebooks": the byte-identity harness `build-aux/verify_ports.py NAME`
  and the smoke gate `make shell-exec SCRIPT=tools/verify_image.sh`.

## The bug, reproduced (2026-11-04, in gacalc's image)

```python
def draw(red):
    with epix.figure(lower_left=Point(x=-1, y=-1), upper_right=Point(x=1, y=1), size="2x2in") as fig:
        if red:
            epix.pen(color=epix.rgb(r=1, g=0, b=0), width=3)
            epix.dashed()
        epix.line(tail=Point(x=-1, y=0), head=Point(x=1, y=0))
    return fig
draw(True); b = draw(False)     # b's eepic is byte-identical to the red, dashed figure's
```

Drawn alone in a fresh process, `draw(False)` gives a different (plain) eepic. Calling
`pen`/`line_style("-")`/`nofill`/`label_color`/`label_angle` by hand at the top of the second figure
makes its drawing commands match a fresh process. The leftover `\xdefinecolor{rgb_ff0000}` stays,
though, because the palette is not reset by any public call.

## Where the global state lives (from a read of `src/`, 2026-11-04)

Function-local `static` singletons, each returned by reference:

| accessor | file | holds |
|---|---|---|
| `the_picture()` | `src/picture.cc` | `picture_data`: canvas, page, unitlength, offsets, **`m_palette`** (the `std::set<Color>` behind the `\xdefinecolor` lines) |
| `xmin()`/`xmax()`/`xsize()`/`ymin()`/`ymax()`/`ysize()`, `the_tix` | `src/picture.cc` | bounding box, animation parameter |
| `the_label_style()` | `src/label_style.cc` | font size and face, text color, label angle, mask, padding, border |
| `the_path_style()` | `src/path_style.cc` | line style (dashes), dash size |
| `the_paint_style()` | `src/paint_style.cc` | pen (line color/width), base pen, fill color and fill flag |
| `the_arrowhead_style()` | `src/arrow_style.cc` | arrowhead shape |
| `the_mark_size()` | `src/marker_style.cc` | dot and tick sizes |
| `the_angle_style()` | `src/angle_units.cc` | radians/degrees/revolutions |
| `the_clip_box()` / `the_crop_box()` | `src/clipping.cc`, `src/cropping.cc` | clip and crop regions |
| `the_screen_stack()` | `src/active_screen.cc` | `screen_broker`, a list of screen pointers seeded with `&the_picture().the_canvas` |
| `cam()` | `src/camera.cc` | the 3D `Camera` (position, target, orientation) |

`picture()` replaces the canvas, so drawn content does not leak. Everything else does. This list is a
lead from one read: confirm it in step 1, because "grep found nothing" is not proof of absence.

## Plan

1. **Inventory.** Grep `src/` for every function-local `static` and namespace-scope mutable object, and
   finish the table above (for example, check `legend`, `format`, and the `Color.cc` neutrals, which look
   immutable).
2. **`void reset_state()`** in a new `src/reset.cc` (+ header in `make_header`'s list and `meson.build`).
   It assigns each singleton a freshly default-constructed value: `the_label_style() = label_state();`,
   `the_paint_style() = paint_state();`, `the_picture() = picture_data();`, `cam() = Camera();`, …
   - **Order matters for `the_screen_stack()`:** it holds a pointer to `the_picture().the_canvas`.
     `picture_data`'s assignment keeps the object's address, so the pointer stays valid, but reset the
     stack to its single canvas entry anyway (pop to one), so a figure that leaked an `activate` without
     `deactivate` cannot poison the next one.
   - The defaults must equal the startup state exactly, or every port's byte-identity breaks. The
     harness is the proof: see step 5.
3. **Bind it:** `reset_state` in `python/epix/_epix.cc`, exported from `epix/__init__.py`.
4. **Call it from `figure()`**, before `_epix.picture(...)`. Update the docstring: "Each figure starts
   from ePiX's default state, so a notebook may draw any number of figures." `animate()` keeps its fork,
   since it is still correct and isolates `tix`. Optionally also reset in the child, for belt and braces.
5. **Verify:**
   - Every port still passes `build-aux/verify_ports.py` (a reset at startup must be a no-op).
   - A new test draws the 80 ported figures **in one process**, each one's eepic byte-identical to its
     fresh-process output. That is the real proof that every piece of state is covered.
   - The red/dashed repro above passes, including the palette.
   - `make asan` passes (the reset reassigns objects that pointers reference).
6. **Docs:** the `CLAUDE.md` bindings section, `figure.py`'s module docstring, and a `NEWS`/changelog
   entry if the project keeps one for the Python layer.

## Open questions

1. **C++ API name.** `reset_state()`? Or have `picture()` reset implicitly? That would change behaviour
   for C++ `.xp` files that set a style *before* `picture()`, which the samples may rely on. I recommend
   an explicit `reset_state()`, called only by the Python `figure()`.
