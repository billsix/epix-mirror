#!/usr/bin/env bash
# percentToIpynb.sh -- convert the jupytext percent-format notebooks (.py) under
# notebooks/ to .ipynb. Pass specific files, or none to convert them all.
export VIRTUAL_ENV_DISABLE_PROMPT=1
source /venv/bin/activate
cd /epix/notebooks

# Lean image (USE_JUPYTER=0) has no jupytext: say so.
if ! command -v jupytext >/dev/null 2>&1; then
    echo "jupytext not found: this image was built without JupyterLab" >&2
    echo "(make image MINIMAL_IMAGE=1 drops it; rebuild with plain 'make image' or USE_JUPYTER=1)" >&2
    exit 2
fi
if [ "$#" -gt 0 ]; then
    jupytext --to ipynb "$@"
else
    jupytext --to ipynb ./*.py
fi
