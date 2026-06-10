FROM registry.fedoraproject.org/fedora:44

# Keep the dnf package cache across builds (BuildKit cache mounts), so iterating
# on this file doesn't re-download the (large) TeX-Live stack every time.
RUN --mount=type=cache,target=/var/cache/libdnf5 \
    --mount=type=cache,target=/var/lib/dnf \
    echo "keepcache=True" >> /etc/dnf/dnf.conf && \
    dnf upgrade -y

# Toolchain.
#   Build (Meson): meson + ninja + gcc-c++ + binutils (ar/ranlib for the static
#     lib); bash/sed/findutils/which/diffutils for make_header + the scripts.
#   Render examples to eps/pdf (elaps): a (La)TeX stack + ghostscript
#     (ps2epsi/ps2pdf) + epstopdf.  ePiX's default backend is eepic, but some
#     samples emit PSTricks/TikZ, so those packages are included too.
#   Animations (flix): ImageMagick `convert` (eps->png, png->mng/gif).
#   libasan: DEV-ONLY runtime for AddressSanitizer builds (`make asan`), used to
#     catch latent libepix memory bugs while the Python bindings are written.
#     REMOVE in the final cleanup phase (see python-bindings task doc).
#   clang-tools-extra: clang-format + clang-tidy for `make format` / `make tidy`
#     (the C++ formatter/linter; pairs with .clang-format / .clang-tidy). Permanent.
RUN --mount=type=cache,target=/var/cache/libdnf5 \
    --mount=type=cache,target=/var/lib/dnf \
    dnf install -y \
        meson ninja-build gcc-c++ binutils \
        libasan \
        clang-tools-extra \
        findutils sed which diffutils bash \
        python3 python3-pip python3-devel \
        ghostscript ImageMagick \
        texlive-collection-basic \
        texlive-collection-latexrecommended \
        texlive-collection-pictures \
        texlive-collection-fontsrecommended \
        texlive-eepic \
        texlive-epstopdf \
        texlive-dvips \
        texlive-collection-pstricks \
        texlive-pgf

# Python notebook stack (Phase 1: inline display + jupytext percent notebooks).
# The epix Python package itself is bind-mounted at runtime (PYTHONPATH), so only
# the environment is baked here.
RUN --mount=type=cache,target=/root/.cache/pip \
    pip install --break-system-packages jupyterlab jupytext ipython ipykernel nanobind ruff

# Whole source tree (trimmed by .dockerignore).  At runtime `make shell`/`build`
# bind-mount the live host tree over /epix, so this copy only feeds the in-image
# install below.  Placed after the slow dnf layer so editing source doesn't
# re-run it.
COPY . /epix

# Build + install with Meson so epix/elaps/flix/laps land on PATH (plus the lib,
# headers, epix-lib.sh, man pages, samples, and epix.info).  The printable LaTeX
# manual stays off by default (-Dmanual), so it is NOT built here.  Out-of-source
# build in /tmp/build; nothing is left in /epix.
RUN cd /epix && \
    meson setup /tmp/build --prefix=/usr/local && \
    meson install -C /tmp/build && \
    rm -rf /tmp/build

COPY entrypoint/entrypoint.sh /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
