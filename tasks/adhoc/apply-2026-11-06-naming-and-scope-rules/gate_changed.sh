#!/bin/bash
# Sort imports, format and lint only the files this task changed (the repo gate rewrites everything).
source /venv/bin/activate; cd /epix || exit 1
F="notebooks/color_sep.py notebooks/cubic_cutaway.py notebooks/decorate.py notebooks/densityplot.py notebooks/dipole.py notebooks/extract.py notebooks/helicoid.py notebooks/label_debug.py notebooks/lighting.py notebooks/line_debug.py notebooks/lorenz.py notebooks/mirrorball.py notebooks/oscillator.py notebooks/pendulum.py notebooks/riemann.py notebooks/sqrt.py notebooks/steiner.py notebooks/steiner2.py notebooks/stereo_proj.py notebooks/trig.py notebooks/twisted_cubic.py python/epix/figure.py python/epix/render.py tools/one_process_check.py "
ruff check --config python/pyproject.toml --select I001 --fix $F
ruff format --config python/pyproject.toml $F
ruff check --config python/pyproject.toml --output-format concise --exit-zero $F
