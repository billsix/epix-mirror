# Ship type information with the `epix` Python package

**Created:** 2026-11-19 (William Emerison Six <billsix@gmail.com>), from a consumer's need in
github.com/billsix/geometricalgebra.
**Done:** 2026-11-19, branch `ship-python-type-information` (`ea712f7`).

The `epix` package carried no type information: the compiled `_epix` extension had no `.pyi`, and
the package had no `py.typed`. A type checker that imported `epix` saw nothing, and every name a
consumer re-exported from it was reported unknown. gacalc's pyright gate excluded 31 notebooks
for that reason (its `tasks/epix-type-stubs-for-pyright.md`, blocked on this task being pushed).

## Decisions (William Emerison Six <billsix@gmail.com>, 2026-11-19)

1. The generated `_epix.pyi` is committed, with a gate that fails when regenerating it changes
   anything.
2. No stub-only `epix-stubs` package for now. The stubs ship inside `epix`, and a stub package can
   follow if a consumer's CI (gacalc's lean image has no ePiX) needs one.

## What was done

- `entrypoint/build_py.sh` runs `python -m nanobind.stubgen -q -m epix._epix -o
  python/epix/_epix.pyi` after the link step, and also when the relink is skipped as up to date.
  The stub is 908 lines and fully typed: no `object` or `Any` fallbacks, and the bound argument
  names are kept.
- `tools/check_stub.sh` regenerates the stub into a scratch file and compares it with the
  committed one. It exits 0 when current, 1 when stale (with a diff), and 2 when it cannot run.
  `tools/verify_image.sh` runs it. ruff's `extend-exclude` skips the stub, so the formatter cannot
  make the comparison fail.
- `python/epix/py.typed` was added, and both files are `[tool.setuptools.package-data]` in
  `python/pyproject.toml`.
- `figure.py` and `render.py` are fully annotated: `_Pending`'s `_repr_png_`, `png`, `eepic` and
  `save`; `figure`, `figure_around` and `activated` as `Generator[…, None, None]`, which is valid
  for the package's `requires-python >= 3.9`; `animate`'s `build`; and the `__init__`s.
  `_Pending.save()` called before the figure renders now raises a `RuntimeError` saying so,
  instead of an `AttributeError` on `None`.
- `CLAUDE.md`: the stub is listed under generated files, plus the type-information bullet in the
  Python layout.
- Not done: making `_Pending` public. Consumers still alias the private name to type a
  `with epix.figure() as fig:` target, and renaming a public API is the maintainer's call.

## Verification

- The stub was generated twice, byte-identical.
- pyright and ty ran from gacalc's image, which has both, against this checkout on the search
  path. A consumer probe that imports `Point` and `LabelPos`, calls `figure`, `label` and `black`,
  and makes one deliberate bad assignment got exactly that one error from each checker. Both
  checkers also passed `python/epix` with no diagnostics.
- A wheel built from `python/` contains `epix/py.typed` and `epix/_epix.pyi`. It has no `.so`,
  as before this change; that is `tasks/meson-python-option.md`.
- An editable install with `--config-settings editable_mode=compat` writes a plain path `.pth`
  (`/epix/python`), which static checkers can follow. The default mode writes an import hook,
  which they cannot.
- `entrypoint/format.sh` exited 0. `tools/verify_image.sh` passed: 8 of 8 ports PASS, and the
  stub is current.

## For the consumer

gacalc's `tasks/epix-type-stubs-for-pyright.md` clears once this branch is pushed. Its steps are:

1. Bump `EPIX_COMMIT`.
2. Install epix in compat mode in `entrypoint/install-epix.sh`.
3. Drop pyright's exclude list and ty's `allowed-unresolved-imports`.
4. Decide how its lean CI image gets the types.
