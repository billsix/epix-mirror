FROM registry.fedoraproject.org/fedora:44

# Keep the dnf package cache across builds (BuildKit cache mounts), so iterating
# on this file doesn't re-download the (large) TeX-Live stack every time.
RUN --mount=type=cache,target=/var/cache/libdnf5 \
    --mount=type=cache,target=/var/lib/dnf \
    echo "keepcache=True" >> /etc/dnf/dnf.conf && \
    dnf upgrade -y

# Toolchain.
#   Build the library + driver scripts:  gcc-c++, make, binutils (ar/ranlib),
#     findutils/sed/which/diffutils/bash (the build + make_header + scripts).
#   Render examples to eps/pdf (elaps):  a (La)TeX stack + ghostscript
#     (ps2epsi/ps2pdf) + epstopdf.  ePiX's default backend is eepic, but some
#     samples emit PSTricks/TikZ, so those packages are included too.
#   Animations (flix):  ImageMagick `convert` (eps->png, png->mng/gif).
# NB: the exact texlive subpackage set is iterated against real `latex` errors;
# the collection-* metapackages give a coherent, reliably-named base.
RUN --mount=type=cache,target=/var/cache/libdnf5 \
    --mount=type=cache,target=/var/lib/dnf \
    dnf install -y \
        gcc-c++ make binutils findutils sed which diffutils bash \
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

# Build-relevant project files only (the runtime `make shell`/`build` bind-mounts
# overlay /epix with the live host tree, so this copy is just for the in-image
# install below). Placed after the slow dnf layer so editing source doesn't
# re-run it.
COPY configure configure.ac Makefile.am Makefile.in make_header VERSION /epix/
COPY config.guess config.sub install-sh missing mkinstalldirs depcomp /epix/
COPY epix-lib.sh bash_completions /epix/
COPY *.cc *.h *.in *.1 /epix/
COPY samples /epix/samples
COPY doc /epix/doc

# Configure, build, and install just what the toolchain needs so
# `epix`/`elaps`/`flix`/`laps` are on PATH in the image.
#   * `all-am` is automake's current-directory (non-recursive) build target:
#     libepix.a + the generated epix.h + the four scripts, WITHOUT recursing
#     into samples/ and doc/ (doc/ would try to build the whole manual).
#   * We then install ONLY the four specific component targets we need -- NOT
#     `install-am`, whose `install-data-local` hook installs the EPIX_NOTEFILES
#     (AUTHORS/BUGS/.../README) and would fail here: the notefiles aren't in the
#     build context, and the committed Makefile.in still names `README` (the
#     mirror's README was renamed to README.md without regenerating Makefile.in).
RUN cd /epix && \
    ./configure && \
    make all-am && \
    make install-binSCRIPTS \
         install-pkgdataDATA \
         install-pkglibLIBRARIES \
         install-includeHEADERS \
         install-pkgincludeHEADERS && \
    make clean

COPY entrypoint/entrypoint.sh /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
