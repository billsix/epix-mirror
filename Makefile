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

CONTAINER_CMD  = podman
CONTAINER_NAME = epix

# Host folder for rendered examples; bind-mounted into the container at /output.
OUTPUT_DIR ?= $(CURDIR)/output

# Appended to every inner `podman run` / `podman build`.  Empty by default
# (correct on a normal host).  Running nested inside the claudecontainer
# sandbox needs --cgroups=disabled, e.g.:
#     make examples PODMAN_RUN_FLAGS=--cgroups=disabled
PODMAN_RUN_FLAGS   ?=
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
	-v $(CURDIR)/entrypoint/examples-anim.sh:/examples-anim.sh:Z

RUN = $(CONTAINER_CMD) run --rm $(PODMAN_RUN_FLAGS) --entrypoint /bin/bash

.PHONY: all
all: image shell ## Build the image, then drop into a shell

.PHONY: image
image: ## Build the OCI image
	$(CONTAINER_CMD) build $(PODMAN_BUILD_FLAGS) -t $(CONTAINER_NAME) .

.PHONY: shell
shell: ## Interactive dev shell (live tree bind-mounted; epix/elaps/flix on PATH)
	$(RUN) -it \
		$(SRC_MOUNT) $(SCRIPT_MOUNTS) \
		$(GITCONFIG_MOUNT) $(TMUX_MOUNT) \
		$(CONTAINER_NAME) /shell.sh

.PHONY: build
build: ## Build libepix.a + driver scripts from the bind-mounted sources
	$(RUN) \
		$(SRC_MOUNT) $(SCRIPT_MOUNTS) \
		$(CONTAINER_NAME) /build.sh

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
py-ext: ## Build the nanobind extension (python/epix/_epix*.so) against libepix
	$(RUN) \
		$(SRC_MOUNT) \
		-v $(CURDIR)/entrypoint/build_py.sh:/build_py.sh:Z \
		$(CONTAINER_NAME) /build_py.sh

.PHONY: asan
asan: ## [dev] AddressSanitizer smoke over the bound libepix surface (remove last phase)
	$(RUN) \
		$(SRC_MOUNT) \
		-v $(CURDIR)/entrypoint/asan_check.sh:/asan_check.sh:Z \
		$(CONTAINER_NAME) /asan_check.sh

.PHONY: jupyter
jupyter: ## Launch JupyterLab (port 8888) with the epix package + notebooks
	$(RUN) -it \
		$(SRC_MOUNT) $(SCRIPT_MOUNTS) \
		-v $(CURDIR)/entrypoint/jupyter.sh:/usr/local/bin/jupyter.sh:Z \
		-p 8888:8888 \
		$(CONTAINER_NAME) /usr/local/bin/jupyter.sh

.PHONY: notebooks
notebooks: ## Convert notebooks/*.py (jupytext percent) to .ipynb
	$(RUN) \
		$(SRC_MOUNT) \
		-v $(CURDIR)/entrypoint/percentToIpynb.sh:/usr/local/bin/percentToIpynb.sh:Z \
		$(CONTAINER_NAME) /usr/local/bin/percentToIpynb.sh

.PHONY: clean
clean: ## Remove the host $(OUTPUT_DIR) folder
	rm -rf $(OUTPUT_DIR)

.PHONY: help
help:
	@grep --extended-regexp '^[a-zA-Z0-9_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-16s\033[0m %s\n", $$1, $$2}'
