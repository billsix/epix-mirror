#!/usr/bin/env python3
"""Comment out every parameter name gcc reported as unused, at the exact column.

Reads data/w3-before-full.log (the keep-going level-3 build that
``TAG=before collect_warnings.sh`` wrote), takes each ``-Wunused-parameter`` site as (file, line, col, name), and
rewrites ``name`` to ``/*name*/`` at that byte column -- right-to-left within a
line so earlier edits don't shift later columns.  Idempotent: a site whose
column already starts with ``/*`` is skipped, so a second run changes nothing.
Paths in the log are relative to the Meson build dir (``../../epix/...``) and are
mapped onto the repo root.  Run from anywhere; prints a per-file count.
"""

from __future__ import annotations

import re
import sys
from collections import defaultdict
from pathlib import Path

HERE: Path = Path(__file__).resolve().parent
REPO: Path = HERE.parents[2]
LOG: Path = HERE / "data" / "w3-before-full.log"
SITE_RE: re.Pattern[str] = re.compile(
    r"^(?P<path>\S+?):(?P<line>\d+):(?P<col>\d+): warning: unused parameter "
    r"'(?P<name>[A-Za-z_]\w*)' \[-Wunused-parameter\]$"
)


def repo_path(log_path: str) -> Path:
    """Map a build-dir-relative log path (``../../epix/src/x.cc``) to the repo."""
    marker: str = "/epix/"
    idx: int = log_path.find(marker)
    rel: str = log_path[idx + len(marker) :] if idx >= 0 else log_path
    return REPO / rel


def main() -> int:
    sites: dict[Path, set[tuple[int, int, str]]] = defaultdict(set)
    for raw in LOG.read_text().splitlines():
        m: re.Match[str] | None = SITE_RE.match(raw)
        if m:
            sites[repo_path(m["path"])].add((int(m["line"]), int(m["col"]), m["name"]))

    total: int = 0
    for path, locs in sorted(sites.items()):
        lines: list[str] = path.read_text().split("\n")
        changed: int = 0
        # right-to-left within a line keeps earlier columns valid
        for line_no, col, name in sorted(locs, key=lambda t: (t[0], -t[1])):
            text: str = lines[line_no - 1]
            i: int = col - 1
            if text.startswith("/*", i) or "/*" + name + "*/" in text:
                continue  # already done (idempotent re-run; later columns may have shifted)
            if not text.startswith(name, i):
                sys.exit(f"{path}:{line_no}:{col}: expected {name!r}, found {text[i:i+20]!r}")
            lines[line_no - 1] = text[:i] + "/*" + name + "*/" + text[i + len(name) :]
            changed += 1
        if changed:
            path.write_text("\n".join(lines))
        print(f"{path.relative_to(REPO)}: {changed}")
        total += changed
    print(f"total: {total}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
