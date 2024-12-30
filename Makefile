SHELL := /bin/bash
.SHELLFLAGS = -e -c
.DEFAULT_GOAL := help
.ONESHELL:

UNAME_S := $(shell uname -s)

NO_GENERATE_TEMPLATES ?= 0
RELEASE_KIND ?= debug

ifeq ($(UNAME_S),Linux)
CC := gcc
endif
ifeq ($(UNAME_S),Darwin)
CC := clang
endif

export PATH := "$(shell pwd)/bin:$(PATH)"

BUILD_DIR := ./build
BINARY := $(shell pwd)/build/src/demo
TEST_BINARY := $(shell pwd)/build/test/tests

.PHONY: configure
configure: ## Configure default candidate
	cmake -B$(BUILD_DIR) -S.

.PHONY: configure/debug
configure/debug: ## Configure debug candidate
	$(MAKE) configure RELEASE_KIND=debug

.PHONY: configure/release
configure/release: ## Configure release candidate
	$(MAKE) configure RELEASE_KIND=release

.PHONY: build
build: configure  ## Build default candidate
	cmake --build $(BUILD_DIR)

.PHONY: build/debug
build/debug: ## Build debug candidate
	$(MAKE) build RELEASE_KIND=debug

.PHONY: build/release
build/release: ## Build release candidate
	$(MAKE) build RELEASE_KIND=release

.PHONY: test
test: $(TEST_BINARY) ## Run test binary
	$<

.PHONY: run
run: $(BINARY) ## Run binary
	$<

.PHONY: run/debug
run/debug: ## Run debug candidate
	$(MAKE) build RELEASE_KIND=debug

.PHONY: run/release
run/release: ## Run release candidate
	$(MAKE) build RELEASE_KIND=release

.PHONY: setup
setup: ./scripts/setup ## Setup dependencies for system
	@$<

SOURCE_FILES := $(shell find ./src -type f -name '*.cpp')
HEADER_FILES := $(shell find ./src -type f -name '*.hpp')

.PHONY: format
format: ## Format the C/C++ code
	@echo $(SOURCE_FILES) $(HEADER_FILES) | xargs clang-format -i

.PHONY: lint
lint: ## Lint the C/C++ code
	@BAD_FILES=$(shell mktemp)
	echo "[clang-format] BEGIN"
	echo $(SOURCE_FILES) $(HEADER_FILES) | xargs -I {} $(SHELL) -c 'clang-format --dry-run --Werror {} || echo {}' >> $${BAD_FILES}
	if [[ -s $${BAD_FILES} ]]; then
		echo "[clang-format] Found formatting errors"
		cat $${BAD_FILES}
	else
		echo "[clang-format] No formatting errors"
	fi
	echo "[clang-format] END"
	echo "[clang-tidy] BEGIN"
	$(MAKE) tidy
	echo "[clang-tidy] END"
	if [[ -s $${BAD_FILES} ]]; then
		exit 1
	fi

.PHONY: tidy
tidy: ## Tidy the C/C++ code
	@clang-tidy $(SOURCE_FILES) $(HEADER_FILES)

.PHONY: version
version: ## Version info
	$(MAKE) --version
	$(CC) --version
	$(MAKE) run/version

.PHONY: help
help: ## Displays help info
	@awk 'BEGIN {FS = ":.*##"; printf "\nUsage:\n  make \033[36m\033[0m\n"} /^[a-zA-Z_-]+:.*?##/ { printf "  \033[36m%-15s\033[0m %s\n", $$1, $$2 } /^##@/ { printf "\n\033[1m%s\033[0m\n", substr($$0, 5) } ' $(MAKEFILE_LIST)
