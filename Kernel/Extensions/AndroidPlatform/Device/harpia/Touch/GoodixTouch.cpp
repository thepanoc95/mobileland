/*
 * GoodixTouch.cpp - Goodix Touchscreen Driver
 * MobileLand (OS) - Harpia (ARM32)
 *
 * Driver for Goodix GT9xx touchscreen controllers
 */

#if DEVICE_HARPIA

#include <IOKit/IOService.h>
#include <DriverBase.hpp>

#define GOODIX_I2C_ADDR           0x14

class HarpiaTouch : public IOService
{
    OSDeclareDefaultStructors(HarpiaTouch);

public:
    virtual bool start(IOService *provider) override;
    virtual void stop(IOService *provider) override;

private:
    bool initHardware();
};

OSDefineMetaClassAndStructors(HarpiaTouch, IOService);

bool HarpiaTouch::start(IOService *provider)
{
    if (!IOService::start(provider)) {
        return false;
    }
    
    kprintf("[HarpiaTouch] start\n");
    
    if (!initHardware()) {
        return false;
    }
    
    registerService();
    return true;
}

void HarpiaTouch::stop(IOService *provider)
{
    kprintf("[HarpiaTouch] stop\n");
    IOService::stop(provider);
}

bool HarpiaTouch::initHardware()
{
    // Initialize touchscreen
    // fMaxX = 720;
    // fMaxY = 1280;
    kprintf("[HarpiaTouch] initialized: 720x1280\n");
    return true;
}

#endif /* DEVICE_HARPIA */
