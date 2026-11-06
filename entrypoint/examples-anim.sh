#!/usr/bin/env bash
#
# examples-anim.sh -- render the .flx animation samples into /output/anim via
# `flix` (ePiX -> png frames -> mng/gif, assembled with ImageMagick).  Kept
# separate from examples.sh because the animation path additionally needs
# ImageMagick's `convert`.  If a render fails on a "not authorized" / PS
# delegate error, that's ImageMagick's policy.xml, not the Makefile.
#
# As in examples.sh, the source dir is COPIED into scratch and rendered there
# (flix, like epix, derives temp paths from the input, so it needs a bare
# relative name run from the file's dir; this also keeps frame PNGs and latex
# transcripts out of the bind-mounted host source tree).
#
# FMT controls the movie format: "mng" (default) or "gif".
set -uo pipefail

# Lean image (BUILD_DOCS=0) has no TeX: say so instead of failing 96 times.
if ! command -v latex >/dev/null 2>&1; then
    echo "ERROR: make examples-anim needs latex + ImageMagick (frames via ePiX, assembled with" >&2
    echo "       convert); this image was built without them (BUILD_DOCS=0)." >&2
    echo "" >&2
    echo "Enable it persistently, then rebuild and run:" >&2
    echo "  echo 'export BUILD_DOCS=1' >> ~/.bashrc && source ~/.bashrc" >&2
    echo "  make image" >&2
    echo "  make examples-anim" >&2
    echo "" >&2
    echo "(This session only: run  export BUILD_DOCS=1  instead of the echo/source line.)" >&2
    exit 2
fi

FMT="${FMT:-mng}"
OUT=/output/anim
SRC=/epix/samples
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

mkdir -p "$OUT"
[ -d "$SRC" ] && cp -r "$SRC" "$WORK/samples"

ok=0; fail=0; failed_list=()
flix_opt=()
[ "$FMT" = "gif" ] && flix_opt+=(--gif)

echo "Rendering animations (FMT=$FMT) -> $OUT"
echo

shopt -s nullglob
for flx in "$WORK/samples"/*.flx; do
    root="$(basename "${flx%.flx}")"
    # -I. lets a .flx #include a sibling header (e.g. lighting.h).
    ( cd "$WORK/samples" && flix -I. "${flix_opt[@]}" -o "$OUT/$root.$FMT" "$root.flx" ) \
        >"$WORK/samples/$root.render.log" 2>&1
    if [ -s "$OUT/$root.$FMT" ]; then
        ok=$((ok+1));   printf '  ok   %s.flx\n' "$root"
    else
        fail=$((fail+1)); failed_list+=("$root.flx")
        printf '  FAIL %s.flx\n' "$root"
    fi
done
shopt -u nullglob

echo
echo "Done: $ok rendered, $fail failed.  Output in $OUT/"
if [ "$fail" -gt 0 ]; then
    echo "Failed (check ImageMagick policy.xml for PS/EPS delegate blocks):"
    printf '    %s\n' "${failed_list[@]}"
fi
[ "$ok" -gt 0 ]
