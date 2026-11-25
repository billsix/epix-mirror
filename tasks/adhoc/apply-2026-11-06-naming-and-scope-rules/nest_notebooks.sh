#!/bin/bash
# Nest one-caller helpers in each notebook with the shared nest_one_caller.py.
# Notebooks are exec'd by the harness, never imported, so a same-named helper in
# another notebook is not a second caller; each notebook is nested alone in a
# scratch repository, where the tool's cross-file check has nothing to match.
# Run from anywhere; edits notebooks/*.py in place.
set -u
root="$(git rev-parse --show-toplevel)"
for f in "$root"/notebooks/*.py; do
    scratch="$(mktemp -d)"
    cp "$f" "$scratch/"
    (
        cd "$scratch" || exit 1
        git init -q
        git add .
        python3 ~/.claude/reference/nest_one_caller.py "$(basename "$f")"
    ) | grep -E "^  (nest|skip)" | sed "s|^|$(basename "$f"): |"
    cp "$scratch/$(basename "$f")" "$f"
    rm -rf "$scratch"
done
