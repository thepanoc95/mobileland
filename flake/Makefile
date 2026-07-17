# ravynOS Flake Makefile Helper
# Usage: make -f flake/Makefile [target]

.DELETE_ON_ERROR:
.PHONY: help dev shell arm env setup build world clean

# Default SDK/Toolchain paths
DEVROOT ?= /workspace/project/build/Developer
SDKROOT ?= $(DEVROOT)/Platforms/ravynOS.platform/Developer/SDKs/ravynOS.sdk
TOOLCHAIN ?= $(DEVROOT)/Platforms/ravynOS.platform/Developer/Toolchains/Default.xctoolchain

# iPhoneOS deployment target settings
TARGET_OS ?= iPhoneOS
TARGET_OS_VERSION ?= 15.0.0
IPHONEOS_DEPLOYMENT_TARGET ?= 15.0
ARM_VERSION ?= 11

# Architecture settings
BUILD_ARCH ?= X86_64
TARGET_ARCH ?= arm64
PLATFORM_NAME ?= Harpia
SOC_NAME ?= MSM8916

# Build settings
JOBS ?= $$(nproc)
BUILD_PROFILE ?= 0
BUILD_WERROR ?= 0

# Check if SDK/Toolchain exist
SDK_OK := $(shell test -d $(SDKROOT) && echo yes || echo no)
TOOLCHAIN_OK := $(shell test -d $(TOOLCHAIN)/usr/bin/clang && echo yes || echo no)

help:
	@echo "ravynOS Build System"
	@echo "===================="
	@echo "(Uses iPhoneOS Deployment Target)"
	@echo ""
	@echo "Targets:"
	@echo "  help       - Show this help"
	@echo "  dev        - Enter nix develop shell"
	@echo "  shell      - Enter legacy nix-shell"
	@echo "  arm        - Enter ARM cross-compilation shell"
	@echo "  env        - Generate environment setup script"
	@echo "  setup      - Setup directory structure"
	@echo "  build      - Build XNU kernel"
	@echo "  world      - Build everything"
	@echo "  clean      - Clean build artifacts"
	@echo ""
	@echo "Environment:"
	@echo "  DEVROOT=$(DEVROOT)"
	@echo "  SDKROOT=$(SDKROOT)"
	@echo "  TOOLCHAIN=$(TOOLCHAIN)"
	@echo "  TARGET_OS=$(TARGET_OS)"
	@echo "  IPHONEOS_DEPLOYMENT_TARGET=$(IPHONEOS_DEPLOYMENT_TARGET)"
	@echo "  BUILD_ARCH=$(BUILD_ARCH)"
	@echo "  TARGET_ARCH=$(TARGET_ARCH)"
	@echo "  ARM_VERSION=$(ARM_VERSION)"
	@echo ""
	@echo "SDK Status: $(SDK_OK)"
	@echo "Toolchain Status: $(TOOLCHAIN_OK)"

dev:
	@echo "Entering nix develop shell..."
	nix develop

shell:
	@echo "Entering nix-shell..."
	nix-shell shell.nix

arm:
	@echo "Entering ARM cross-compilation shell..."
	nix develop .#ravynos-arm-shell

env:
	@echo "Generating environment setup script..."
	@# Source from nix output
	nix build .#ravynos-shell --print-out-paths > /dev/null 2>&1 || true
	@echo "#!/bin/bash" > setup-ravynos.sh
	@echo "# ravynOS Environment Setup (iPhoneOS Target)" >> setup-ravynos.sh
	@echo "" >> setup-ravynos.sh
	@echo "export DEVROOT=$(DEVROOT)" >> setup-ravynos.sh
	@echo "export SDKROOT=$(SDKROOT)" >> setup-ravynos.sh
	@echo "export TOOLCHAIN=$(TOOLCHAIN)" >> setup-ravynos.sh
	@echo "export PATH=\$${TOOLCHAIN}/usr/bin:\$${TOOLCHAIN}/usr/libexec:\$$PATH" >> setup-ravynos.sh
	@echo "export CC=\$${TOOLCHAIN}/usr/bin/clang" >> setup-ravynos.sh
	@echo "export CXX=\$${TOOLCHAIN}/usr/bin/clang++" >> setup-ravynos.sh
	@echo "export LD=\$${TOOLCHAIN}/usr/bin/ld64" >> setup-ravynos.sh
	@echo "" >> setup-ravynos.sh
	@echo "# iPhoneOS deployment target" >> setup-ravynos.sh
	@echo "export TARGET_OS=$(TARGET_OS)" >> setup-ravynos.sh
	@echo "export TARGET_OS_VERSION=$(TARGET_OS_VERSION)" >> setup-ravynos.sh
	@echo "export IPHONEOS_DEPLOYMENT_TARGET=$(IPHONEOS_DEPLOYMENT_TARGET)" >> setup-ravynos.sh
	@echo "export ARM_VERSION=$(ARM_VERSION)" >> setup-ravynos.sh
	@echo "chmod +x setup-ravynos.sh"
	@echo "Setup script created: setup-ravynos.sh"

setup:
	@echo "Creating directory structure..."
	mkdir -p $(DEVROOT)/Platforms/ravynOS.platform/Developer/SDKs
	mkdir -p $(DEVROOT)/Platforms/ravynOS.platform/Developer/Toolchains
	mkdir -p $(DEVROOT)/Platforms/ravynOS.platform/Developer/Tools
	mkdir -p $(DEVROOT)/usr
	@echo "Directory structure created."
	@echo ""
	@echo "Next steps:"
	@echo "1. Download SDK and Toolchain from ravynOS releases"
	@echo "2. Extract SDK to: $(SDKROOT)"
	@echo "3. Extract Toolchain to: $(TOOLCHAIN)"
	@echo "4. Run: make -f flake/Makefile env"

build:
ifneq ($(TOOLCHAIN_OK),yes)
	@echo "Error: Toolchain not found at $(TOOLCHAIN)"
	@echo "Run 'make setup' and extract the toolchain first."
	@exit 1
endif
	@echo "Building XNU kernel..."
	export SDKROOT=$(SDKROOT) && \
	export TOOLCHAIN=$(TOOLCHAIN) && \
	export PATH="$${TOOLCHAIN}/usr/bin:$${TOOLCHAIN}/usr/libexec:$$PATH" && \
	bmake -m $(CURDIR)/BSD/share/mk \
		-j$(JOBS) \
		KERNEL_CONFIGS=RELEASE \
		ARCH_CONFIGS=$(BUILD_ARCH) \
		build

world: check-env
	@echo "Building ravynOS world..."
	export SDKROOT=$(SDKROOT) && \
	export TOOLCHAIN=$(TOOLCHAIN) && \
	export PATH="$${TOOLCHAIN}/usr/bin:$${TOOLCHAIN}/usr/libexec:$$PATH" && \
	bmake -m $(CURDIR)/BSD/share/mk \
		-j$(JOBS) \
		world

check-env:
ifneq ($(SDK_OK)$(TOOLCHAIN_OK),yesyes)
	@echo "Error: Missing dependencies"
	@echo "SDK: $(SDK_OK)"
	@echo "Toolchain: $(TOOLCHAIN_OK)"
	@echo ""
	@echo "Run 'make setup' and extract the SDK and toolchain."
	@exit 1
endif

clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(DEVROOT)/tmp/*
	find . -name "*.o" -delete
	find . -name "*.d" -delete
	@echo "Clean complete."
