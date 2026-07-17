#ifndef _IOKIT_SHIM_H
#define _IOKIT_SHIM_H

// Minimal IOKit shims for Linux host compilation
// These are NOT functional and only exist to satisfy the compiler.

#include <cstddef>
#include <cstring>
#include <cstdint>

typedef unsigned int        UInt32;
typedef unsigned short      UInt16;
typedef unsigned char       UInt8;
typedef unsigned long long  UInt64;
typedef int                 SInt32;
typedef unsigned long       IOVirtualAddress;
typedef unsigned long long  IOByteCount;
typedef unsigned long long  IOByteOffset;

typedef UInt32              IOReturn;

#define kIOReturnSuccess 0
#define kIOReturnBadArgument 1
#define kIOReturnTimeout 2
#define kIOReturnIOError 3

#define kIODirectionInOut 0

class OSObject {
public:
    void retain() {}
    void release() {}
};

class IOService : public OSObject {
public:
    virtual OSObject* getProperty(const char* key) { (void)key; return nullptr; }
    virtual bool start(IOService* provider) { (void)provider; return true; }
    virtual void stop(IOService* provider) { (void)provider; }
    virtual void free() {}
    virtual void registerService() {}
    virtual IOReturn registerInterrupt(int index, void* handler, void* target, void* refcon, void* dispatchSource) { (void)index; (void)handler; (void)target; (void)refcon; (void)dispatchSource; return kIOReturnSuccess; }
};

class IOMemoryDescriptor : public OSObject {
public:
    static IOMemoryDescriptor* withPhysicalAddress(UInt64 physBase, UInt64 physSize, int direction) { (void)physBase; (void)physSize; (void)direction; return nullptr; }
    IOMemoryMap* map(UInt64 offset, UInt64 length, int direction) { (void)offset; (void)length; (void)direction; return nullptr; }
};

class IOMemoryMap : public OSObject {
public:
    IOVirtualAddress getVirtualAddress() { return 0; }
};

class OSDictionary : public OSObject {};
class OSString : public OSObject {
public:
    static OSString* withCString(const char* str) { (void)str; return nullptr; }
    const char* getCStringNoCopy() { return ""; }
};
class OSArray : public OSObject {
public:
    unsigned int getCount() { return 0; }
};
class OSNumber : public OSObject {
public:
    UInt32 unsigned32BitValue() { return 0; }
    UInt64 unsigned64BitValue() { return 0; }
};

typedef void (*IOFilterInterruptAction)(OSObject* owner, void* refCon,
                                         int interruptSource);
typedef void (*IOInterruptActionHandler)(OSObject* owner, void* refCon,
                                         IOFilterInterruptAction action,
                                         int interruptSource);

#define OSDeclareDefaultStructors(CLASS)
#define OSDeclareDefaultStructorsWithInit(CLASS, INIT)
#define OSDefineMetaClassAndStructors(CLASS, SUPERCLASS)
#define OSDynamicCast(CLASS, OBJ) (static_cast<CLASS*>(OBJ))
#define DLOG(fmt, ...) // Suppress DLOG for host build
#define kprintf(fmt, ...) // Suppress kprintf for host build
#define IOSleep(ms) // Suppress IOSleep for host build

// Dummy definitions for IOKitKeys
#define kIONameMatchKey "IONameMatch"

#endif // _IOKIT_SHIM_H