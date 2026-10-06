# Makefile -- containerized build/run wrapper for ePiX.
#
# Invoke as:   make <target>     (make help for the list)
# This is NOT the project build (ePiX builds with Meson: meson setup build).
# It's a thin convenience wrapper that drives the Meson build inside a Fedora
# `podman` image, so you don't need the toolchain on your host.
#
# Pattern: the host stays clean, all toolchain lives in a pinned OCI image, and
# this file is a thin UX layer over `podman run`.

.DEFAULT_GOAL := help

# This wrapper drives heavy, ordered `podman run` steps; never parallelize them
# (e.g. `build: lib py-ext` must run lib before py-ext links its output).
.NOTPARALLEL:

CONTAINER_CMD  = podman
CONTAINER_NAME = epix

# Optional-feature flags. Full image by default; a LEAN image is opt-in with
# MINIMAL_IMAGE=1, which flips each feature OFF -- `make image MINIMAL_IMAGE=1` drops the
# TeX-Live/ghostscript/ImageMagick rendering stack (BUILD_DOCS) and JupyterLab (USE_JUPYTER),
# neither of which the gates need (`make build`, `make asan`, the byte-identity harness run on
# g++ + libepix + nanobind). MINIMAL_IMAGE is a BUILD-CONTENT signal and is deliberately
# SEPARATE from NESTED_PODMAN (a RUN-capability signal, used only by PODMAN_RUN_FLAGS below):
# the sandbox does NOT auto-set MINIMAL_IMAGE, so a nested `make image` builds the full image
# unless you ask for lean. Override a single flag too (`make image MINIMAL_IMAGE=1 BUILD_DOCS=1`).
# A bare `podman build` stays lean regardless (each Dockerfile ARG defaults to 0).
# Convention: runClaudeInContainer tasks/reference/minimal-nested-images.md.
BUILD_DOCS  ?= $(if $(filter 1,$(MINIMAL_IMAGE)),0,1)
USE_JUPYTER ?= $(if $(filter 1,$(MINIMAL_IMAGE)),0,1)

# Host folder for rendered examples; bind-mounted into the container at /output.
OUTPUT_DIR ?= $(CURDIR)/output

# Extra flags for every container `run`. Auto-set when running nested inside a
# runClaudeInContainer/runCrushInContainer sandbox (which exports NESTED_PODMAN=1,
# making --cgroups=disabled apply so podman-in-podman works); empty — and
# byte-identical behavior — on a normal host. Overridable:
#   make shell PODMAN_RUN_FLAGS='--cgroups=disabled --network=host'
# On `run` lines only, never `build` (podman build rejects --cgroups). Convention:
# runClaudeInContainer tasks/reference/nested-podman-design.md.
PODMAN_RUN_FLAGS   ?= $(if $(filter 1,$(NESTED_PODMAN)),--cgroups=disabled)
PODMAN_BUILD_FLAGS ?=

# Passed through to the render scripts.
RENDER ?=     # examples:       empty => .eepic only;  pdf => also render PDF
FMT    ?=     # examples-anim:  empty => mng;          gif => animated GIF

# Optional host-file bind mounts -- included only when the file exists, so this
# never breaks on a machine that lacks them.  readlink -f resolves dotfile
# symlink farms; :Z is the SELinux relabel.
GITCONFIG_REAL  := $(shell readlink -f $(HOME)/.gitconfig 2>/dev/null)
GITCONFIG_MOUNT := $(shell [ -f "$(GITCONFIG_REAL)" ] && echo "-v $(GITCONFIG_REAL):/root/.gitconfig:Z")
TMUX_REAL       := $(shell readlink -f $(HOME)/.tmux.conf 2>/dev/null)
TMUX_MOUNT      := $(shell [ -f "$(TMUX_REAL)" ] && echo "-v $(TMUX_REAL):/root/.tmux.conf:Z")

# Live source tree + entrypoint scripts, bind-mounted so script edits take
# effect without an image rebuild.
SRC_MOUNT = -v $(CURDIR):/epix:Z
SCRIPT_MOUNTS = \
	-v $(CURDIR)/entrypoint/shell.sh:/shell.sh:Z \
	-v $(CURDIR)/entrypoint/build.sh:/build.sh:Z \
	-v $(CURDIR)/entrypoint/examples.sh:/examples.sh:Z \
	-v $(CURDIR)/entrypoint/examples-anim.sh:/examples-anim.sh:Z \
	-v $(CURDIR)/entrypoint/format.sh:/format.sh:Z \
	-v $(CURDIR)/entrypoint/jupyter.sh:/usr/local/bin/jupyter.sh:Z

RUN = $(CONTAINER_CMD) run --rm $(PODMAN_RUN_FLAGS) --entrypoint /bin/bash

# `shell` and `shell-exec` share ONE flag block so the batch twin can never drift
# from the interactive shell (scoped to this pair only). Convention:
# runClaudeInContainer tasks/add-shell-exec-target.md.
SHELL_RUN_FLAGS = \
	--entrypoint /bin/bash \
	$(SRC_MOUNT) $(SCRIPT_MOUNTS) \
	$(GITCONFIG_MOUNT) $(TMUX_MOUNT)

# In-container repo mount path (matches SRC_MOUNT: -v $(CURDIR):/epix).
REPO_MOUNT = /epix

# shell-exec payload: cd to the repo root (independent of shell.sh's own cd), then
# run the inline CMD, else the repo-relative SCRIPT. Prefers CMD when both are set.
SHELL_EXEC_ARGS = -c 'cd $(REPO_MOUNT) && $(if $(CMD),$(CMD),exec bash $(SCRIPT))'

.PHONY: all
all: image shell ## Build the image, then drop into a shell

.PHONY: image
image: ## Build the OCI image (full; MINIMAL_IMAGE=1 for lean -- see the flag block)
	$(CONTAINER_CMD) build $(PODMAN_BUILD_FLAGS) \
		--build-arg BUILD_DOCS=$(BUILD_DOCS) \
		--build-arg USE_JUPYTER=$(USE_JUPYTER) \
		-t $(CONTAINER_NAME) .

.PHONY: shell
shell: ## Interactive dev shell (live tree bind-mounted; epix/elaps/flix on PATH)
	$(CONTAINER_CMD) run --rm $(PODMAN_RUN_FLAGS) -it $(SHELL_RUN_FLAGS) $(CONTAINER_NAME) /shell.sh

.PHONY: shell-exec
shell-exec: image ## Run a script/command in the shell env (no TTY): make shell-exec SCRIPT=path | CMD='...'
	@[ -n "$(SCRIPT)$(CMD)" ] || { echo 'usage: make shell-exec SCRIPT=<repo-relative path> | CMD="..."'; exit 2; }
	$(CONTAINER_CMD) run --rm $(PODMAN_RUN_FLAGS) $(SHELL_RUN_FLAGS) $(CONTAINER_NAME) /shell.sh $(SHELL_EXEC_ARGS)

.PHONY: lib
lib: ## Build libepix.a + driver scripts (meson) from the bind-mounted sources
	$(RUN) \
		$(SRC_MOUNT) $(SCRIPT_MOUNTS) \
		$(CONTAINER_NAME) /build.sh

.PHONY: build
build: lib py-ext ## Full build: libepix.a + the Python extension (reflects edited sources)

.PHONY: examples
examples: ## Render samples/+doc/ figures -> $(OUTPUT_DIR) (.eepic; add RENDER=pdf for PDF)
	mkdir -p $(OUTPUT_DIR)
	$(RUN) \
		$(SRC_MOUNT) $(SCRIPT_MOUNTS) \
		-v $(OUTPUT_DIR):/output:Z \
		-e RENDER=$(RENDER) \
		$(CONTAINER_NAME) /examples.sh

.PHONY: examples-anim
examples-anim: ## Render .flx animations -> $(OUTPUT_DIR)/anim (FMT=mng|gif)
	mkdir -p $(OUTPUT_DIR)
	$(RUN) \
		$(SRC_MOUNT) $(SCRIPT_MOUNTS) \
		-v $(OUTPUT_DIR):/output:Z \
		-e FMT=$(FMT) \
		$(CONTAINER_NAME) /examples-anim.sh

.PHONY: py-ext
py-ext: ## Build the nanobind extension (_epix*.so); links build/libepix.a if present, else installed
	$(RUN) \
		$(SRC_MOUNT) \
		-v $(CURDIR)/entrypoint/build_py.sh:/build_py.sh:Z \
		$(CONTAINER_NAME) /build_py.sh

.PHONY: asan
asan: ## [dev] AddressSanitizer smoke over the bound libepix surface (fate: tasks/add-sanitizer-gate.md)
	$(RUN) \
		$(SRC_MOUNT) \
		-v $(CURDIR)/entrypoint/asan_check.sh:/asan_check.sh:Z \
		$(CONTAINER_NAME) /asan_check.sh

.PHONY: jupyter
jupyter: py-ext ## Launch JupyterLab (:8888) with the epix package; builds the extension first
	$(RUN) -it \
		$(SRC_MOUNT) $(SCRIPT_MOUNTS) \
		-p 8888:8888 \
		$(CONTAINER_NAME) /usr/local/bin/jupyter.sh

.PHONY: notebooks
notebooks: ## Convert notebooks/*.py (jupytext percent) to .ipynb
	$(RUN) \
		$(SRC_MOUNT) \
		-v $(CURDIR)/entrypoint/percentToIpynb.sh:/usr/local/bin/percentToIpynb.sh:Z \
		$(CONTAINER_NAME) /usr/local/bin/percentToIpynb.sh

.PHONY: format
format: ## Format C++ (clang-format) + Python (ruff) in place
	$(RUN) \
		$(SRC_MOUNT) \
		-v $(CURDIR)/entrypoint/format.sh:/format.sh:Z \
		$(CONTAINER_NAME) /format.sh

.PHONY: clean
clean: ## Remove the host $(OUTPUT_DIR) folder
	rm -rf $(OUTPUT_DIR)

.PHONY: image-export
image-export: ## export the OCI image to a timestamped tar in the repo root
	$(CONTAINER_CMD) save $(CONTAINER_NAME) -o $(CONTAINER_NAME)-$(shell date +%m-%d-%Y_%H-%M-%S).tar

.PHONY: image-import
image-import: ## import an OCI image tar: make image-import FILE=foo.tar
	$(CONTAINER_CMD) load -i $(FILE)

.PHONY: help
help:
	@grep --extended-regexp '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-16s\033[0m %s\n", $$1, $$2}'
