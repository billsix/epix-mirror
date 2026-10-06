#!/usr/bin/env bash
# collect_warnings.sh -- build libepix once at a given Meson warning level in a scratch
# dir and write the full compiler output + a class x count summary under data/.
# Run from the repo root inside the image:
#   make shell-exec SCRIPT=tasks/adhoc/raise-meson-warning-level/collect_warnings.sh
# Optional: WLEVEL=2 (default 3); TAG=before|after (default: run) names the output files
# so a re-run never overwrites an earlier snapshot. Keep-going build so every warning is reported.
set -uo pipefail
WLEVEL="${WLEVEL:-3}"
TAG="${TAG:-run}"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUT="$HERE/data"
mkdir -p "$OUT"
B="/tmp/epix-w$WLEVEL"
rm -rf "$B"
meson setup "$B" -Dwarning_level="$WLEVEL" >/dev/null || exit 1
ninja -C "$B" -k 0 libepix.a 2>&1 | tee "$OUT/w${WLEVEL}-${TAG}-full.log" >/dev/null
echo "== warning classes (level $WLEVEL) =="
grep -oE "\[-W[a-z0-9=+-]+\]" "$OUT/w${WLEVEL}-${TAG}-full.log" | sort | uniq -c | sort -rn | tee "$OUT/w${WLEVEL}-${TAG}-by-class.txt"
echo "== per file =="
grep -E "warning:" "$OUT/w${WLEVEL}-${TAG}-full.log" | sed -E 's/^(\.\.\/)?([^:]+):.*/\2/' | sort | uniq -c | sort -rn | head -25 | tee "$OUT/w${WLEVEL}-${TAG}-by-file.txt"
echo "total warnings: $(grep -c 'warning:' "$OUT/w${WLEVEL}-${TAG}-full.log")"
