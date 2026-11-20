# The stubgen steps work without an installed `epix`

**Status:** in progress
**Priority:** 3
**Difficulty:** 1
**Created:** 2026-11-20 (William Emerison Six <billsix@gmail.com>)

## BLUF

`entrypoint/build_py.sh` and `tools/check_stub.sh` run `python3 -m nanobind.stubgen -m
epix._epix`, which imports `epix` from wherever Python finds it. In this repo's image the package
is installed editable, so that works, but a consumer that runs `build_py.sh` before installing the
package fails with `ModuleNotFoundError: No module named 'epix'`. Put `python/` (the tree the
script just built into) on `PYTHONPATH` for both stubgen calls. Done means both scripts pass in
this image with the editable `epix` uninstalled, and the regenerated stub is unchanged.

## Context

- Found 2026-11-19 by github.com/billsix/geometricalgebra's image build
  (its `tasks/archive/2026/11/19/epix-type-stubs-for-pyright.md`): its
  `entrypoint/install-epix.sh` runs this repo's `build_py.sh` before `uv pip install -e python`,
  and works around the failure with `PYTHONPATH="$SRC/python"`. That workaround stays harmless
  once this lands.
- The stubgen step came from `tasks/archive/2026/11/19/ship-python-type-information.md`.

## Plan

1. Prefix both stubgen calls with `PYTHONPATH="$PWD/python${PYTHONPATH:+:$PYTHONPATH}"` (both
   scripts `cd /epix` first).
2. Verify in `localhost/epix`: uninstall `epix` in a throwaway container, run `build_py.sh` and
   `tools/check_stub.sh`; then `git diff --exit-code python/epix/_epix.pyi`.
