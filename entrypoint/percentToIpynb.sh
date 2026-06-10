#!/usr/bin/env bash
# percentToIpynb.sh -- convert the jupytext percent-format notebooks (.py) under
# notebooks/ to .ipynb. Pass specific files, or none to convert them all.
cd /epix/notebooks
if [ "$#" -gt 0 ]; then
    jupytext --to ipynb "$@"
else
    jupytext --to ipynb ./*.py
fi
