/*
 * DriverBase.hpp
 * MobileLand (OS) - Common driver base class
 *
 * Base class for all Android platform device drivers
 */

#ifndef _DRIVER_BASE_HPP
#define _DRIVER_BASE_HPP

#include <IOKit/IOService.h>
#include <IOKit/IOInterruptController.h>

// Device identification
#define DEVICE_CLASS_MSM8916      "msm8916"
#define DEVICE_CLASS_HARPIA       "harpia"
#define DEVICE_CLASS_HARPIA64     "harpia64"

// Base address definitions for MSM8916
#define MSM8916_UART_BASE        0x078B0000
#define MSM8916_UART_SIZE        0x1000

#define MSM8916_GIC_DIST_BASE    0x0B000000
#define MSM8916_GIC_CPU_BASE     0x0B002000
#define MSM8916_GIC_SIZE         0x1000

#define MSM8916_TLMM_BASE        0x01000000
#define MSM8916_TLMM_SIZE        0x80000

#define MSM8916_CLK_CTL_BASE     0x00100000
#define MSM8916_CLK_CTL_SIZE     0x40000

#define MSM8916_MPM_BASE         0x00400000
#define MSM8916_MPM_SIZE         0x1000

#define MSM8916_PMIC_BASE        0x00500000
#define MSM8916_PMIC_SIZE        0x1000

// MSM8916 Hardware Version
#define MSM8916_HW_REVISION_1_0  0x10000
#define MSM8916_HW_REVISION_1_1   0x10100
#define MSM8916_HW_REVISION_1_2   0x10200

class DriverBase : public IOService
{
    OSDeclareAbstractStructors(DriverBase);

public:
    // Initialization
    virtual bool init(OSDictionary *dict = nullptr) override;
    virtual void free() override;
    
    // Device matching
    virtual bool matchDevice(const char *compatible);
    
    // Register access
    virtual uint32_t readReg32(uint32_t offset);
    virtual void writeReg32(uint32_t offset, uint32_t value);
    virtual uint64_t readReg64(uint32_t offset);
    virtual void writeReg64(uint32_t offset, uint64_t value);
    
    // Clock management
    virtual bool enableClock(const char *clockName);
    virtual bool disableClock(const char *clockName);
    
    // Power management
    virtual bool enablePower(const char *powerDomain);
    virtual bool disablePower(const char *powerDomain);
    
    // Interrupt handling
    virtual IOReturn registerInterrupt(int index, void *handler, void *refCon);
    virtual IOReturn enableInterrupt(int index);
    virtual IOReturn disableInterrupt(int index);
    
protected:
    // Device info
    const char *fDeviceName;
    const char *fDeviceClass;
    uint32_t fHwRevision;
    
    // Register base
    vm_address_t fRegBase;
    uint32_t fRegSize;
    
    // Clock references
    OSArray *fClockNames;
    
    // Interrupt info
    int fInterruptIndex;
    bool fInterruptEnabled;
};

// Inline register access helpers
static inline uint32_t ioread32(volatile void *addr)
{
    return *(volatile uint32_t *)addr;
}

static inline void iowrite32(volatile void *addr, uint32_t value)
{
    *(volatile uint32_t *)addr = value;
}

static inline uint64_t ioread64(volatile void *addr)
{
    return *(volatile uint64_t *)addr;
}

static inline void iowrite64(volatile void *addr, uint64_t value)
{
    *(volatile uint64_t *)addr = value;
}

// Register bit operations
static inline uint32_t setbits32(volatile uint32_t *addr, uint32_t mask)
{
    return iowrite32(addr, ioread32(addr) | mask);
}

static inline uint32_t clrbits32(volatile uint32_t *addr, uint32_t mask)
{
    return iowrite32(addr, ioread32(addr) & ~mask);
}

static inline uint32_t wrbits32(volatile uint32_t *addr, uint32_t mask, uint32_t value)
{
    return iowrite32(addr, (ioread32(addr) & ~mask) | (value & mask));
}

#endif /* _DRIVER_BASE_HPP */
