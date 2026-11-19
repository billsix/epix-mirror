# Ship type information with the `epix` Python package

**Status:** proposed — needs go-ahead (open questions below)
**Priority:** 5
**Difficulty:** 4
**Created:** 2026-11-19 (William Emerison Six <billsix@gmail.com>), from a consumer's need in
github.com/billsix/geometricalgebra.

## BLUF

The `epix` package carries no type information: the compiled `_epix` extension has no `.pyi`
stub, and the package has no `py.typed` marker. A static type checker that imports `epix`
therefore sees nothing, and every name a consumer re-exports from it (`Point`, `LabelPos`,
`Color`, the `epix.figure._Pending` proxy) is reported unknown. Generate a stub for `_epix` with
`nanobind.stubgen`, add `py.typed`, and give the pure-Python modules (`figure.py`, `render.py`)
full annotations. Done means pyright and ty both resolve `from epix import Point, LabelPos` to real
types from an installed epix, with a check that the stub matches the built extension.

## Context (read first)

- **The consumer.** gacalc (github.com/billsix/geometricalgebra) draws its book's figures with
  `epix`. Its scene helpers are the `gacalc.epixscene` package. 31 of its notebooks import
  `Point`, `PendingFigure`, `LabelPos` and `Color` through those helpers, and for that reason its
  pyright gate excludes them (its `pyrightconfig.json`, and its
  `tasks/epix-type-stubs-for-pyright.md`). Its ty gate allows `epix` to be unresolved
  (`pyproject.toml`, `[tool.ty.analysis] allowed-unresolved-imports`).
- **Two separate gaps**, both confirmed 2026-11-19 in gacalc's image:
  1. *No type information.* `python/epix/` holds `__init__.py`, `figure.py`, `render.py` and the
     built `_epix*.so`, with no `.pyi` and no `py.typed`. `__init__.py` re-exports the scene API with
     `from ._epix import *`, so its whole surface (about 360 bindings in `python/epix/_epix.cc`) is
     invisible to a checker. `figure.py` is partly annotated: `render`, `figure` and `fit_bounds`
     are; `_Pending`'s `png`/`eepic`/`save` and `activated` are not.
  2. *The editable install hides the package from static tools.* gacalc's image installs epix
     editable from `/opt/epix-src/python`. setuptools' default editable mode writes an import-hook
     finder (`__editable___epix_1_2_22_finder.py`), which pyright and ty cannot follow, even
     with stubs. A path-based `.pth` (`--config-settings editable_mode=compat`, or a
     non-editable install) is visible to them. This half is the consumer's install command
     (gacalc's `entrypoint/install-epix.sh`), noted here so the stub work is tested the way a
     consumer installs.
- **nanobind 3.1.0** (in gacalc's image) has `nanobind.stubgen`, which writes a `.pyi` from an
  imported extension module, including the `nb::arg` names that gacalc's keyword-argument gate
  relies on.
- `entrypoint/build_py.sh` builds the extension outside Meson. `tasks/meson-python-option.md`
  proposes moving it into Meson. Stub generation belongs in whichever builds the `.so`.

## Plan

1. Generate `python/epix/_epix.pyi` with `python -m nanobind.stubgen -m epix._epix -o …` after
   the extension is built (`entrypoint/build_py.sh`, and in Meson if that task lands first).
2. Add `python/epix/py.typed` and list both files as package data in `python/pyproject.toml`.
3. Annotate the rest of `figure.py` and `render.py` (`_Pending.png`/`eepic`/`save`, `activated`).
   Consider making `_Pending` public, since consumers have to alias a private name to type a
   `with epix.figure() as fig:` target.
4. Gate: a check that rebuilding the stub reproduces the committed one, or that the stub is
   regenerated on every build (Q1), plus pyright and ty over `notebooks/` and a small consumer
   probe (`from epix import Point, LabelPos; p: Point = Point(x=1, y=2)`) with zero diagnostics.
5. Tell the consumer: gacalc then switches its epix install to a mode static tools can see, drops
   the pyright exclusions and ty's `allowed-unresolved-imports`, and runs its gates.

## Open questions

1. **Commit the generated `_epix.pyi`, or generate it at build time only?** Recommend: commit it,
   with a check that regenerating it changes nothing, so a reader (and a checker with no compiler)
   sees the API, and a binding change shows up in review.
2. **A stub-only distribution for consumers without ePiX?** gacalc's lean CI image has no ePiX at
   all, so even with stubs inside the package its CI pyright cannot see them unless the image builds
   ePiX. A separate `epix-stubs` package (`.pyi` files only, installable with no compiler) would let
   it check the 31 notebooks there. Recommend: not now. Ship the stubs in the package first; gacalc
   can check those notebooks in its full image, and a stub package can follow if CI needs it.
