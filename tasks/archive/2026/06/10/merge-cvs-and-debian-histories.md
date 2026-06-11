# Merge the CVS-mirror and Debian-based histories into one clean master

**Status:** DONE (2026-06-10). Executed exactly as planned; all checks passed. Bill handles all pushes himself.

## Result (2026-06-10)

Unified `master` built and verified:
- `git diff backup/master-pre-graft master` → **empty** (final tree byte-identical).
- `8f8c19e` (2003 Savannah import) is now an ancestor of `master`.
- `master` total commits: **115** (74 CVS + 1 strip + 40 modern).
- The three `*-from debian` commits (`e2f5431`/`3e61d13`/`9190483`) are gone from `master`.
- All 4 topic-branch merges preserved; the 4 topic branches deleted.
- Graft seam: `c4bc237 release epix-1.2.22` → `864cd83 Drop generated doc/sample artifacts` → modern work.
- New `master` tip: `001b05f Merge branch 'pythonBindingsAndNotebooks'`.
- Safety tag `backup/master-pre-graft` (→ old `adbf6c2`) left in place; drop it once confident.

**Caveat — GPG signing:** Bill's `commit.gpgsign` is on, but gpg pinentry times out in the
container, so the rebase ran with `-c commit.gpgsign=false`. The new graft commit and all 40
replayed modern commits are therefore **unsigned** (rebasing rewrote their SHAs regardless).
Bill confirmed mid-run to keep signing off. Re-sign on his host if desired before/after pushing.

**Pushes:** Bill's job. `master` is rewritten, so it needs a force-push to origin and/or github.

## Problem

`/epix` has two disjoint git histories with **no common ancestor**:

| Branch | Root | Tip | Count | Origin |
|---|---|---|---|---|
| `github/master` | `8f8c19e` "Version 0.9.0rc1: Initial Savannah import, Aug 27 2003" | `c4bc237` "release epix-1.2.22" | 74 | mirrored from upstream **CVS** |
| `master` / `origin/master` | `e2f5431` "epix-1.2.18 from debian" | `a5a91a5` (modern work) | 42 | **Debian/Fedora** release tarballs + all modern meson/C++17/python work |

`github/master` was mirrored from CVS (Bill had forgotten this); `master` was bootstrapped from Debian release tarballs and is where all the real modernization lives. Goal: one unified `master` with the deep CVS history (back to 2003) underneath the modern work, and a clean shape.

## Key finding — the histories meet cleanly at 1.2.22

`master`'s first three commits are pure tarball imports:

```
e2f5431  epix-1.2.18 from debian   (root)
3e61d13  epix-1.2.19 from debian
9190483  epix-1.2.22 from debian    <-- parent of the first modern commit
aabe942  added task to document the repo   <-- modern work starts here
...
a5a91a5  Merge branch 'pythonBindingsAndNotebooks'   (HEAD)
```

The debian 1.2.22 tree (`9190483`) vs the CVS 1.2.22 tip (`c4bc237`) differ in **only 6 files, all additions present only in the CVS tree** — generated artifacts Debian stripped:

```
doc/manual-stamp
doc/manual.pdf.gz
doc/manual.ps.gz
doc/manual_src.tar.gz
samples/sample-stamp
samples/sample_src.tar.gz
```

Every other tracked file is byte-identical. So `c4bc237` is the unambiguous graft point — it *is* 1.2.22, just carrying the upstream-generated blobs the modern repo (correctly) doesn't track. Verified: none of the 39 modern commits ever touch those 6 paths, and all 6 are absent from current `master` HEAD.

## Decisions (from Bill, 2026-06-10)

1. **Preserve** the 4 topic-branch merges via `--rebase-merges` (not flatten).
2. **Delete** the 4 topic branches after the merge.
3. Bill handles all pushes (origin = the Pi, github = the mirror). Claude does **not** push.

The 4 topic branches (all ancestors of `master`, pointing at the second-parent tips of the merges):

```
meson                        -> 4514c84  Merge branch 'meson'
normalizeDirectoryStructure  -> 80261db  moved Makefile.docker to Makefile
C++17modernization           -> 67790ef  archived task modernize-cxx-standard
pythonBindingsAndNotebooks   -> b70de71  fix how to run jupyter
```

(No tags exist in the repo.)

## Plan / commands

Insert one honest commit on top of the CVS tip that strips the 6 generated blobs — this makes the graft base tree **identical** to debian's 1.2.22 (`9190483`), guaranteeing the final tree is unchanged — then replay the modern work onto it with merges preserved.

```sh
# 0. SAFETY: tag the current tip so nothing is lost
git tag backup/master-pre-graft master

# 1. Build graft base G on the CVS tip: strip the 6 generated artifacts.
#    tree(G) == tree(9190483) == debian 1.2.22 exactly.
git checkout --detach c4bc237
git rm doc/manual-stamp doc/manual.pdf.gz doc/manual.ps.gz \
       doc/manual_src.tar.gz samples/sample-stamp samples/sample_src.tar.gz
git commit -m "Drop generated doc/sample artifacts (not tracked going forward)"
G=$(git rev-parse HEAD)

# 2. Replay the modern work (aabe942..master) onto G, PRESERVING the 4 topic-branch merges.
git rebase --rebase-merges --onto "$G" 9190483 master

# 3. VERIFY content is unchanged: must print NOTHING.
git diff backup/master-pre-graft master

# 4. Delete the 4 topic branches (decision #2).
git branch -D meson normalizeDirectoryStructure C++17modernization pythonBindingsAndNotebooks

# 5. (optional) drop the safety tag once satisfied
#    git tag -d backup/master-pre-graft
```

### Resulting history — single root from 2003, ~114 commits

```
8f8c19e (2003 Savannah import) ─ … ─ c4bc237 (release 1.2.22)   <- 74 CVS commits, untouched
        └─ "Drop generated doc/sample artifacts"                 <- 1 new graft commit
              └─ aabe942' … a5a91a5'                             <- 39 modern commits, merges preserved
```

The three `*-from debian` commits disappear; all other content is preserved. SHA1s on the modern half change (Bill approved).

## Verification checklist

- [ ] `git diff backup/master-pre-graft master` prints nothing (final tree identical).
- [ ] `git merge-base --is-ancestor 8f8c19e master` succeeds (CVS root is now an ancestor).
- [ ] `git log --oneline --graph master` shows the 4 topic-branch merges intact.
- [ ] `git rev-list --count master` ~= 114 (74 + 1 + 39).
- [ ] Old `*-from debian` commits no longer reachable from `master`.

## Notes / risks

- The rebase should produce **zero conflicts**: `tree(G) == tree(9190483)`, and nothing in the modern range touches the 6 stripped paths.
- After step 4 the only ref still holding the *old* pre-rebase modern commits is the `backup/master-pre-graft` tag — intentional, drop it (step 5) when confident.
- Bill pushes the result to origin and/or github himself (force-push, since `master` is rewritten). Not Claude's job.
- `github/master` / `origin/master` remote-tracking refs still point at old/CVS tips until Bill pushes + refetches — expected.
