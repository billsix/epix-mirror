#!/usr/bin/env bash
# jupyter.sh -- launch JupyterLab in the container with the epix package
# importable. Run via `make jupyter`; open http://127.0.0.1:8888/lab on the host.
export VIRTUAL_ENV_DISABLE_PROMPT=1
source /venv/bin/activate
cd /epix

# Lean image (USE_JUPYTER=0) has no JupyterLab: say so.
if ! command -v jupyter >/dev/null 2>&1; then
    echo "ERROR: make jupyter needs JupyterLab; this image was built without it (USE_JUPYTER=0)." >&2
    echo "" >&2
    echo "Enable it persistently, then rebuild and run:" >&2
    echo "  echo 'export USE_JUPYTER=1' >> ~/.bashrc && source ~/.bashrc" >&2
    echo "  make image" >&2
    echo "  make jupyter" >&2
    echo "" >&2
    echo "(This session only: run  export USE_JUPYTER=1  instead of the echo/source line.)" >&2
    exit 2
fi
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
