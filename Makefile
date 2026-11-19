REPOSITORY ?= i8degrees/nomlib-builder
VERSION ?= 0.13.1
ARCH ?= amd64

VENDOR_PREFIX ?= /app/vendor

all: help

help:
	@echo "usage"
	@echo
	@echo -e "Targets"
	@echo -e "\tbuild"
	@echo -e "\tcopy"
	@echo -e "\thelp (default)"
	@echo
.PHONY: help

build:
	bin/docker_build.sh $(PWD)
.PHONY: build

copy:
	bin/docker_copy-deps.sh $(PWD)
.PHONY: copy

_cross-compile:
	@return 0
	@echo
	@echo "1. Cross compiling example..."
	@rustc -vV
	@echo
	@echo "2. Compiling application (linux-musl x86_64)..."
	@cargo build --manifest-path=tests/hello-world/Cargo.toml --release --target x86_64-unknown-linux-musl
	@du -sh tests/hello-world/target/x86_64-unknown-linux-musl/release/helloworld
	@echo
	@echo "3. Compiling application (apple-darwin x86_64)..."
	@cargo build --manifest-path=tests/hello-world/Cargo.toml --release --target x86_64-apple-darwin
	@du -sh tests/hello-world/target/x86_64-apple-darwin/release/helloworld
.PHONY: cross-compile
