# The stubgen steps work without an installed `epix` — done

**Created:** 2026-11-20 (William Emerison Six <billsix@gmail.com>)
**Done:** 2026-11-20

`entrypoint/build_py.sh` and `tools/check_stub.sh` ran `python3 -m nanobind.stubgen -m
epix._epix`, which needed an installed `epix` package. This repo's image has one (editable), but
github.com/billsix/geometricalgebra's `entrypoint/install-epix.sh` runs `build_py.sh` before
installing the package and failed with `ModuleNotFoundError: No module named 'epix'` (found in
its `tasks/archive/2026/11/19/epix-type-stubs-for-pyright.md`, which worked around it with
`PYTHONPATH="$SRC/python"`; that workaround stays harmless). The stubgen step itself came from
`tasks/archive/2026/11/19/ship-python-type-information.md`.

Both stubgen calls now run with `PYTHONPATH="$PWD/python${PYTHONPATH:+:$PYTHONPATH}"` (both
scripts `cd /epix` first), so they import the tree just built.

Verified in `localhost/epix` after `uv pip uninstall epix` in a throwaway container: the previous
`build_py.sh` exited 1, the new one 0, `tools/check_stub.sh` reported the stub current, and
`python/epix/_epix.pyi` was unchanged in git.
