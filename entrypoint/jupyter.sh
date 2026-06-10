#!/usr/bin/env bash
# jupyter.sh -- launch JupyterLab in the container with the epix package on the
# path. Run via `make jupyter`; open http://127.0.0.1:8888/lab on the host.
cd /epix
export PYTHONPATH="/epix/python:${PYTHONPATH}"

python3 -m ipykernel install --user --name=epix --display-name=epix 2>/dev/null || true

exec jupyter lab \
    --allow-root \
    --ip=0.0.0.0 \
    --port=8888 \
    --ServerApp.token='' \
    --ServerApp.password='' \
    --ServerApp.disable_check_xsrf=True \
    --no-browser \
    --notebook-dir=/epix \
    --MultiKernelManager.default_kernel_name=epix
