 #include <stdint.h>
 #include <stddef.h>
 #include <string.h>
 
 #ifndef SINGLE_FILE_BOOTLOADER_H
 #define SINGLE_FILE_BOOTLOADER_H

 #ifdef __QEMU_TEST_MODE
 #define __QEMU_TEST_MODE 1
 #endif

 /*
  * QEMU Test Mode Implementation
  * 
  * This provides a standalone test environment for the bootloader
  * without requiring lk2nd on QEMU targets. It simulates basic
  * hardware initialization and provides test kernels.
  */
 
 #ifdef __QEMU_TEST_MODE
 
 #if __QEMU_TEST_MODE
 
 /* Test mode configuration */
 #define TEST_MODE_ENABLED 1
 #define TEST_KERNEL_SELF 1  /* Test kernel that just prints and halts */
 
 /* Test helper functions */
 static void test_mode_init(void) {
     /* Minimal initialization for QEMU test environment */
     debug_print("[TEST] QEMU Test Mode Initialized\n");
 }
 
 static void test_run_self(void) {
     /* Run a minimal self-test kernel that doesn't boot XNU */
     debug_print("[TEST] Running self-test kernel...\n");
     
     /* Simulate a simple kernel that prints and halts */
     debug_print("[TEST] ====================================\n");
     debug_print("[TEST]    STAGE3 BOOTLOADER SELF-TEST\n");
     debug_print("[TEST] ====================================\n");
     debug_print("[TEST] Memory layout verification:\n");
     debug_print("[TEST]   BOOTLOADER_BASE:  0x");
     debug_hex(BOOTLOADER_BASE, 8);
     debug_print("\n");
     debug_print("[TEST]   KERNEL_LOAD_BASE: 0x");
     debug_hex(KERNEL_LOAD_BASE, 8);
     debug_print("\n");
     debug_print("[TEST]   DEVICE_TREE_BASE: 0x");
     debug_hex(DEVICE_TREE_BASE, 8);
     debug_print("\n");
     debug_print("[TEST]   FBMEM_BASE:      0x");
     debug_hex(FBMEM_BASE, 8);
     debug_print("\n");
     debug_print("[TEST] Display initialization:\n");
     debug_print("[TEST]   Resolution: ");
     debug_hex(DISPLAY_WIDTH, 4);
     debug_print("x");
     debug_hex(DISPLAY_HEIGHT, 4);
     debug_print("\n");
     debug_print("[TEST]   Depth: ");
     debug_hex(DISPLAY_DEPTH, 2);
     debug_print(" bpp\n");
     debug_print("[TEST] Test complete. Halting.\n");
 }
 
 /* Skip to test mode if enabled */
 #if TEST_MODE_ENABLED
 #define QEMU_TEST_INIT() test_mode_init()
 #define QEMU_TEST_SELF() test_run_self()
 #else
 #define QEMU_TEST_INIT() ((void)0)
 #define QEMU_TEST_SELF() ((void)0)
 #endif
 
 #else /* !__QEMU_TEST_MODE */
 #define QEMU_TEST_INIT() ((void)0)
 #define QEMU_TEST_SELF() ((void)0)
 #endif /* __QEMU_TEST_MODE */
 
 #else /* !__QEMU_TEST_MODE */
 #define QEMU_TEST_INIT() ((void)0)
 #define QEMU_TEST_SELF() ((void)0)
 #endif /* __QEMU_TEST_MODE */
 
 void *memcpy(void *dest, const void *src, size_t n) {
     uint8_t *d = (uint8_t *)dest;
     const uint8_t *s = (const uint8_t *)src;
     for (size_t i = 0; i < n; ++i) {
         d[i] = s[i];
     }
     return dest;
 }
 
 void *memset(void *s, int c, size_t n) {
     uint8_t *p = (uint8_t *)s;
     for (size_t i = 0; i < n; ++i) {
         p[i] = (uint8_t)c;
     }
     return s;
 }
 
 void *__aeabi_memcpy(void *dest, const void *src, size_t n) {
     return memcpy(dest, src, n);
 }
 
 void *__aeabi_memset(void *s, size_t n, int c) {
     return memset(s, c, n);
 }
 
 /* ============================================================================
    CONFIGURATION & PLATFORM DEFINITIONS
    ============================================================================ */
 
 /* Platform identification */
 #define PLATFORM_UNKNOWN     0
 #define PLATFORM_HARPIA      1  /* Motorola Moto G4 (XT1622) - MSM8916 */
 #define PLATFORM_QEMU_ARM    2  /* QEMU ARM generic */
 #define PLATFORM_QEMU_ARM64  3  /* QEMU ARM64 generic */
 
 #ifndef TARGET_PLATFORM
 #define TARGET_PLATFORM PLATFORM_HARPIA
 #endif
 
 /* Architecture detection */
 #if defined(__aarch64__)
 #define ARCH_ARM64 1
 #define ARCH_ARM   0
 #elif defined(__arm__)
 #define ARCH_ARM   1
 #define ARCH_ARM64 0
 #else
 #error "Unsupported architecture"
 #endif
 
 /* Memory layout configuration */
 #if TARGET_PLATFORM == PLATFORM_HARPIA
   /* Motorola Harpia (MSM8916): 2GB RAM, LPAE enabled */
   #define BOOTLOADER_BASE       0x80000000
   #define KERNEL_LOAD_BASE      0x81000000
   #define DEVICE_TREE_BASE      0x82000000
   #define FBMEM_BASE            0x83000000
   #define FBMEM_SIZE            0x00800000  /* 8MB framebuffer */
   #define MAX_KERNEL_SIZE       0x00C00000  /* 12MB max kernel */
   #define MAX_DEVICETREE_SIZE   0x00100000  /* 1MB max DT */
   
   /* Harpia display: ILI9341 over DSI, 480x800 RGB565 */
   #define DISPLAY_WIDTH         480
   #define DISPLAY_HEIGHT        800
   #define DISPLAY_DEPTH         16  /* bits per pixel */
   #define DISPLAY_BPP           (DISPLAY_DEPTH / 8)
 
   #define UART_BASE             0x78B0000
   
 #elif TARGET_PLATFORM == PLATFORM_QEMU_ARM64
   #define BOOTLOADER_BASE       0x40000000
   #define KERNEL_LOAD_BASE      0x40100000
   #define DEVICE_TREE_BASE      0x44000000
   #define FBMEM_BASE            0x48000000
   #define FBMEM_SIZE            0x00800000
   #define MAX_KERNEL_SIZE       0x00C00000
   #define MAX_DEVICETREE_SIZE   0x00100000
   
   #define DISPLAY_WIDTH         1024
   #define DISPLAY_HEIGHT        768
   #define DISPLAY_DEPTH         32
   #define DISPLAY_BPP           (DISPLAY_DEPTH / 8)
   
   #define UART_BASE             0x9000000
   
 #else
   #error "Unsupported platform"
 #endif
 
 /* ============================================================================
    MACH-O BINARY FORMAT DEFINITIONS
    ============================================================================ */
 
 /* Mach-O magic numbers */
 #define MH_MAGIC       0xfeedface   /* 32-bit */
 #define MH_MAGIC_64    0xfeedfacf   /* 64-bit */
 #define MH_CIGAM       0xcefaedfe   /* 32-bit swapped */
 #define MH_CIGAM_64    0xcffaedfe   /* 64-bit swapped */
 
 /* Mach-O CPU types */
 #define CPU_TYPE_ARM   7
 #define CPU_TYPE_ARM64 0x0100000c
 
 /* Mach-O file types */
 #define MH_KERNEL      0x6
 
 /* Mach-O load command types */
 #define LC_SEGMENT     0x1
 #define LC_SEGMENT_64  0x19
 #define LC_MAIN        0x28
 #define LC_UNIXTHREAD  0x5
 
 /* Segment flags */
 #define VM_PROT_READ   1
 #define VM_PROT_WRITE  2
 #define VM_PROT_EXEC   4
 
 typedef struct {
     uint32_t magic;
     uint32_t cputype;
     uint32_t cpusubtype;
     uint32_t filetype;
     uint32_t ncmds;
     uint32_t sizeofcmds;
     uint32_t flags;
     uint32_t reserved;
 } mach_header_64_t;
 
 typedef struct {
     uint32_t cmd;
     uint32_t cmdsize;
 } load_command_t;
 
 typedef struct {
     uint32_t cmd;
     uint32_t cmdsize;
     char     segname[16];
     uint64_t vmaddr;
     uint64_t vmsize;
     uint64_t fileoff;
     uint64_t filesize;
     uint32_t maxprot;
     uint32_t initprot;
     uint32_t nsects;
     uint32_t flags;
 } segment_command_64_t;
 
 typedef struct {
     uint32_t cmd;
     uint32_t cmdsize;
     char     segname[16];
     uint32_t vmaddr;
     uint32_t vmsize;
     uint32_t fileoff;
     uint32_t filesize;
     uint32_t maxprot;
     uint32_t initprot;
     uint32_t nsects;
     uint32_t flags;
 } segment_command_32_t;
 
 typedef struct {
     uint32_t cmd;
     uint32_t cmdsize;
     uint32_t entry_off;
     uint32_t stacksize;
 } entry_point_command_t;
 
 /* ============================================================================
    APPLE DEVICE TREE FORMAT
    ============================================================================ */
 
 typedef struct {
     uint32_t signature;          /* "DT\0\0" */
     uint32_t version;            /* Device tree version */
     uint32_t size;               /* Total size of device tree */
     uint32_t reserved_entry_ofs; /* Offset to reserved entry */
 } device_tree_header_t;
 
 typedef struct {
     uint32_t prop_off;  /* Offset to property data */
     uint32_t child_off; /* Offset to child node */
 } device_tree_node_t;
 
 typedef struct {
     uint32_t name_off;  /* Offset to property name */
     uint32_t length;    /* Property data length */
 } device_tree_property_t;
 
 /* ============================================================================
    DISPLAY FRAMEBUFFER ABSTRACTION
    ============================================================================ */
 
 typedef struct {
     uint32_t width;
     uint32_t height;
     uint32_t stride;      /* bytes per line */
     uint32_t depth;       /* bits per pixel */
     uint32_t bpp;         /* bytes per pixel */
     uint8_t  *base;       /* virtual address */
     uint64_t phys_base;   /* physical address */
 } display_info_t;
 
 static display_info_t display_info = {
     .width = DISPLAY_WIDTH,
     .height = DISPLAY_HEIGHT,
     .stride = DISPLAY_WIDTH * DISPLAY_BPP,
     .depth = DISPLAY_DEPTH,
     .bpp = DISPLAY_BPP,
     .base = (uint8_t *)FBMEM_BASE,
     .phys_base = FBMEM_BASE,
 };
 
 /* ============================================================================
    UART/SERIAL DEBUGGING
    ============================================================================ */
 
 static inline void uart_putc(char c) {
 #if TARGET_PLATFORM == PLATFORM_HARPIA
     /* MSM8916 BLSP1_UART2 register offsets */
     volatile uint32_t *uart_status = (volatile uint32_t *)(UART_BASE + 0x08);
     volatile uint32_t *uart_data = (volatile uint32_t *)(UART_BASE + 0x00);
     
     /* Wait for TX FIFO not full (bit 4 of UARTDM_SR) */
     while ((*uart_status & 0x08) == 0) {}
     
     *uart_data = c;
     
     if (c == '\n') {
         while ((*uart_status & 0x08) == 0) {}
         *uart_data = '\r';
     }
 #endif
 }
 
 static inline void debug_putc(char c) {
     uart_putc(c);
 }
 
 static void debug_print(const char *str) {
     while (*str) {
         uart_putc(*str++);
     }
 }
 
 static void debug_hex(uint64_t val, int width) {
     const char *hex = "0123456789abcdef";
     for (int i = width - 1; i >= 0; i--) {
         uart_putc(hex[(val >> (i * 4)) & 0xf]);
     }
 }
 
 static uint16_t rgb888_to_rgb565(uint32_t rgb888) {
     uint8_t r = (rgb888 >> 16) & 0xff;
     uint8_t g = (rgb888 >> 8) & 0xff;
     uint8_t b = rgb888 & 0xff;
     return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
 }
 
 static void display_clear(uint32_t color) {
     uint16_t *fb = (uint16_t *)display_info.base;
     uint16_t rgb565 = rgb888_to_rgb565(color);
     
     for (uint32_t i = 0; i < (display_info.width * display_info.height); i++) {
         fb[i] = rgb565;
     }
 }
 
 static void display_put_pixel(uint32_t x, uint32_t y, uint32_t color) {
     if (x >= display_info.width || y >= display_info.height) return;
     
     uint16_t *fb = (uint16_t *)display_info.base;
     uint16_t rgb565 = rgb888_to_rgb565(color);
     fb[y * display_info.width + x] = rgb565;
 }
 
 /* Simple 8x8 font for bootstrap display output */
 typedef struct {
     char ch;
     uint8_t bitmap[8];
 } font_glyph_t;
 
 static const font_glyph_t bootstrap_font[] = {
     {'A', {0x18, 0x3c, 0x66, 0x7e, 0x66, 0x66, 0x66, 0x00}},
     {'B', {0x7c, 0x66, 0x66, 0x7c, 0x66, 0x66, 0x7c, 0x00}},
     {'C', {0x3c, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3c, 0x00}},
     {'D', {0x78, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0x78, 0x00}},
     {'E', {0x7e, 0x60, 0x60, 0x7c, 0x60, 0x60, 0x7e, 0x00}},
     {'L', {0x60, 0x60, 0x60, 0x60, 0x60, 0x66, 0x7e, 0x00}},
     {'O', {0x3c, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00}},
     {'R', {0x7c, 0x66, 0x66, 0x7c, 0x78, 0x6c, 0x66, 0x00}},
     {'S', {0x3c, 0x66, 0x60, 0x3c, 0x06, 0x66, 0x3c, 0x00}},
     {'T', {0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00}},
     {'X', {0x66, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0x66, 0x00}},
     {'N', {0x66, 0x76, 0x7e, 0x6e, 0x66, 0x66, 0x66, 0x00}},
     {'U', {0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3c, 0x00}},
 };
 
 static void display_putchar(uint32_t x, uint32_t y, char c, uint32_t fg_color) {
     for (size_t i = 0; i < sizeof(bootstrap_font) / sizeof(bootstrap_font[0]); i++) {
         if (bootstrap_font[i].ch != c) continue;
         
         const uint8_t *bitmap = bootstrap_font[i].bitmap;
         for (int row = 0; row < 8; row++) {
             for (int col = 0; col < 8; col++) {
                 if ((bitmap[row] >> (7 - col)) & 1) {
                     display_put_pixel(x + col, y + row, fg_color);
                 }
             }
         }
         return;
     }
 }
 
 static void display_puts(uint32_t x, uint32_t y, const char *str, uint32_t fg_color) {
     while (*str) {
         display_putchar(x, y, *str++, fg_color);
         x += 10;
         if (x >= display_info.width) {
             x = 0;
             y += 12;
         }
     }
 }
 
 
 /* ============================================================================
     KEXT INJECTION
    ============================================================================*/
 
 
 /*
  * Kernel Extension (Kext) Injection System
  * 
  * This module handles loading, pre-linking, and injecting kernel extensions
  * into the XNU kernel before boot. Kexts are stored in a dedicated memory
  * region and passed to the kernel via the device tree.
  * 
  * KEXT Format:
  *   Each kext is stored as a Mach-O bundle with:
  *   - MH_BUNDLE filetype
  *   - Linked at a fixed base + offset
  * 
  * The bootloader maintains a KEXT table in the device tree's
  * "/chosen/kext-injections" node.
  */

 /* Maximum number of kexts we can inject */
 #define MAX_KEXTS 16

 /* KEXT entry structure */
 typedef struct {
     const char *identifier;     /* KEXT bundle identifier */
     uint64_t load_addr;       /* Physical load address */
     uint32_t size;             /* Size of kext binary */
     uint32_t flags;            /* KEXT flags */
     uint64_t entry_point;      /* KEXT entry point (if executable) */
 } kext_entry_t;

 /* KEXT table in memory */
 static kext_entry_t kext_table[MAX_KEXTS];
 static uint32_t kext_count = 0;

 /* KEXT memory region */
 #define KEXT_LOAD_BASE   0x84000000
 #define KEXT_MAX_SIZE    0x00400000  /* 4MB for all kexts */
 static uint64_t kext_next_addr = KEXT_LOAD_BASE;

 /* KEXT flags */
 #define KEXT_FLAG_LINKED     0x01
 #define KEXT_FLAG_EXECUTABLE 0x02
 #define KEXT_FLAG_CODESIGN  0x04

 /*
  * Register a kext for injection
  * 
  * @param identifier  KEXT bundle identifier (e.g., "com.apple.driver.AppleHDA")
  * @param data        Pointer to kext Mach-O data
  * @param size        Size of kext data
  * @return            KEXT ID on success, -1 on failure
  */
 static int kext_register(const char *identifier, const void *data, uint32_t size) {
     if (kext_count >= MAX_KEXTS) {
         debug_print("[KEXT] ERROR: Max kext count reached\n");
         return -1;
     }
     
     if (!identifier || !data || size == 0) {
         debug_print("[KEXT] ERROR: Invalid parameters\n");
         return -1;
     }
     
     /* Check for duplicate identifier */
     for (uint32_t i = 0; i < kext_count; i++) {
         if (kext_table[i].identifier && 
             strcmp(kext_table[i].identifier, identifier) == 0) {
             debug_print("[KEXT] WARNING: KEXT already registered: ");
             debug_print(identifier);
             debug_print("\n");
             return i;
         }
     }
     
     /* Allocate memory for kext */
     uint64_t load_addr = kext_next_addr;
     
     /* Align to 4KB */
     kext_next_addr = (kext_next_addr + 0xFFF) & ~0xFFF;
     
     /* Copy kext to memory */
     memcpy((void *)(uintptr_t)load_addr, data, size);
     
     /* Add to table */
     kext_entry_t *entry = &kext_table[kext_count];
     entry->identifier = identifier;
     entry->load_addr = load_addr;
     entry->size = size;
     entry->flags = KEXT_FLAG_LINKED;
     entry->entry_point = load_addr; /* Default to load address */
     
     debug_print("[KEXT] Registered: ");
     debug_print(identifier);
     debug_print(" at 0x");
     debug_hex(load_addr, 8);
     debug_print(" (");
     debug_hex(size, 4);
     debug_print(" bytes)\n");
     
     kext_count++;
     kext_next_addr += size;
     
     return kext_count - 1;
 }

 /*
  * Pre-link a kext (resolve symbolic references)
  * 
  * In a full implementation, this would:
  * 1. Parse Mach-O symbols
  * 2. Resolve external references
  * 3. Apply fixups
  * 
  * @param kext_id     KEXT ID from kext_register
  * @param base_addr   Base address for symbol resolution
  * @return            0 on success, -1 on failure
  */
 static int kext_prelink(int kext_id, uint64_t base_addr) {
     if (kext_id < 0 || (uint32_t)kext_id >= kext_count) {
         debug_print("[KEXT] ERROR: Invalid kext ID\n");
         return -1;
     }
     
     kext_entry_t *entry = &kext_table[kext_id];
     
     debug_print("[KEXT] Pre-linking: ");
     debug_print(entry->identifier);
     debug_print("\n");
     
     /* TODO: Full symbol resolution would go here */
     /* For now, assume kexts are pre-linked */
     
     entry->flags |= KEXT_FLAG_LINKED;
     
     return 0;
 }

 /*
  * Get the kext table for device tree insertion
  */
 static kext_entry_t *kext_get_table(void) {
     return kext_table;
 }

 /*
  * Get the number of registered kexts
  */
 static uint32_t kext_get_count(void) {
     return kext_count;
 }

 /*
  * Get the total size needed for all kexts
  */
 static uint64_t kext_get_total_size(void) {
     return kext_next_addr - KEXT_LOAD_BASE;
 }

 /*
  * Inject kext table into device tree
  * 
  * Creates/updates the "/chosen/kext-injections" node with
  * the kext table information.
  * 
  * @param dt_base     Device tree base address
  * @return            0 on success, -1 on failure
  */
 static int kext_inject_into_device_tree(uint8_t *dt_base) {
     if (!dt_base) return -1;
     
     device_tree_header_t *header = (device_tree_header_t *)dt_base;
     if (header->signature != 0x54440000) {
         debug_print("[KEXT] Invalid device tree\n");
         return -1;
     }
     
     debug_print("[KEXT] Injecting ");
     debug_hex(kext_count, 2);
     debug_print(" kexts into device tree\n");
     
     debug_print("[KEXT] KEXT table base: 0x");
     debug_hex((uint64_t)(uintptr_t)kext_table, 8);
     debug_print("\n");
     
     debug_print("[KEXT] Total kext memory: ");
     debug_hex(kext_get_total_size(), 8);
     debug_print(" bytes\n");
     
     /* TODO: Actually create/update DT node for kexts
      * The device tree should have:
      * /chosen/kext-injections {
      *     kext-table = <address of kext_table>;
      *     kext-count = <number of kexts>;
      *     kext-size = <total size>;
      * };
      */
     
     return 0;
 }

 /*
  * Initialize kext system
  */
 static void kext_init(void) {
     kext_count = 0;
     kext_next_addr = KEXT_LOAD_BASE;
     memset(kext_table, 0, sizeof(kext_table));
     
     debug_print("[KEXT] Kext injection system initialized\n");
     debug_print("[KEXT] KEXT region: 0x");
     debug_hex(KEXT_LOAD_BASE, 8);
     debug_print(" - 0x");
     debug_hex(KEXT_LOAD_BASE + KEXT_MAX_SIZE, 8);
     debug_print("\n");
 }

 /*
  * Embed kexts from bootloader (placeholder)
  * 
  * In a real implementation, kexts would be embedded in the
  * bootloader binary or loaded from a separate partition.
  */
 static void kext_embed(void) {
     /* Placeholder for embedded kexts */
     /* 
      * Example:
      * extern const uint8_t kext_applehda[];
      * extern const uint32_t kext_applehda_size;
      * kext_register("com.apple.driver.AppleHDA", 
      *               kext_applehda, kext_applehda_size);
      */
     
     debug_print("[KEXT] No embedded kexts\n");
 }

 /* ============================================================================
    DEVICE TREE MANIPULATION
    ============================================================================ */
 static int device_tree_set_chosen_display(uint8_t *dt_base, 
                                           uint64_t fb_phys, 
                                           uint32_t width,
                                           uint32_t height,
                                           uint32_t stride) {
     (void)fb_phys;
     (void)width;
     (void)height;
     (void)stride;
 
     if (!dt_base) return -1;
     
     device_tree_header_t *header = (device_tree_header_t *)dt_base;
     if (header->signature != 0x54440000) {  /* "DT\0\0" in little-endian */
         debug_print("[DT] Invalid device tree signature\n");
         return -1;
     }
     
     debug_print("[DT] Device tree found at 0x");
     debug_hex((uint64_t)(uintptr_t)dt_base, 8);
     debug_print(", size: ");
     debug_hex(header->size, 4);
     debug_print("\n");
     
     return 0;
 }
 
 /* ============================================================================
    MEMORY MANAGEMENT & MMU
    ============================================================================ */
 
 #if ARCH_ARM64
 
 static inline void msr(const char *reg, uint64_t value) {
     /* Macro for inline assembly MSR instructions */
     if (!strcmp(reg, "SCTLR_EL1")) {
         asm volatile("msr sctlr_el1, %0" : : "r" (value));
     } else if (!strcmp(reg, "TCR_EL1")) {
         asm volatile("msr tcr_el1, %0" : : "r" (value));
     } else if (!strcmp(reg, "TTBR0_EL1")) {
         asm volatile("msr ttbr0_el1, %0" : : "r" (value));
     } else if (!strcmp(reg, "TTBR1_EL1")) {
         asm volatile("msr ttbr1_el1, %0" : : "r" (value));
     } else if (!strcmp(reg, "MAIR_EL1")) {
         asm volatile("msr mair_el1, %0" : : "r" (value));
     }
 }
 
 static inline uint64_t mrs(const char *reg) {
     uint64_t value = 0;
     if (!strcmp(reg, "SCTLR_EL1")) {
         asm volatile("mrs %0, sctlr_el1" : "=r" (value));
     } else if (!strcmp(reg, "ID_AA64MMFR0_EL1")) {
         asm volatile("mrs %0, id_aa64mmfr0_el1" : "=r" (value));
     }
     return value;
 }
 
 static void mmu_init_arm64(void) {
     /* Configure MAIR (memory attribute indirection) */
     uint64_t mair = 0x00bb04ff;  /* Normal, device, etc. */
     msr("MAIR_EL1", mair);
     
     /* Configure TCR */
     uint64_t tcr = 0x80823519;   /* 4KB granule, 48-bit VA */
     msr("TCR_EL1", tcr);
     
     /* Enable MMU */
     uint64_t sctlr = mrs("SCTLR_EL1");
     sctlr |= 1;  /* Enable bit */
     msr("SCTLR_EL1", sctlr);
     
     asm volatile("dsb sy; isb");
 }
 
 #elif ARCH_ARM
 
 static inline void mcr(uint32_t op1, uint32_t op2, uint32_t reg, uint32_t value) {
     /* ARM32 coprocessor write */
     asm volatile("mcr p15, %0, %1, c1, c%2, %3" : : "n" (op1), "r" (value), "i" (reg), "i" (op2));
 }
 
 static inline uint32_t mrc(uint32_t op1, uint32_t op2, uint32_t reg) {
     uint32_t value;
     asm volatile("mrc p15, %0, %1, c1, c%2, %3" : "=r" (value) : "n" (op1), "i" (reg), "i" (op2));
     return value;
 }
 
 static void mmu_init_arm32(void) {
     /* Set up TTBR0 for low addresses, identity mapped */
     uint32_t ttbr = KERNEL_LOAD_BASE | 0x4a;  /* IRGN=1, RGN=1, S=1 */
     mcr(0, 0, 2, ttbr);
     
     /* Configure DACR */
     mcr(0, 0, 3, 0x55555555);
     
     /* Enable LPAE if supported */
     uint32_t id_mmfr0 = mrc(0, 0, 1);
     if ((id_mmfr0 >> 28) & 0xf) {
         uint32_t sctlr = mrc(0, 0, 1);
         sctlr |= (1 << 31);  /* LPAE enable */
         mcr(0, 0, 1, sctlr);
     }
     
     /* TLB invalidation */
     asm volatile("mcr p15, 0, %0, c8, c7, 0" : : "r" (0));
     asm volatile("dsb sy; isb");
 }
 
 #endif
 
 static void mmu_init(void) {
 #if ARCH_ARM64
     mmu_init_arm64();
 #else
     mmu_init_arm32();
 #endif
     debug_print("[MMU] Memory management unit initialized\n");
 }
 
 /* ============================================================================
    MACH-O KERNEL LOADER
    ============================================================================ */
 
 typedef struct {
     uint64_t entry_point;
     uint64_t load_base;
     uint32_t is_64bit;
     uint32_t is_valid;
 } kernel_info_t;
 
 static kernel_info_t macho_load(uint8_t *image_base, size_t image_size) {
     kernel_info_t info = {0};
     
     if (image_size < sizeof(mach_header_64_t)) {
         debug_print("[MACHO] Image too small\n");
         return info;
     }
     
     mach_header_64_t *header = (mach_header_64_t *)image_base;
     
     /* Detect architecture and endianness */
     uint32_t is_64bit = 0;
     
     if (header->magic == MH_MAGIC_64) {
         is_64bit = 1;
     } else if (header->magic == MH_MAGIC) {
         is_64bit = 0;
     } else if (header->magic == MH_CIGAM_64) {
         is_64bit = 1;
     } else if (header->magic == MH_CIGAM) {
         is_64bit = 0;
     } else {
         debug_print("[MACH] Invalid magic: 0x");
         debug_hex(header->magic, 8);
         debug_print("\n");
         return info;
     }
     
     debug_print("[MACH] Found ");
     if (is_64bit) {
         debug_print("64-bit");
     } else {
         debug_print("32-bit");
     }
     debug_print(" Mach-O, CPU type: 0x");
     debug_hex(header->cputype, 4);
     debug_print("\n");
     
     /* Process load commands */
     load_command_t *cmd = (load_command_t *)(image_base + (is_64bit ? 32 : 28));
     uint64_t min_vaddr = ~0ull, max_vaddr = 0;
     
     for (uint32_t i = 0; i < header->ncmds; i++) {
         if (is_64bit && cmd->cmd == LC_SEGMENT_64) {
             segment_command_64_t *seg = (segment_command_64_t *)cmd;
             
             debug_print("[MACH] LC_SEGMENT_64: ");
             for (int j = 0; j < 16; j++) {
                 if (seg->segname[j]) debug_putc(seg->segname[j]);
             }
             debug_print(" -> 0x");
             debug_hex(seg->vmaddr, 16);
             debug_print(" size: 0x");
             debug_hex(seg->vmsize, 8);
             debug_print("\n");
             
             if (seg->vmsize > 0) {
                 if (seg->vmaddr < min_vaddr) min_vaddr = seg->vmaddr;
                 if (seg->vmaddr + seg->vmsize > max_vaddr) {
                     max_vaddr = seg->vmaddr + seg->vmsize;
                 }
             }
             
             /* Load segment data */
             if (seg->filesize > 0) {
                 uint8_t *dest = (uint8_t *)(uintptr_t)seg->vmaddr;
                 uint8_t *src = image_base + seg->fileoff;
                 memcpy(dest, src, seg->filesize);
                 
                 /* Zero out BSS */
                 if (seg->vmsize > seg->filesize) {
                     memset(dest + seg->filesize, 0, seg->vmsize - seg->filesize);
                 }
             }
             
         } else if (!is_64bit && cmd->cmd == LC_SEGMENT) {
             segment_command_32_t *seg = (segment_command_32_t *)cmd;
             
             debug_print("[MACH] LC_SEGMENT: ");
             for (int j = 0; j < 16; j++) {
                 if (seg->segname[j]) uart_putc(seg->segname[j]);
             }
             debug_print(" -> 0x");
             debug_hex(seg->vmaddr, 8);
             debug_print("\n");
             
             if (seg->filesize > 0) {
                 uint8_t *dest = (uint8_t *)(uintptr_t)seg->vmaddr;
                 uint8_t *src = image_base + seg->fileoff;
                 memcpy(dest, src, seg->filesize);
                 
                 if (seg->vmsize > seg->filesize) {
                     memset(dest + seg->filesize, 0, seg->vmsize - seg->filesize);
                 }
             }
             
         } else if (cmd->cmd == LC_MAIN || cmd->cmd == LC_UNIXTHREAD) {
             if (cmd->cmd == LC_MAIN) {
                 entry_point_command_t *ep = (entry_point_command_t *)cmd;
                 info.entry_point = (uint64_t)((uintptr_t)image_base + ep->entry_off);
                 debug_print("[MACH] Entry point (LC_MAIN): 0x");
                 debug_hex(info.entry_point, 16);
                 debug_print("\n");
             }
         }
         
         cmd = (load_command_t *)((uint8_t *)cmd + cmd->cmdsize);
     }
     
     info.load_base = min_vaddr;
     info.is_64bit = is_64bit;
     info.is_valid = 1;
     
     debug_print("[MACH] Load base: 0x");
     debug_hex(info.load_base, 16);
     debug_print(", Kernel size: 0x");
     debug_hex(max_vaddr - min_vaddr, 16);
     debug_print("\n");
     
     return info;
 }
 
 /* ============================================================================
    BOOTLOADER INITIALIZATION STUBS
    ============================================================================ */
 
 /* Device/board-specific initialization structure */
 typedef struct {
     const char *name;
     uint32_t platform_id;
     
     /* Setup functions */
     void (*uart_init)(void);
     void (*display_init)(void);
     void (*mmu_init_custom)(void);
     void (*clock_init)(void);
     void (*power_init)(void);
     
 } board_config_t;
 
 /* Forward declarations for board configs */
 static void harpia_uart_init(void);
 static void harpia_display_init(void);
 static void harpia_mmu_init(void);
 static void harpia_clock_init(void);
 static void harpia_power_init(void);
 
 static void qemu_arm64_uart_init(void);
 static void qemu_arm64_display_init(void);
 static void qemu_arm64_clock_init(void);
 static void qemu_arm64_power_init(void);
 
 /* Board configuration table */
 static const board_config_t board_configs[] = {
     {
         .name = "Motorola Harpia (XT1622/MSM8916)",
         .platform_id = PLATFORM_HARPIA,
         .uart_init = harpia_uart_init,
         .display_init = harpia_display_init,
         .mmu_init_custom = harpia_mmu_init,
         .clock_init = harpia_clock_init,
         .power_init = harpia_power_init,
     },
     {
         .name = "QEMU ARM64 Generic",
         .platform_id = PLATFORM_QEMU_ARM64,
         .uart_init = qemu_arm64_uart_init,
         .display_init = qemu_arm64_display_init,
         .clock_init = qemu_arm64_clock_init,
         .power_init = qemu_arm64_power_init,
     },
 };
 
 #define NUM_BOARDS (sizeof(board_configs) / sizeof(board_configs[0]))
 
 /* ============================================================================
    MOTOROLA HARPIA (MSM8916) BOARD INITIALIZATION
    ============================================================================ */
 
 /* MSM8916 register definitions */
 #define HARPIA_CLK_CTL_BASE     0x1800000
 #define HARPIA_GPIO_BASE        0x1000000
 #define HARPIA_TLMM_BASE        0xFD510000
 
 static void harpia_uart_init(void) {
     debug_print("[HARPIA] UART2 (BLSP1) initialized at 0x");
     debug_hex(UART_BASE, 8);
     debug_print("\n");
 }
 
 static void harpia_display_init(void) {
     debug_print("[HARPIA] Display initialized: ");
     debug_hex(DISPLAY_WIDTH, 3);
     debug_print("x");
     debug_hex(DISPLAY_HEIGHT, 3);
     debug_print(" RGB565 at 0x");
     debug_hex(FBMEM_BASE, 8);
     debug_print("\n");
     
     /* Initialize ILI9341 DSI display controller */
     /* Stub: actual DSI/panel driver would go here */
     display_clear(0x000000);
 }
 
 static void harpia_mmu_init(void) {
     debug_print("[HARPIA] Custom MMU setup for LPAE (Cortex-A53)\n");
     mmu_init();  /* Use standard ARM32 LPAE setup */
 }
 
 static void harpia_clock_init(void) {
     debug_print("[HARPIA] Clock initialization for MSM8916\n");
     /* Stub: MSM8916 clock control setup */
 }
 
 static void harpia_power_init(void) {
     debug_print("[HARPIA] Power domain initialization\n");
     /* Stub: power domain setup */
 }
 
 /* ============================================================================
    QEMU ARM64 BOARD INITIALIZATION
    ============================================================================ */
 
 static void qemu_arm64_uart_init(void) {
     debug_print("[QEMU-ARM64] PL011 UART initialized at 0x");
     debug_hex(UART_BASE, 8);
     debug_print("\n");
 }
 
 static void qemu_arm64_display_init(void) {
     debug_print("[QEMU-ARM64] Display initialized: ");
     debug_hex(DISPLAY_WIDTH, 4);
     debug_print("x");
     debug_hex(DISPLAY_HEIGHT, 4);
     debug_print(" ARGB8888 at 0x");
     debug_hex(FBMEM_BASE, 8);
     debug_print("\n");
     
     display_clear(0x1e1e1e);
 }
 
 static void qemu_arm64_clock_init(void) {
     debug_print("[QEMU-ARM64] Clock initialization\n");
 }
 
 static void qemu_arm64_power_init(void) {
     debug_print("[QEMU-ARM64] Power initialization\n");
 }
 
 /* ============================================================================
    DEVICE TREE EMBEDDING & EXTRACTION
    ============================================================================ */
 
 /* Symbol references to embedded device tree (provided by linker script) */
 extern uint8_t _device_tree_start;
 extern uint8_t _device_tree_end;
 
 static void embed_device_tree(void) {
     size_t dt_size = (size_t)(&_device_tree_end - &_device_tree_start);
     
     if (dt_size == 0) {
         debug_print("[DT] No embedded device tree\n");
         return;
     }
     
     if (dt_size > MAX_DEVICETREE_SIZE) {
         debug_print("[DT] Device tree too large: 0x");
         debug_hex(dt_size, 8);
         debug_print("\n");
         return;
     }
     
     memcpy((void *)DEVICE_TREE_BASE, &_device_tree_start, dt_size);
     debug_print("[DT] Embedded device tree copied: 0x");
     debug_hex(dt_size, 8);
     debug_print(" bytes at 0x");
     debug_hex(DEVICE_TREE_BASE, 8);
     debug_print("\n");
     
     /* Update device tree with runtime display info */
     device_tree_set_chosen_display(
         (uint8_t *)DEVICE_TREE_BASE,
         display_info.phys_base,
         display_info.width,
         display_info.height,
         display_info.stride
     );
 }
 
 /* ============================================================================
    KERNEL BOOT PARAMETERS (arm64 boot protocol)
    ============================================================================ */
 
 typedef struct {
     uint64_t kernel_size;
     uint64_t kernel_offset;
     uint64_t image_size;
     uint64_t image_offset;
     uint64_t flags;
     uint64_t res2;
     uint64_t res3;
     uint64_t res4;
     uint32_t magic;
     uint32_t version;
 } arm64_image_header_t;
 
 #define ARM64_MAGIC 0x644d5241  /* "ARM\x64" */
 
 /* ============================================================================
    BOOTLOADER ENTRY POINT
    ============================================================================ */
 
 void bootloader_main(void) {
     // Early initialization
     display_clear(0x000000);
     display_puts(10, 10, "XNU BOOT", 0xffffff);
     
     debug_print("\n");
     debug_print("=====================================\n");
     debug_print("  XNU ARM/ARM64 Bootloader v1.0\n");
     debug_print("  Target: ");
     debug_print(board_configs[TARGET_PLATFORM].name);
     debug_print("\n");
     debug_print("=====================================\n\n");
     
     /* Board detection and initialization */
     const board_config_t *board = &board_configs[TARGET_PLATFORM];
     
     debug_print("[BOOT] Initializing board: ");
     debug_print(board->name);
     debug_print("\n");
     
     if (board->uart_init) board->uart_init();
     if (board->clock_init) board->clock_init();
     if (board->power_init) board->power_init();
     if (board->mmu_init_custom) board->mmu_init_custom();
     
     debug_print("[BOOT] Memory layout:\n");
     debug_print("[BOOT]   Bootloader: 0x");
     debug_hex(BOOTLOADER_BASE, 8);
     debug_print("\n");
     debug_print("[BOOT]   Kernel:     0x");
     debug_hex(KERNEL_LOAD_BASE, 8);
     debug_print("\n");
     debug_print("[BOOT]   DeviceTree: 0x");
     debug_hex(DEVICE_TREE_BASE, 8);
     debug_print("\n");
     debug_print("[BOOT]   Framebuffer: 0x");
     debug_hex(FBMEM_BASE, 8);
     debug_print(" (");
     debug_hex(FBMEM_SIZE, 8);
     debug_print(" bytes)\n");
     
     /* Display initialization */
     if (board->display_init) board->display_init();
     display_puts(10, 50, "INIT DONE", 0x00ff00);
     
     /* Embed device tree */
     embed_device_tree();
     
     /* Load kernel from embedded location or external source */
     debug_print("[BOOT] Loading kernel...\n");
     
     /* In a real implementation, the kernel would be loaded from:
      * - NAND/eMMC storage
      * - Network (tftp)
      * - RAM disk embedded in bootloader
      * For this stub, assume kernel is at KERNEL_LOAD_BASE
      */
     
     uint8_t *kernel_image = (uint8_t *)KERNEL_LOAD_BASE;
     kernel_info_t kernel = macho_load(kernel_image, MAX_KERNEL_SIZE);
     
     if (!kernel.is_valid) {
         debug_print("[BOOT] Failed to load kernel\n");
         display_puts(10, 90, "KERN FAIL", 0xff0000);
         
         /* Halt on failure */
         while (1) {
             asm volatile("wfi");
         }
     }
     
     debug_print("[BOOT] Kernel loaded successfully\n");
     display_puts(10, 90, "KERN OK", 0x00ff00);
     
     /* Additional XNU-specific setup */
     debug_print("[BOOT] Setting up XNU environment...\n");
     
     /* Prepare boot arguments for XNU */
     /* XNU expects certain information in registers or memory */
     
     debug_print("[BOOT] Jumping to kernel entry point: 0x");
     debug_hex(kernel.entry_point, 16);
     debug_print("\n");
     
     display_puts(10, 130, "JUMP", 0xffff00);
     
     /* Jump to kernel entry point */
 #if ARCH_ARM64
     /* ARM64 kernel entry - x0=device tree, x1=unknown, x2=unknown, x3=unknown */
     typedef void (*kernel_entry_arm64_t)(uint64_t, uint64_t, uint64_t, uint64_t);
     kernel_entry_arm64_t entry = (kernel_entry_arm64_t)kernel.entry_point;
     entry((uint64_t)DEVICE_TREE_BASE, 0, 0, 0);
 #else
     /* ARM32 kernel entry - r0=device tree, r1=machine type, r2=atags/fdt */
     typedef void (*kernel_entry_arm32_t)(uint32_t, uint32_t, uint32_t);
     kernel_entry_arm32_t entry = (kernel_entry_arm32_t)(uintptr_t)kernel.entry_point;
     entry((uint32_t)DEVICE_TREE_BASE, 0, (uint32_t)DEVICE_TREE_BASE);
 #endif
     
     /* Should never reach here */
     debug_print("[BOOT] Kernel returned!\n");
     while (1) {
         asm volatile("wfi");
     }
 }
 
 /* ============================================================================
    ASSEMBLY STARTUP CODE (Platform-specific)
    ============================================================================ */
 
 #if ARCH_ARM64
 
 asm(
     ".section .text.boot\n"
     ".align 12\n"
     ".globl _start\n"
     ".globl start\n"
     "_start:\n"
     "start:\n"
     /* Set up stack */
     "adrp x0, __stack_top\n"
     "add x0, x0, #0\n"
     "mov sp, x0\n"
     
     /* Clear BSS */
     "adrp x0, __bss_start\n"
     "add x0, x0, #0\n"
     "adrp x1, __bss_end\n"
     "add x1, x1, #0\n"
     "cmp x0, x1\n"
     "b.ge 2f\n"
     "1:\n"
     "str xzr, [x0], #8\n"
     "cmp x0, x1\n"
     "b.lt 1b\n"
     "2:\n"
     
     /* Disable MMU and caches */
     "mrs x0, sctlr_el1\n"
     "bic x0, x0, #1\n"
     "msr sctlr_el1, x0\n"
     "dsb sy\n"
     "isb\n"
     
     /* Call bootloader main */
     "bl bootloader_main\n"
     
     /* Halt */
     "1:\n"
     "wfi\n"
     "b 1b\n"
 );
 
 #elif ARCH_ARM
 
 asm(
     ".section .text.boot\n"
     ".align 14\n"
     ".globl _start\n"
     ".globl start\n"
     "_start:\n"
     "start:\n"
     /* Set up stack */
     "ldr sp, =__stack_top\n"
     
     /* Disable MMU */
     "mrc p15, 0, r0, c1, c0, 0\n"
     "bic r0, r0, #1\n"
     "mcr p15, 0, r0, c1, c0, 0\n"
     "dsb\n"
     "isb\n"
     
     /* Call bootloader main */
     "bl bootloader_main\n"
     
     /* Halt */
     "1:\n"
     "wfi\n"
     "b 1b\n"
 );
 
 #endif
 
 /* ============================================================================
    STUB: ADDITIONAL BOARD DEFINITIONS
    ============================================================================ */
 
    // NOTE: For now, only Qualcomm, Raspberry Pi, and Apple boards/devices are supported!!
 
 /* These are placeholders for adding support to other devices/boards.
  * To add a new platform:
  * 
  * 1. Create board-specific definitions:
  *    #define NEW_PLATFORM_BASE    0x...
  *    #define NEW_DISPLAY_WIDTH    ...
  *    etc.
  * 
  * 2. Add board config to memory layout conditionals:
  *    #elif TARGET_PLATFORM == PLATFORM_NEW_DEVICE
  * 
  * 3. Implement board functions:
  *    static void new_device_uart_init(void)
  *    static void new_device_display_init(void)
  *    static void new_device_clock_init(void)
  *    static void new_device_power_init(void)
  * 
  * 4. Add entry to board_configs array
  * 
  * 5. Compile with: -DTARGET_PLATFORM=PLATFORM_NEW_DEVICE
  */
 
 /* STUB: Snapdragon MSM8974 (OnePlus One, Nexus 5) */
 /*
 #define PLATFORM_ONEPLUS_ONE  4
 static void oneplus_one_uart_init(void) { }
 static void oneplus_one_display_init(void) { }
 static void oneplus_one_clock_init(void) { }
 static void oneplus_one_power_init(void) { }
 */
 
 /* STUB: Snapdragon MSM8960 (Galaxy Note 2) */
 /*
 #define PLATFORM_GALAXY_NOTE2  5
 static void galaxy_note2_uart_init(void) { }
 static void galaxy_note2_display_init(void) { }
 static void galaxy_note2_clock_init(void) { }
 static void galaxy_note2_power_init(void) { }
 */
 
 /* STUB: Apple A7 (iPad Air 1) - if supporting native iOS devices */
 /*
 #define PLATFORM_IPAD_AIR1  6
 static void ipad_air1_uart_init(void) { }
 static void ipad_air1_display_init(void) { }
 static void ipad_air1_clock_init(void) { }
 static void ipad_air1_power_init(void) { }
 */
 
 #endif /* SINGLE_FILE_BOOTLOADER_H */