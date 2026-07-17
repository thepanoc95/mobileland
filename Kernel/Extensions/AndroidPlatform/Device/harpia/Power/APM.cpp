/*
 * APM.cpp - Application Power Manager
 * MobileLand (OS) - Harpia (ARM32)
 *
 * Power management driver for MSM8916 ARM32
 */

#if DEVICE_HARPIA

#include <IOKit/IOService.h>
#include <DriverBase.hpp>

class HarpiaAPM : public IOService
{
    OSDeclareDefaultStructors(HarpiaAPM);

public:
    virtual bool start(IOService *provider) override;
    virtual void stop(IOService *provider) override;
};

OSDefineMetaClassAndStructors(HarpiaAPM, IOService);

bool HarpiaAPM::start(IOService *provider)
{
    if (!IOService::start(provider)) {
        return false;
    }
    
    kprintf("[HarpiaAPM] start\n");
    kprintf("[HarpiaAPM] battery: present=%d level=%d%%\n", 1, 100);
    
    registerService();
    return true;
}

void HarpiaAPM::stop(IOService *provider)
{
    kprintf("[HarpiaAPM] stop\n");
    IOService::stop(provider);
}

#endif /* DEVICE_HARPIA */
