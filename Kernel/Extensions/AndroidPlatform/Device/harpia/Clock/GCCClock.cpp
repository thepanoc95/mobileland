/*
 * GCCClock.cpp - Global Clock Controller Driver
 * MobileLand (OS) - Harpia (ARM32)
 *
 * Clock management driver for MSM8916 ARM32
 */

#if DEVICE_HARPIA

#include <IOKit/IOService.h>
#include <DriverBase.hpp>

class HarpiaClock : public IOService
{
    OSDeclareDefaultStructors(HarpiaClock);

public:
    virtual bool start(IOService *provider) override;
    virtual void stop(IOService *provider) override;
    
    virtual bool enableClock(int clockId);
    virtual bool disableClock(int clockId);

private:
    uint32_t fEnabledClocks;
};

OSDefineMetaClassAndStructors(HarpiaClock, IOService);

bool HarpiaClock::start(IOService *provider)
{
    if (!IOService::start(provider)) {
        return false;
    }
    
    kprintf("[HarpiaClock] start\n");
    fEnabledClocks = 0;
    
    registerService();
    return true;
}

void HarpiaClock::stop(IOService *provider)
{
    kprintf("[HarpiaClock] stop\n");
    IOService::stop(provider);
}

bool HarpiaClock::enableClock(int clockId)
{
    fEnabledClocks |= (1 << clockId);
    kprintf("[HarpiaClock] enable clock %d\n", clockId);
    return true;
}

bool HarpiaClock::disableClock(int clockId)
{
    fEnabledClocks &= ~(1 << clockId);
    kprintf("[HarpiaClock] disable clock %d\n", clockId);
    return true;
}

#endif /* DEVICE_HARPIA */
