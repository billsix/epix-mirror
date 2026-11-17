#!/usr/bin/env bash
# epix-mirror's gates for this change, run in the epix image from the repo root:
#   make shell-exec SCRIPT=tasks/adhoc/arrowheads-end-on-their-points/gates.sh
# Rebuilds libepix and the extension from this tree, runs the port harness on every
# sample that draws an arrow (one process per name), then the smoke gate.
status=0
{ [ -d build ] && meson setup --reconfigure build >/dev/null 2>&1; bash entrypoint/build.sh >/dev/null 2>&1; } || { echo "build failed"; status=1; }
bash entrypoint/build_py.sh >/dev/null 2>&1 || { echo "py-ext failed"; status=1; }
for n in $(grep -l 'arrow(' samples/*.xp samples/*.flx 2>/dev/null | xargs -n1 basename | sed 's/\.[a-z]*$//'); do
	[ -f "notebooks/$n.py" ] || continue
	python3 build-aux/verify_ports.py "$n" 2>&1 | tail -1 | sed "s/^/$n: /" || status=1
done
bash tools/verify_image.sh || status=1
exit $status
