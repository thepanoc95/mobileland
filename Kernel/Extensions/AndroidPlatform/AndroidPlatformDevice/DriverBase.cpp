/*
 * DriverBase.cpp
 * MobileLand (OS) - Common driver base class implementation
 */

#include "DriverBase.hpp"
#include <kern/debug.h>

OSDefineAbstractStructors(DriverBase, IOService);

bool DriverBase::init(OSDictionary *dict)
{
    if (!IOService::init(dict)) {
        return false;
    }
    
    fDeviceName = nullptr;
    fDeviceClass = nullptr;
    fHwRevision = 0;
    fRegBase = 0;
    fRegSize = 0;
    fClockNames = nullptr;
    fInterruptIndex = -1;
    fInterruptEnabled = false;
    
    return true;
}

void DriverBase::free()
{
    if (fClockNames) {
        fClockNames->release();
        fClockNames = nullptr;
    }
    
    IOService::free();
}

bool DriverBase::matchDevice(const char *compatible)
{
    if (!compatible || !fDeviceClass) {
        return false;
    }
    
    return (strcmp(compatible, fDeviceClass) == 0);
}

uint32_t DriverBase::readReg32(uint32_t offset)
{
    if (fRegBase == 0 || offset >= fRegSize) {
        kprintf("[DriverBase] readReg32: invalid offset 0x%x\n", offset);
        return 0xFFFFFFFF;
    }
    
    volatile uint32_t *reg = (volatile uint32_t *)(fRegBase + offset);
    return *reg;
}

void DriverBase::writeReg32(uint32_t offset, uint32_t value)
{
    if (fRegBase == 0 || offset >= fRegSize) {
        kprintf("[DriverBase] writeReg32: invalid offset 0x%x\n", offset);
        return;
    }
    
    volatile uint32_t *reg = (volatile uint32_t *)(fRegBase + offset);
    *reg = value;
}

uint64_t DriverBase::readReg64(uint32_t offset)
{
    if (fRegBase == 0 || offset + 4 >= fRegSize) {
        kprintf("[DriverBase] readReg64: invalid offset 0x%x\n", offset);
        return 0xFFFFFFFFFFFFFFFFULL;
    }
    
    volatile uint64_t *reg = (volatile uint64_t *)(fRegBase + offset);
    return *reg;
}

void DriverBase::writeReg64(uint32_t offset, uint64_t value)
{
    if (fRegBase == 0 || offset + 4 >= fRegSize) {
        kprintf("[DriverBase] writeReg64: invalid offset 0x%x\n", offset);
        return;
    }
    
    volatile uint64_t *reg = (volatile uint64_t *)(fRegBase + offset);
    *reg = value;
}

bool DriverBase::enableClock(const char *clockName)
{
    if (!clockName) {
        return false;
    }
    
    // In a full implementation, this would:
    // 1. Look up the clock in the GCC (Global Clock Controller)
    // 2. Enable the clock via the clock driver
    // 3. Optionally set the frequency
    
    kprintf("[DriverBase] enableClock: %s\n", clockName);
    return true;
}

bool DriverBase::disableClock(const char *clockName)
{
    if (!clockName) {
        return false;
    }
    
    kprintf("[DriverBase] disableClock: %s\n", clockName);
    return true;
}

bool DriverBase::enablePower(const char *powerDomain)
{
    if (!powerDomain) {
        return false;
    }
    
    kprintf("[DriverBase] enablePower: %s\n", powerDomain);
    return true;
}

bool DriverBase::disablePower(const char *powerDomain)
{
    if (!powerDomain) {
        return false;
    }
    
    kprintf("[DriverBase] disablePower: %s\n", powerDomain);
    return true;
}

IOReturn DriverBase::registerInterrupt(int index, void *handler, void *refCon)
{
    fInterruptIndex = index;
    
    // Register with the platform interrupt controller
    IOReturn ret = IOService::registerInterrupt(this, index, handler, this, refCon);
    
    if (ret == kIOReturnSuccess) {
        kprintf("[DriverBase] registerInterrupt: index=%d registered\n", index);
    } else {
        kprintf("[DriverBase] registerInterrupt: failed index=%d ret=0x%x\n", index, ret);
    }
    
    return ret;
}

IOReturn DriverBase::enableInterrupt(int index)
{
    if (index < 0) {
        return kIOReturnBadArgument;
    }
    
    fInterruptEnabled = true;
    return IOService::enableInterrupt(this, index);
}

IOReturn DriverBase::disableInterrupt(int index)
{
    if (index < 0) {
        return kIOReturnBadArgument;
    }
    
    fInterruptEnabled = false;
    return IOService::disableInterrupt(this, index);
}
