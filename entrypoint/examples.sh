#!/usr/bin/env bash
#
# examples.sh -- render the sample figures into /output (a host folder
# bind-mounted by `make examples`).
#
#   * Always emits .eepic (LaTeX picture macros) via `epix` -- needs g++ plus a
#     latex (ePiX runs latex to measure label sizes, even for eepic output).
#   * When RENDER=pdf, instead drives `elaps --pdf`, which writes a rendered
#     .pdf (needs the TeX-Live/ghostscript stack baked into the image).
#
# Sources: samples/*.xp and doc/*.xp, kept apart under /output/samples and
# /output/doc.  Each source subdir is COPIED into a scratch dir and rendered
# there, so epix's temp binaries and latex's transcripts never litter the
# (bind-mounted) host source tree -- and sibling #includes still resolve.
# epix derives its temp path from the input, so we run from the file's dir with
# a bare relative name (an absolute path makes it build nonexistent nested dirs).
#
# Each figure is compiled independently; a failure (e.g. a sample that needs -D
# preprocessor flags, or a non-standalone .xp) is logged and the run continues.
set -uo pipefail

RENDER="${RENDER:-eepic}"          # "eepic" (default) or "pdf"
OUT=/output
SRC=/epix
WORK="$(mktemp -d)"
trap 'rm -rf "$WORK"' EXIT

ok=0; fail=0; failed_list=()

render_one() {                     # $1 = basename root, $2 = output/scratch subdir
    local root="$1" sub="$2" target
    mkdir -p "$OUT/$sub"
    # -I. puts the source dir on the compiler's include path so samples that
    # #include a sibling header (e.g. lighting.h, std_F.h) resolve it.
    if [ "$RENDER" = "pdf" ]; then
        target="$OUT/$sub/$root.pdf"
        ( cd "$WORK/$sub" && elaps -I. --pdf -o "$target" "$root.xp" ) \
            >"$WORK/$sub/$root.render.log" 2>&1
    else
        target="$OUT/$sub/$root.eepic"
        ( cd "$WORK/$sub" && epix -I. -o "$target" "$root.xp" ) \
            >"$WORK/$sub/$root.render.log" 2>&1
    fi
    # Trust the artifact, not the exit code: the epix/elaps wrappers can exit 0
    # even when they produce nothing.  Require a non-empty output file.
    [ -s "$target" ]
}

echo "Rendering examples (RENDER=$RENDER) -> $OUT"
echo

for sub in samples doc; do
    [ -d "$SRC/$sub" ] || continue
    cp -r "$SRC/$sub" "$WORK/$sub"     # render from a scratch copy (no litter in source tree)
    shopt -s nullglob
    for xp in "$WORK/$sub"/*.xp; do
        root="$(basename "${xp%.xp}")"
        if render_one "$root" "$sub"; then
            ok=$((ok+1));   printf '  ok   %s/%s.xp\n' "$sub" "$root"
        else
            fail=$((fail+1)); failed_list+=("$sub/$root.xp")
            printf '  FAIL %s/%s.xp\n' "$sub" "$root"
        fi
    done
    shopt -u nullglob
done

echo
echo "Done: $ok rendered, $fail failed.  Output in $OUT/{samples,doc}/"
if [ "$fail" -gt 0 ]; then
    echo "Failed (often need -D flags or are non-standalone .xp):"
    printf '    %s\n' "${failed_list[@]}"
fi
# Exit 0 unless nothing rendered at all (catastrophic: bad toolchain/mount).
[ "$ok" -gt 0 ]
