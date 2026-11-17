# A full (deep) audit of the repository

**Status:** proposed — needs go-ahead (open questions below)
**Priority:** 4
**Difficulty:** 5
**Created:** 2026-11-17 (William Emerison Six <billsix@gmail.com>)
**Related:** `tasks/apply-cpp-coding-standard.md` (the C++ sweep, which should start from this audit's
findings); the repo-review-conventions reference doc from github.com/billsix/runClaudeInContainer
(`tasks/reference/repo-review-conventions.md`: the method, dimensions and record layout)

## BLUF

Run `/audit-repo deep` on this repository: a review of every dimension (docs, docs against code, the
C++ library, the Python bindings and notebooks, the build and image, tests and the port harness, tools,
the open tasks and the archive). The result is a frozen record at
`tasks/review/<YYYY>/<MM>/<DD>/repo-review.md`, with a one-line pointer to it in `CLAUDE.md`. Done means
the record exists and its findings are turned into tasks (`/findings-to-tasks`) or explicitly left.

## Context

- The repo is a mirror of an upstream ePiX that is no longer maintained, extended here with a Meson
  build, nanobind Python bindings, about 81 notebook ports checked byte for byte against the C++
  samples, and a container image. `CLAUDE.md` already warns that the older prose docs drift from the
  source ("Doc drift").
- No review record exists yet (`tasks/review/` is absent).
- **A known finding to start from:** `make format` exits 1 on `HEAD` because ruff reports six findings
  in `tools/one_process_check.py` (seen 2026-11-17 while doing
  `tasks/archive/2026/11/17/arrowheads-end-on-their-points.md`). A gate that is red on `HEAD` hides new
  failures.
- A deep review fans out readers across the corpora and costs millions of subagent tokens, so it runs
  only when the maintainer asks.

## Open questions

1. **Deep or quick?** `/audit-repo deep` (every dimension, a dated frozen record) or the quick
   `/audit-repo` (docs against source, findings in chat)? Recommended: deep, since this asks for a full
   audit and the record feeds the C++ sweep.
2. **Order:** run this audit before `tasks/apply-cpp-coding-standard.md` starts, so the sweep's plan
   uses its findings? Recommended: yes.
