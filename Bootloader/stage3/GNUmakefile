# ============================================================================
# Kickstart Loader
# ============================================================================

# Toolchain
ARCH        ?= arm
CROSS_ARM   := arm-linux-gnueabihf-
CROSS_ARM64 := aarch64-linux-gnu-

ifeq ($(ARCH),arm64)
CC      := $(CROSS_ARM64)gcc
OBJCOPY := $(CROSS_ARM64)objcopy
OBJDUMP := $(CROSS_ARM64)objdump
READELF := $(CROSS_ARM64)readelf
EXT      := elf64
ARCHDEF  := -D__aarch64__
else
CC      := $(CROSS_ARM)gcc
OBJCOPY := $(CROSS_ARM)objcopy
OBJDUMP := $(CROSS_ARM)objdump
READELF := $(CROSS_ARM)readelf
EXT      := elf
ARCHDEF  := -D__arm__
endif

# Git information
COMMIT := $(shell git rev-parse --short HEAD 2>/dev/null || echo unknown)
BRANCH := $(shell git rev-parse --abbrev-ref HEAD 2>/dev/null || echo nogit)

TARGET := kickstart_loader_$(COMMIT)_$(BRANCH).$(EXT)

# Files
SRC     := boot.c
OBJ     := xnu_bootloader.o
LINKER  := linker.ld

# Optional embedded Apple DeviceTree
DTB     ?=
DTBOBJ  :=

ifneq ($(DTB),)
DTBOBJ := device_tree.o
endif

# ============================================================================
# Compiler Flags
# ============================================================================

COMMON_CFLAGS := \
    -ffreestanding \
    -fno-builtin \
    -fno-stack-protector \
    -fno-pic \
    -fno-pie \
    -nostdlib \
    -nostartfiles \
    -Wall \
    -Wextra \
    -O2 \
    $(ARCHDEF)

ifeq ($(ARCH),arm64)
COMMON_CFLAGS += \
    -march=armv8-a
else
COMMON_CFLAGS += \
    -mcpu=cortex-a53 \
    -mfpu=neon-vfpv4 \
    -mfloat-abi=hard
endif

LDFLAGS := \
    -nostdlib \
    -nostartfiles \
    -no-pie \
    -T $(LINKER)

# ============================================================================
# Targets
# ============================================================================

all: $(TARGET)

$(OBJ): $(SRC)
	$(CC) $(COMMON_CFLAGS) -c $< -o $@

device_tree.o:
	$(OBJCOPY) \
	    -I binary \
	    -O elf32-littlearm \
	    -B arm \
	    $(DTB) $@
	$(OBJCOPY) \
	    --rename-section .data=.device_tree \
	    $@

$(TARGET): $(OBJ) $(DTBOBJ)
	$(CC) $(LDFLAGS) $^ -o $@

disasm: $(TARGET)
	$(OBJDUMP) -D $(TARGET) > $(TARGET).asm

headers: $(TARGET)
	$(READELF) -h $(TARGET)

sections: $(TARGET)
	$(READELF) -S $(TARGET)

symbols: $(TARGET)
	$(READELF) -s $(TARGET)

clean:
	rm -f *.o *.elf *.elf64 *.asm

.PHONY: all clean disasm headers sections symbols