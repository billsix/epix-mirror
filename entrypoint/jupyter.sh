#!/usr/bin/env bash
# jupyter.sh -- launch JupyterLab in the container with the epix package
# importable. Run via `make jupyter`; open http://127.0.0.1:8888/lab on the host.
export VIRTUAL_ENV_DISABLE_PROMPT=1
source /venv/bin/activate
cd /epix
# Editable, offline install of python/ against the live mount (mirrors shell.sh).
uv pip install --python "$(which python)" --no-deps --no-index --no-build-isolation -e python

python -m ipykernel install --user --name=epix --display-name=epix >/dev/null 2>&1 || true

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
