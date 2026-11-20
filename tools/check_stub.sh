#!/usr/bin/env bash
# check_stub.sh -- the committed type stub python/epix/_epix.pyi matches the built
# extension. Regenerates the stub from the built python/epix/_epix*.so with nanobind's
# stubgen into a scratch file and compares. Run in the container:
#
#   make py-ext && make shell-exec SCRIPT=tools/check_stub.sh
#
# Exit 0: the stub is current. Exit 1: it is stale (run `make py-ext`, which rewrites
# it, and commit the result). Exit 2: the check could not run (no built extension, or
# stubgen failed), so nothing was compared.
set -u
export VIRTUAL_ENV_DISABLE_PROMPT=1
[ -f /venv/bin/activate ] && source /venv/bin/activate
[ -d /epix ] && cd /epix

scratch=$(mktemp --suffix=.pyi) || exit 2
trap 'rm -f "$scratch"' EXIT

# python/ on the path: compare against the tree here, installed epix or not.
if ! PYTHONPATH="$PWD/python${PYTHONPATH:+:$PYTHONPATH}" \
    python3 -m nanobind.stubgen -q -m epix._epix -o "$scratch"; then
    echo "check_stub: could not generate a stub from epix._epix (is the extension built? make py-ext)" >&2
    exit 2
fi
if cmp -s "$scratch" python/epix/_epix.pyi; then
    echo "check_stub: python/epix/_epix.pyi is current"
    exit 0
fi
echo "check_stub: python/epix/_epix.pyi is stale; run make py-ext and commit it" >&2
diff -u python/epix/_epix.pyi "$scratch" | head -40 >&2
exit 1
