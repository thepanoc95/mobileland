#include "AndroidPlatformDevice.h"

OSDefineMetaClassAndStructors(AndroidPlatformDevice, IOService)

#define ANDROID_DEBUG 1

#if ANDROID_DEBUG
#define DLOG(fmt, ...) \
    kprintf("[AndroidPlatformDevice] " fmt "\n", ##__VA_ARGS__)
#else
#define DLOG(fmt, ...)
#endif

static OSObject* getPropertyFromProvider(IOService* provider,
                                         const char* key)
{
    OSObject* prop = provider->getProperty(key);
    if (prop == nullptr) {
        /* Try the IOService name-based matching fallback. */
        prop = provider->getProperty(kIONameMatchKey);
    }
    return prop;
}

IOService* AndroidPlatformDevice::probe(IOService* provider, SInt32* score)
{
    OSString* compatible = OSDynamicCast(OSString,
        getPropertyFromProvider(provider, kAndroidCompatibleKey));

    if (compatible == nullptr) {
        DLOG("probe: no compatible property, no match\n");
        return nullptr;
    }

    DLOG("probe: matched compatible='%s'\n", compatible->getCStringNoCopy());

    /* Set a reasonable probe score. Higher than default so we win
     * against any generic IOPlatformDevice driver. */
    if (score) *score = 5000;

    return this;
}

bool AndroidPlatformDevice::start(IOService* provider)
{
    if (!IOService::start(provider)) {
        DLOG("start: superclass start failed\n");
        return false;
    }

    /* Device name */
    fDeviceName = OSDynamicCast(OSString,
        getPropertyFromProvider(provider, kAndroidDeviceNameKey));
    if (fDeviceName == nullptr) {
        fDeviceName = OSString::withCString("unknown");
    } else {
        fDeviceName->retain();
    }
    DLOG("start: device '%s'\n", fDeviceName->getCStringNoCopy());

    /* Compatible string */
    fCompatible = OSDynamicCast(OSString,
        getPropertyFromProvider(provider, kAndroidCompatibleKey));
    if (fCompatible) fCompatible->retain();

    /* Bus type */
    OSString* busTypeStr = OSDynamicCast(OSString,
        getPropertyFromProvider(provider, kAndroidBusTypeKey));
    if (busTypeStr) {
        const char* bt = busTypeStr->getCStringNoCopy();
        if (strcmp(bt, "i2c") == 0)      fBusType = kAndroidBusTypeI2C;
        else if (strcmp(bt, "spi") == 0) fBusType = kAndroidBusTypeSPI;
        else if (strcmp(bt, "uart") == 0) fBusType = kAndroidBusTypeUART;
        else                              fBusType = kAndroidBusTypeMMIO;
    } else {
        fBusType = kAndroidBusTypeMMIO;
    }

    /* Bus address (I2C slave address, SPI chip-select, etc.) */
    OSNumber* busAddrNum = OSDynamicCast(OSNumber,
        getPropertyFromProvider(provider, kAndroidBusAddrKey));
    fBusAddress = busAddrNum ? busAddrNum->unsigned32BitValue() : 0;

    DLOG("start: busType=%d busAddr=0x%x\n", fBusType, fBusAddress);

    /* --- Map register space --- */

    OSNumber* regBaseNum = OSDynamicCast(OSNumber,
        getPropertyFromProvider(provider, kAndroidRegBaseKey));
    OSNumber* regSizeNum = OSDynamicCast(OSNumber,
        getPropertyFromProvider(provider, kAndroidRegSizeKey));

    if (regBaseNum && regSizeNum) {
        UInt64 physBase = regBaseNum->unsigned64BitValue();
        UInt64 physSize = regSizeNum->unsigned64BitValue();

        DLOG("start: reg base=0x%llx size=0x%llx\n", physBase, physSize);

        /* Create a memory descriptor for the physical register space. */
        fRegDescriptor = IOMemoryDescriptor::withPhysicalAddress(
            physBase, physSize, kIODirectionInOut);

        if (fRegDescriptor == nullptr) {
            DLOG("start: failed to create memory descriptor\n");
            return false;
        }

        /* Map it into the kernel virtual address space. */
        fRegMap = fRegDescriptor->map(
            0,                    /* offset into descriptor */
            0,                    /* length (0 = entire descriptor) */
            kIODirectionInOut);

        if (fRegMap == nullptr) {
            DLOG("start: failed to map register space\n");
            fRegDescriptor->release();
            fRegDescriptor = nullptr;
            return false;
        }

        fRegBase = fRegMap->getVirtualAddress();
        fRegSize = (IOByteCount)physSize;

        DLOG("start: reg mapped at vaddr=0x%lx\n", (unsigned long)fRegBase);
    } else {
        DLOG("start: no register space in device tree\n");
        fRegDescriptor = nullptr;
        fRegMap = nullptr;
        fRegBase = 0;
        fRegSize = 0;
    }

    /* --- Parse interrupt info --- */

    OSNumber* irqNum = OSDynamicCast(OSNumber,
        getPropertyFromProvider(provider, kAndroidInterruptKey));
    if (irqNum) {
        fInterruptIndex = (int)irqNum->unsigned32BitValue();
        DLOG("start: interrupt index=%d\n", fInterruptIndex);
    } else {
        fInterruptIndex = -1;
    }
    fInterruptRegistered = false;

    /* --- Parse clock names --- */

    /* Clocks may be an array of clock name strings. */
    fClockNames = OSDynamicCast(OSArray,
        getPropertyFromProvider(provider, kAndroidClockKey));
    if (fClockNames) fClockNames->retain();

    /* --- Enable clocks --- */

    enableClock();

    /* --- Register service --- */

    /* Publish this device so other drivers (or the platform expert)
     * can find it in the IORegistry. */
    registerService();

    DLOG("start: success\n");
    return true;
}

void AndroidPlatformDevice::stop(IOService* provider)
{
    DLOG("stop\n");

    /* Disable interrupts if registered. */
    if (fInterruptRegistered) {
        disableInterrupt(fInterruptIndex);
        fInterruptRegistered = false;
    }

    /* Disable clocks. */
    disableClock();

    IOService::stop(provider);
}

void AndroidPlatformDevice::free(void)
{
    DLOG("free\n");

    /* Release register mapping. */
    if (fRegMap) {
        fRegMap->release();
        fRegMap = nullptr;
    }
    if (fRegDescriptor) {
        fRegDescriptor->release();
        fRegDescriptor = nullptr;
    }

    /* Release device tree properties. */
    if (fDeviceName) {
        fDeviceName->release();
        fDeviceName = nullptr;
    }
    if (fCompatible) {
        fCompatible->release();
        fCompatible = nullptr;
    }
    if (fClockNames) {
        fClockNames->release();
        fClockNames = nullptr;
    }

    IOService::free();
}

UInt32 AndroidPlatformDevice::read32(IOByteOffset offset)
{
    if (fRegBase == 0 || offset + sizeof(uint32_t) > fRegSize) {
        DLOG("read32: invalid access offset=0x%x\n", offset);
        return 0xFFFFFFFF;
    }
    volatile uint32_t* reg =
        (volatile uint32_t*)(fRegBase + offset);
    return *reg;
}

void AndroidPlatformDevice::write32(IOByteOffset offset, UInt32 value)
{
    if (fRegBase == 0 || offset + sizeof(uint32_t) > fRegSize) {
        DLOG("write32: invalid access offset=0x%x\n", offset);
        return;
    }
    volatile uint32_t* reg =
        (volatile uint32_t*)(fRegBase + offset);
    *reg = value;
}

UInt16 AndroidPlatformDevice::read16(IOByteOffset offset)
{
    if (fRegBase == 0 || offset + sizeof(uint16_t) > fRegSize) {
        return 0xFFFF;
    }
    return *(volatile uint16_t*)(fRegBase + offset);
}

void AndroidPlatformDevice::write16(IOByteOffset offset, UInt16 value)
{
    if (fRegBase == 0 || offset + sizeof(uint16_t) > fRegSize) return;
    *(volatile uint16_t*)(fRegBase + offset) = value;
}

UInt8 AndroidPlatformDevice::read8(IOByteOffset offset)
{
    if (fRegBase == 0 || offset + 1 > fRegSize) return 0xFF;
    return *(volatile uint8_t*)(fRegBase + offset);
}

void AndroidPlatformDevice::write8(IOByteOffset offset, UInt8 value)
{
    if (fRegBase == 0 || offset + 1 > fRegSize) return;
    *(volatile uint8_t*)(fRegBase + offset) = value;
}

/* ===================================================================== *
 * Interrupt handling
 * ===================================================================== */

IOReturn AndroidPlatformDevice::registerInterrupt(int index,
    IOInterruptActionHandler handler, void* target, void* refcon)
{
    if (index < 0 || fRegBase == 0) {
        return kIOReturnBadArgument;
    }

    /* Register the interrupt with the XNU interrupt controller framework.
     * On MSM8916 with GIC v2, the interrupt index maps directly to the
     * SPI (Shared Peripheral Interrupt) number from the device tree. */
    IOReturn ret = IOService::registerInterrupt(
        this,
        index,
        handler,
        target,
        refcon,
        0
    );

    if (ret == kIOReturnSuccess) {
        fInterruptRegistered = true;
        DLOG("registerInterrupt: index=%d registered\n", index);
    } else {
        DLOG("registerInterrupt: failed index=%d ret=0x%x\n", index, ret);
    }

    return ret;
}

void AndroidPlatformDevice::enableInterrupt(int index)
{
    if (index < 0) return;
    /* On GIC v2, this would call into the platform expert's
     * interrupt enable function. Stub for now — the GIC v2
     * driver in XNU handles unmasking. */
    DLOG("enableInterrupt: index=%d\n", index);
}

void AndroidPlatformDevice::disableInterrupt(int index)
{
    if (index < 0) return;
    DLOG("disableInterrupt: index=%d\n", index);
}

/* ===================================================================== *
 * Clock & power management
 * ===================================================================== */

IOReturn AndroidPlatformDevice::enableClock(void)
{
    /* Stub: In a real implementation, this would:
     * 1. Look up each clock name in fClockNames
     * 2. Find the corresponding clock controller driver in the IORegistry
     * 3. Call the clock driver's enable function
     *
     * On MSM8916, clocks are managed by the GCC (Global Clock Controller)
     * block. The driver for GCC would be a separate IOKit driver that
     * publishes an IOClockController service.
     */
    DLOG("enableClock: %d clocks\n",
         fClockNames ? (int)fClockNames->getCount() : 0);
    return kIOReturnSuccess;
}

IOReturn AndroidPlatformDevice::disableClock(void)
{
    DLOG("disableClock\n");
    return kIOReturnSuccess;
}

/* ===================================================================== *
 * I2C/SPI bus interface (optional, for peripheral buses)
 * ===================================================================== */

UInt32 AndroidPlatformDevice::busReadReg(UInt8 regAddr, UInt8 regSize)
{
    /* Stub: subclass overrides this for I2C/SPI devices.
     * The subclass would:
     * 1. Format a register-read command based on regSize
     * 2. Send it via the I2C/SPI controller driver
     * 3. Read the response */
    DLOG("busReadReg: addr=0x%02x size=%d (stub)\n", regAddr, regSize);
    return 0xFFFFFFFF;
}

IOReturn AndroidPlatformDevice::busWriteReg(UInt8 regAddr,
    UInt32 value, UInt8 regSize)
{
    DLOG("busWriteReg: addr=0x%02x val=0x%x size=%d (stub)\n",
         regAddr, value, regSize);
    return kIOReturnSuccess;
}

const char* AndroidPlatformDevice::getDeviceName(void)
{
    return fDeviceName ? fDeviceName->getCStringNoCopy() : "unknown";
}

AndroidBusType AndroidPlatformDevice::getBusType(void)
{
    return fBusType;
}

UInt32 AndroidPlatformDevice::getBusAddress(void)
{
    return fBusAddress;
}