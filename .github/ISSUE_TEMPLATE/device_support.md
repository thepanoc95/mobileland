---
name: Add Device Support
description: Request support for a new device in MobileLand(OS)
title: "[DEVICE] "
labels: ["device-support", "triage"]
assignees: []
---

## Device Information

### Device Details
- **Device Name**: (e.g., Motorola Moto G4, Xiaomi Redmi Note 4)
- **Model Number(s)**: (e.g., XT1622, MRA58K)
- **Manufacturer**: (e.g., Motorola, Xiaomi)

### SoC & Architecture
- **SoC (System-on-Chip)**: (e.g., Snapdragon 410, MSM8916)
- **Architecture**: ARM / ARM64 (specify)
- **CPU Cores**: (e.g., Quad-core)
- **RAM**: (e.g., 2GB, 4GB)

### lk2nd Support
- **lk2nd Support Status**: 
  - [ ] Already supported in lk2nd
  - [ ] Likely supported (Qualcomm SoC in supported list)
  - [ ] Has a mainline Linux kernel
  - [ ] Unsure / needs verification
  
**Link to lk2nd Device Support**: [here](https://github.com/msm8916-mainline/lk2nd/blob/main/Documentation/devices.md)

---

## Hardware Specifications

### Storage & Memory
- **Internal Storage**: (e.g., 16GB, 64GB)
- **RAM Configuration**: (e.g., 2GB, 4GB)
- **Storage Type**: eMMC / UFS

### Display
- **Resolution**: (e.g., 1920x1080, 1440x2880)
- **Diagonal**: (e.g., 5.5 inches)

### Connectivity
- **Wi-Fi**: (e.g., 802.11 a/b/g/n/ac)
- **Bluetooth**: (e.g., 4.2, 5.0)
- **USB Version**: (e.g., USB 2.0, USB 3.0)
- **Other**: (NFC, GPS, etc.)

### Boot & Recovery
- **Stock Bootloader**: (e.g., LK, ABL, XBL)
- **Recovery Method**: (e.g., fastboot, adb, EDL)
- **Unlockable**: Yes / No / Unknown
- **A/B Partition model**: Yes / No / Unknown  

---

## Testing & Availability

- **Device Availability**: 
  - [ ] Have access to the device
  - [ ] Can obtain the device
  - [ ] Know someone with access

- **Willing to Test**: 
  - [ ] Yes, can perform testing
  - [ ] Willing to collaborate with maintainers
  - [ ] Available for bug reports & feedback

---

## Additional Information

### Relevant Links
- Link to device specifications: 
- Link to stock ROM source (if available):
- Link to any existing port attempts:

### Notes
(Any additional context about this device, why it's a good candidate, known issues, etc.)

---

## Checklist

Before submitting, please verify:

- [ ] Device uses a **Qualcomm SoC** (non-Qualcomm devices are not supported yet)
- [ ] The SoC is **supported by lk2nd** (or likely to be)
- [ ] You've checked [lk2nd's supported devices list](https://github.com/msm8916-mainline/lk2nd/blob/main/Documentation/devices.md) for your SoC
- [ ] You've provided accurate device specifications
- [ ] You understand this is a feature request and may take time to implement
- [ ] You have access to a device tree or have it already

---

### Support Status

> **Important**: MobileLand(OS) currently supports **Qualcomm devices with SoCs backed by lk2nd**. If your device does not meet these requirements, this issue will be suspended. Non-Qualcomm SoC support is not available yet.
>
> Check the [lk2nd project](https://github.com/msm8916-mainline/lk2nd) to confirm your SoC is supported before opening this issue.
