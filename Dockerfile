FROM registry.fedoraproject.org/fedora:44

# Keep the dnf package cache across builds (BuildKit cache mounts), so iterating
# on this file doesn't re-download the (large) TeX-Live stack every time. The cache
# mount is build plumbing only: the installed packages land in committed layers, so
# an exported image is self-contained.
RUN --mount=type=cache,target=/var/cache/libdnf5 \
    --mount=type=cache,target=/var/lib/dnf \
    echo "keepcache=True" >> /etc/dnf/dnf.conf && \
    dnf upgrade -y

# Shell dotfiles (.extrabashrc: prompt + the format-on-exit hook).
COPY entrypoint/dotfiles/ /root/

# System packages: per-group, host-runnable scripts (see each script's header for
# what the group is and why). 01 = build + Python + format toolchain; 02 = the
# TeX/ghostscript/ImageMagick rendering stack. Both run unconditionally today; a
# flag-gated lean image is tasks/minimal-nested-image.md.
COPY entrypoint/01-install-base.sh entrypoint/02-install-render.sh /usr/local/bin/
RUN --mount=type=cache,target=/var/cache/libdnf5 \
    --mount=type=cache,target=/var/lib/dnf \
    /usr/local/bin/01-install-base.sh && \
    /usr/local/bin/02-install-render.sh

# Python environment: a venv at /venv (--system-site-packages, so it also sees the
# dnf-installed setuptools/wheel/ruff), populated with uv. Every entry script
# activates it. Deps land in the committed /venv layer (the uv cache mount only
# speeds rebuilds), so an exported image runs offline.
#   jupyterlab/jupytext/ipython/ipykernel: the notebook front-end.
#   nanobind: headers + nb_combined.cpp for the extension build (entrypoint/build_py.sh).
# JupyterLab tweaks (same as geometricalgebra / modelviewprojection):
#   jupytext-config set-default-viewer python: a .py (jupytext percent) file opens as a
#     notebook by default.
#   labextension disable ...announcements: no "news"/update banners. --level=user
#     writes /root/.jupyter/labconfig/, read under any prefix.
RUN --mount=type=cache,target=/root/.cache/uv \
    python3 -m venv --system-site-packages /venv/ && \
    export VIRTUAL_ENV_DISABLE_PROMPT=1 && \
    source /venv/bin/activate && \
    uv pip install --python /venv/bin/python jupyterlab jupytext ipython ipykernel nanobind && \
    jupytext-config set-default-viewer python && \
    jupyter labextension disable --level=user "@jupyterlab/apputils-extension:announcements" && \
    echo "source ~/.extrabashrc" >> ~/.bashrc && \
    echo "elaps --pdf samples/hello.xp" >> ~/.bash_history && \
    echo "/build.sh" >> ~/.bash_history && \
    echo "/usr/local/bin/jupyter.sh # JupyterLab on http://127.0.0.1:8888/lab" >> ~/.bash_history

# Whole source tree (trimmed by .dockerignore).  At runtime `make shell`/`build`
# bind-mount the live host tree over /epix, so this copy only feeds the two
# in-image installs below.  Placed after the slow layers so editing source doesn't
# re-run them.
COPY . /epix

# Build + install with Meson so epix/elaps/flix/laps land on PATH (plus the lib,
# headers, epix-lib.sh, man pages, samples, and epix.info).  The printable LaTeX
# manual stays off by default (-Dmanual), so it is NOT built here.  Out-of-source
# build in /tmp/build; nothing is left in /epix.
RUN cd /epix && \
    meson setup /tmp/build --prefix=/usr/local && \
    meson install -C /tmp/build && \
    rm -rf /tmp/build

# The project's own Python package, installed EDITABLE into the venv, offline,
# from the fixed path /epix/python. At runtime the bind mount shadows this copy at
# the same path, so host edits (and the _epix*.so that `make py-ext` builds into
# python/epix/) are what `import epix` sees; shell.sh/jupyter.sh re-run this same
# idempotent install on entry.
RUN source /venv/bin/activate && cd /epix && \
    uv pip install --python /venv/bin/python --no-deps --no-index --no-build-isolation -e python

COPY entrypoint/entrypoint.sh /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
