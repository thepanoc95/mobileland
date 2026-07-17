#
# RAVYNOS BUILD SYSTEM - Kernel Extensions
#
# Architecture-specific kext selection for ravynOS
#

TARGET_ARCH ?= ${MACHINE}

# Common KEXTs (built for all architectures)
SUBDIR_COMMON = AppleAPIC IOACPIFamily IOPCIFamily IOStorageFamily corecrypto pthread IOHIDFamily

# X86_64-only KEXTs (Intel-specific hardware - excluded for ARM)
SUBDIR_X86_ONLY = AppleIntelPIIXATA IOATAFamily RavynAHCIPort ApplePS2Controller

# Architecture-specific KEXTs
SUBDIR_X86_64 = IOGraphics AppleFileSystemDriver RavynXHCIPort msdosfs hfs
SUBDIR_ARM64 = AndroidPlatform
SUBDIR_ARM = AndroidPlatform

# Select KEXTs based on architecture
SUBDIR = ${SUBDIR_COMMON}

.if "${TARGET_ARCH}" == "X86_64" || "${TARGET_ARCH}" == "x86_64" || "${TARGET_ARCH}" == "amd64"
    SUBDIR += ${SUBDIR_X86_ONLY} ${SUBDIR_X86_64}
.endif

.if "${TARGET_ARCH}" == "arm64" || "${TARGET_ARCH}" == "aarch64" || "${TARGET_ARCH}" == "ARM64"
    SUBDIR += ${SUBDIR_ARM64}
.endif

.if "${TARGET_ARCH}" == "arm" || "${TARGET_ARCH}" == "ARM"
    SUBDIR += ${SUBDIR_ARM}
.endif

all: kernelcache

.for target_dir in ${RAVYN_SDKROOT}/System/Library/Extensions
	mkdir -p ${target_dir}
.for kext in ${SUBDIR}
	.if ${kext} == "IOGraphics"
		cp -fR ${.OBJDIR}/${kext}/IOGOPFramebuffer.kext ${target_dir}/
	.else
		cp -fR ${.OBJDIR}/${kext}/${kext}.kext ${target_dir}/
	.endif
.endfor
.endfor

EXTENSIONS = ${ROOT_BINARY_DIR}${ROOT_SOURCE_DIR}/Kernel/Extensions
XNU_OBJDIR = ${ROOT_BINARY_DIR}${ROOT_SOURCE_DIR}/Kernel/xnu

KEXTS_BASE = ${EXTENSIONS}/IOACPIFamily/IOACPIFamily.kext ${EXTENSIONS}/AppleAPIC/AppleAPIC.kext ${EXTENSIONS}/IOPCIFamily/IOPCIFamily.kext ${EXTENSIONS}/corecrypto/corecrypto.kext ${EXTENSIONS}/pthread/pthread.kext ${EXTENSIONS}/IOStorageFamily/IOStorageFamily.kext

KEXTS_X86_64 = ${EXTENSIONS}/IOATAFamily/IOATAFamily.kext ${EXTENSIONS}/RavynAHCIPort/RavynAHCIPort.kext ${EXTENSIONS}/RavynXHCIPort/RavynXHCIPort.kext ${EXTENSIONS}/IOGraphics/IOGOPFramebuffer.kext ${EXTENSIONS}/AppleFileSystemDriver/AppleFileSystemDriver.kext ${EXTENSIONS}/msdosfs/msdosfs.kext ${EXTENSIONS}/hfs/hfs_encodings/hfs_encodings.kext ${EXTENSIONS}/hfs/hfs.kext

KEXTS_ARM = ${EXTENSIONS}/AndroidPlatform/AndroidPlatform.kext

kernelcache: ${SUBDIR}
	@echo "Generating prelinked kernel cache for ${TARGET_ARCH}..."
	.if "${TARGET_ARCH}" == "X86_64" || "${TARGET_ARCH}" == "x86_64" || "${TARGET_ARCH}" == "amd64"
		${TOOLS}/plktool ${.OBJDIR}/${.TARGET}.tmp ${XNU_OBJDIR}/sym/kernel ${XNU_OBJDIR}/sym/System.kext ${KEXTS_BASE} ${KEXTS_X86_64}
		@echo "  X86_64 KEXTs included"
	.else
		${TOOLS}/plktool ${.OBJDIR}/${.TARGET}.tmp ${XNU_OBJDIR}/sym/kernel ${XNU_OBJDIR}/sym/System.kext ${KEXTS_BASE} ${KEXTS_ARM}
		@echo "  ARM KEXTs included (Intel KEXTs excluded)"
	.endif
	chmod 0644 ${.TARGET}.tmp
	cp -f ${.OBJDIR}/${.TARGET} ${ROOT_BINARY_DIR}
	@echo "Kernelcache generated!"

show-kexts:
	@echo "KEXTs for ${TARGET_ARCH}:"
	@echo "  Common: ${SUBDIR_COMMON}"
	.if "${TARGET_ARCH}" == "X86_64" || "${TARGET_ARCH}" == "x86_64" || "${TARGET_ARCH}" == "amd64"
		@echo "  X86_64: ${SUBDIR_X86_ONLY} ${SUBDIR_X86_64}"
	.else
		@echo "  (Intel KEXTs excluded for ARM)"
	.endif
	.if "${TARGET_ARCH}" == "arm64" || "${TARGET_ARCH}" == "aarch64"
		@echo "  ARM64: ${SUBDIR_ARM64}"
	.endif

.include <bsd.subdir.mk>
