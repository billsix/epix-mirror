#!/usr/bin/env bash
# verify_image.sh -- in-image smoke gate for the container build: the venv is active
# and `epix` imports from the live mount; the byte-identity harness PASSes on a sample
# of ports; the two JupyterLab tweaks are baked. Run from the repo root inside the
# image (`make build` first, so the extension exists):
#   make shell-exec SCRIPT=tools/verify_image.sh
# Every step runs; exit is nonzero if any failed. Promoted from tasks/adhoc/ (the
# dockerfile-to-standard-template task) because the minimal-image work reuses it.
status=0

echo "## environment"
which python
python -c 'import epix, sys; print("epix from", epix.__file__); print("prefix", sys.prefix)' || status=1
uv --version || status=1
ruff --version || status=1
jupyter lab --version || status=1

echo "## byte-identity harness (sample)"
for n in cube koch lorenz steiner stereo_proj plotting3 tori decorate; do
    python3 build-aux/verify_ports.py "$n" || status=1
done

echo "## jupyter settings"
jupytext-config list-default-viewer || status=1
jupyter labextension list 2>&1 | grep -i -B1 -A1 announce
echo "--- labconfig:"; ls /root/.jupyter/labconfig/ && cat /root/.jupyter/labconfig/page_config.json; echo

echo "## result: $status"
exit $status
