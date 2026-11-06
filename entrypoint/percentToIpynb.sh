#!/usr/bin/env bash
# percentToIpynb.sh -- convert the jupytext percent-format notebooks (.py) under
# notebooks/ to .ipynb. Pass specific files, or none to convert them all.
export VIRTUAL_ENV_DISABLE_PROMPT=1
source /venv/bin/activate
cd /epix/notebooks

# Lean image (USE_JUPYTER=0) has no jupytext: say so.
if ! command -v jupytext >/dev/null 2>&1; then
    echo "ERROR: make notebooks needs jupytext; this image was built without it (USE_JUPYTER=0)." >&2
    echo "" >&2
    echo "Enable it persistently, then rebuild and run:" >&2
    echo "  echo 'export USE_JUPYTER=1' >> ~/.bashrc && source ~/.bashrc" >&2
    echo "  make image" >&2
    echo "  make notebooks" >&2
    echo "" >&2
    echo "(This session only: run  export USE_JUPYTER=1  instead of the echo/source line.)" >&2
    exit 2
fi
if [ "$#" -gt 0 ]; then
    jupytext --to ipynb "$@"
else
    jupytext --to ipynb ./*.py
fi
