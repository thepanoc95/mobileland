#
# AndroidPlatform.kext Makefile
# MobileLand (OS) - PocketDarwin Project
#
# Supports building for different device targets:
#   make DEVICE=harpia     - Build for Motorola Harpia (MSM8916, ARM32)
#   make DEVICE=harpia64   - Build for Motorola Harpia (MSM8916, ARM64)
#   make DEVICE=list       - List supported devices
#

# ============================================================================
# Configuration
# ============================================================================

KEXT_NAME := AndroidPlatform
KEXT_ID := com.pocketdarwin.driver.AndroidPlatform
KEXT_VERSION := 1.0.0

# Device target (override with DEVICE=<device>)
DEVICE ?= harpia64

# Source directories
SRCDIR := $(shell dirname $(MAKEFILE_LIST))
DEVICES_DIR := $(SRCDIR)/Device
COMMON_DIR := $(SRCDIR)/AndroidPlatformDevice

# Output directories
OBJROOT ?= $(CURDIR)/build
DESTDIR ?= $(OBJROOT)/System/Library/Extensions

# Detect cross-compilation environment
CROSS_COMPILE ?=
ifeq ($(CROSS_COMPILE),)
    # Check if we're on ARM host
    UNAME_M := $(shell uname -m 2>/dev/null)
    ifeq ($(UNAME_M),aarch64)
        CROSS_COMPILE := 1
    endif
endif

# ============================================================================
# Device Definitions
# ============================================================================

# Device architecture mapping
DEVICE_ARCH_harpia = arm
DEVICE_ARCH_harpia64 = arm64
DEVICE_ARCH_msm8916_common = arm arm64

# Device-specific source files (relative to Device/<device>/)
DEVICE_SOURCES_harpia = \
	Framebuffer/IOFramebuffer.cpp \
	Touch/GoodixTouch.cpp \
	Power/APM.cpp \
	Clock/GCCClock.cpp

DEVICE_SOURCES_harpia64 = \
	Framebuffer/IOFramebuffer.cpp \
	Touch/GoodixTouch.cpp \
	Power/APM.cpp \
	Clock/GCCClock.cpp \
	GPU/Adreno.cpp \
	Display/DSIHost.cpp

DEVICE_SOURCES_msm8916_common = \
	Framebuffer/IOFramebuffer.cpp \
	Touch/GoodixTouch.cpp \
	Power/APM.cpp \
	Clock/GCCClock.cpp \
	GPU/Adreno.cpp \
	Display/DSIHost.cpp

# Common sources (always included)
COMMON_SOURCES = \
	$(SRCDIR)/AndroidPlatform.cpp \
	$(COMMON_DIR)/AndroidPlatformDevice.cpp

# Get device architecture
ARCH := $(DEVICE_ARCH_$(DEVICE))

# ============================================================================
# Compiler Settings
# ============================================================================

# Cross-compilation toolchain
ifeq ($(CROSS_COMPILE),1)
    # ARM cross-compiler for x86 host
    CC := aarch64-linux-gnu-gcc
    CXX := aarch64-linux-gnu-g++
    STRIP := aarch64-linux-gnu-strip
    ARCH_PREFIX := aarch64-linux-gnu-
else ifeq ($(ARCH),arm)
    # ARM native or cross-compiler
    CC ?= arm-linux-gnueabihf-gcc
    CXX ?= arm-linux-gnueabihf-g++
    STRIP ?= arm-linux-gnueabihf-strip
    ARCH_PREFIX := arm-linux-gnueabihf-
else ifeq ($(ARCH),arm64)
    # ARM64 native or cross-compiler
    CC ?= aarch64-linux-gnu-gcc
    CXX ?= aarch64-linux-gnu-g++
    STRIP ?= aarch64-linux-gnu-strip
    ARCH_PREFIX := aarch64-linux-gnu-
else
    # Default compiler
    CC ?= clang
    CXX ?= clang++
    STRIP ?= strip
    ARCH_PREFIX :=
endif

# Common flags
CFLAGS_COMMON := -fno-exceptions -fno-rtti -Wall -Wextra -Wno-unused-parameter
CFLAGS_COMMON += -D$(KERNEL_CONFIG) -D$(BOARD_CONFIG)
CFLAGS_COMMON += -DDEVICE_$(shell echo $(DEVICE) | tr 'a-z' 'A-Z')=1

# Architecture-specific flags
ifeq ($(ARCH),arm)
    CFLAGS_ARCH := -march=armv7-a -mfpu=neon-vfpv4 -mfloat-abi=hard
    CFLAGS_ARCH += -D__arm__
    CFLAGS_ARCH += -fno-pic
else ifeq ($(ARCH),arm64)
    CFLAGS_ARCH := -march=armv8-a
    CFLAGS_ARCH += -D__arm64__
    CFLAGS_ARCH += -fno-pic
else
    # x86/x86_64 fallback for host compilation
    CFLAGS_ARCH := -D__x86_64__
endif

# Kernel build flags (required for kmod)
CFLAGS_KERNEL := -mkernel -fno-builtin -mno-red-zone
CFLAGS_KERNEL += -fno-pic -fno-pie
CFLAGS_KERNEL += -nostdinc

# Add SDK include paths
CFLAGS_SDK := -I$(SYSROOT)/usr/include -I$(SYSROOT)/System/Library/Frameworks

CFLAGS := $(CFLAGS_COMMON) $(CFLAGS_ARCH) $(CFLAGS_KERNEL) $(CFLAGS_SDK)

# Linker flags
LDFLAGS := -mkernel -nostdlib
LDFLAGS += -Wl,-rpath,@loader_path/../Frameworks

# Include paths
INCFLAGS := -I$(SRCDIR) -I$(COMMON_DIR)
INCFLAGS += -I$(DEVICES_DIR)/$(DEVICE) 2>/dev/null || true

# Frameworks and libraries (for host build only)
ifneq ($(CROSS_COMPILE),1)
    ifeq ($(ARCH),)
        LIBS := -lIOKit -lCoreFoundation -llibkern
    else
        LIBS :=
    endif
else
    LIBS := -lkmod
endif

# ============================================================================
# Build Rules
# ============================================================================

KEXT_DIR := $(DESTDIR)/$(KEXT_NAME).kext/Contents
KEXT_MACOS := $(KEXT_DIR)/MacOS/$(KEXT_NAME)
KEXT_RESOURCES := $(KEXT_DIR)/Resources

# Object files (per device)
OBJS := $(patsubst %.cpp,$(OBJROOT)/%.o,$(COMMON_SOURCES))
OBJS += $(patsubst %.cpp,$(OBJROOT)/%.o,$(foreach src,$(DEVICE_SOURCES_$(DEVICE)),$(DEVICES_DIR)/$(DEVICE)/$(src)))

# ============================================================================
# Targets
# ============================================================================

.PHONY: all clean install info list help all_devices

all: info $(KEXT_MACOS) $(KEXT_DIR)/Info.plist

info:
	@echo "Building AndroidPlatform.kext"
	@echo "  Device: $(DEVICE)"
	@echo "  Architecture: $(ARCH)"
	@echo "  Cross-compile: $(CROSS_COMPILE)"
	@echo "  CC: $(CC)"
	@echo "  CXX: $(CXX)"
	@echo "  Common sources: $(COMMON_SOURCES)"
	@echo "  Device sources ($(DEVICE)): $(DEVICE_SOURCES_$(DEVICE))"

$(KEXT_MACOS): $(OBJS)
	@mkdir -p $(dir $@)
	@echo "Linking $@ (cross=$(CROSS_COMPILE))..."
ifeq ($(CROSS_COMPILE),1)
	$(CXX) $(CFLAGS) $(INCFLAGS) -dynamiclib -kext -o $@ $(OBJS) $(LDFLAGS) $(LIBS)
	$(STRIP) -S $@
else
	$(CXX) $(CFLAGS) $(INCFLAGS) -dynamiclib -kext -o $@ $(OBJS) $(LIBS)
endif

$(OBJROOT)/%.o: $(SRCDIR)/%.cpp | $(OBJROOT)
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CFLAGS) $(INCFLAGS) -c $< -o $@

$(OBJROOT)/%.o: $(DEVICES_DIR)/%/%.cpp | $(OBJROOT)
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CFLAGS) $(INCFLAGS) -c $< -o $@

$(OBJROOT)/%.o: $(COMMON_DIR)/%.cpp | $(OBJROOT)
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CXX) $(CFLAGS) $(INCFLAGS) -c $< -o $@

$(OBJROOT):
	@mkdir -p $@

$(KEXT_DIR):
	@mkdir -p $@/MacOS
	@mkdir -p $@/Resources
	@mkdir -p $@/PlugIns

$(KEXT_DIR)/Info.plist: $(SRCDIR)/Info.plist | $(KEXT_DIR)
	@echo "Copying Info.plist..."
	@cat $< | sed 's/\$$(DEVICE)/$(DEVICE)/g' > $@

install: all
	@echo "Installing to $(DESTDIR)..."
	@mkdir -p $(DESTDIR)
	cp -r $(DESTDIR)/$(KEXT_NAME).kext $(DESTDIR)/

clean:
	@echo "Cleaning..."
	rm -rf $(OBJROOT)

list:
	@echo "Supported devices:"
	@echo "  harpia         - Motorola Moto G4 Play (ARM32, MSM8916)"
	@echo "  harpia64       - Motorola Moto G4 Play (ARM64, MSM8916)"
	@echo "  msm8916_common - Common MSM8916 drivers (both architectures)"
	@echo ""
	@echo "Cross-compilation:"
	@echo "  For ARM cross-compile on x86 host:"
	@echo "    CROSS_COMPILE=1 make DEVICE=harpia64"

help:
	@echo "AndroidPlatform.kext Build System"
	@echo ""
	@echo "Usage:"
	@echo "  make [DEVICE=<device>] [CROSS_COMPILE=1] [target]"
	@echo ""
	@echo "Targets:"
	@echo "  all       - Build the kext (default)"
	@echo "  clean     - Remove build artifacts"
	@echo "  install   - Build and install to DESTDIR"
	@echo "  list      - List supported devices"
	@echo "  info      - Show build configuration"
	@echo "  help      - Show this help message"
	@echo ""
	@echo "Cross-compilation (on x86 host for ARM target):"
	@echo "  CROSS_COMPILE=1 make DEVICE=harpia64"
	@echo "  CROSS_COMPILE=1 make DEVICE=harpia"
	@echo ""
	@echo "Native build (on ARM host):"
	@echo "  make DEVICE=harpia64"
	@echo ""
	@echo "Host build (for testing):"
	@echo "  make DEVICE=harpia64 KERNEL_CONFIG=HOST_BUILD"

all_devices:
	@echo "Building all device variants..."
	$(MAKE) DEVICE=harpia CROSS_COMPILE=1 clean
	$(MAKE) DEVICE=harpia CROSS_COMPILE=1
	$(MAKE) DEVICE=harpia64 CROSS_COMPILE=1 clean
	$(MAKE) DEVICE=harpia64 CROSS_COMPILE=1
