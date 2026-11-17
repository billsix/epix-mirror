#!/usr/bin/env bash
# Build an epix tree inside geometricalgebra's image and run probe.py against it.
#
#   run_probe.sh head    # the committed HEAD (git archive), the "before"
#   run_probe.sh tree    # the working tree, the "after"
#
# Output lands in tasks/adhoc/arrowheads-end-on-their-points/out-<mode>/. Needs the
# `gacalc` image (github.com/billsix/geometricalgebra), which carries TeX, Ghostscript,
# Pillow and the build tools epix needs.
set -euo pipefail

mode="${1:?usage: run_probe.sh head|tree}"
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo="$(git -C "$here" rev-parse --show-toplevel)"
out="$here/out-$mode"
mkdir -p "$out"

case "$mode" in
head) copy='git config --global --add safe.directory /src && git -C /src archive HEAD | tar -x -C /epix' ;;
tree) copy='cp -a /src/. /epix && rm -rf /epix/build /epix/python/epix/_epix*.so' ;;
*)
	echo "run_probe.sh: mode must be head or tree" >&2
	exit 2
	;;
esac

podman run ${PODMAN_RUN_FLAGS:-} --rm \
	-v "$repo":/src:ro,z -v "$here":/probe:z -v "$out":/out:z \
	--entrypoint /bin/bash gacalc -c "
set -euo pipefail
mkdir /epix && $copy
meson setup /epix/build /epix --prefix=/usr/local >/dev/null
meson install -C /epix/build >/dev/null
source /venv/bin/activate
bash /epix/entrypoint/build_py.sh >/dev/null
uv pip install -q --python /venv/bin/python --no-deps --no-build-isolation -e /epix/python
cd / && python /probe/probe.py /out
"
