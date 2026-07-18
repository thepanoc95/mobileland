/* ============================================================================
   LOADER.PLIST KEXT CONFIGURATION (OpenCore-style)
   ============================================================================ */

/*
 * loader.plist is an XML configuration file that specifies which Kexts
 * to load during boot, similar to OpenCore's Kext NSBundles.
 *
 * Example loader.plist format:
 * <?xml version="1.0" encoding="UTF-8"?>
 * <!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "...">
 * <plist version="1.0">
 * <dict>
 *     <key>Kexts</key>
 *     <array>
 *         <dict>
 *             <key>Enabled</key>
 *             <true/>
 *             <key>Path</key>
 *             <string>Library/Extensions/AppleHDA.kext</string>
 *         </dict>
 *     </array>
 * </dict>
 * </plist>
 */

/* Kext entry from loader.plist */
typedef struct {
    const char *path;
    const char *plist_path;
    const char *arch;
    const char *min_kernel;
    const char *max_kernel;
    uint8_t enabled;
} plist_kext_entry_t;

#define PLIST_KEXT_MAX 32
static plist_kext_entry_t plist_kext_list[PLIST_KEXT_MAX];
static uint32_t plist_kext_count = 0;

#define LOADER_PLIST_BASE   0x85000000
#define LOADER_PLIST_SIZE   0x00100000

typedef enum {
    PLIST_STATE_IDLE,
    PLIST_STATE_READING_KEY,
} plist_state_t;

typedef struct {
    const char *data;
    size_t pos;
    size_t len;
    plist_state_t state;
    char current_key[128];
    char current_string[256];
    int in_kext_array;
    int in_kext_dict;
    plist_kext_entry_t *current_kext;
} plist_parser_t;

typedef struct {
    uint32_t major;
    uint32_t minor;
    uint32_t patch;
} kernel_version_t;

static int parse_kernel_version(const char *str, kernel_version_t *ver) {
    if (!str || !ver) return -1;
    ver->major = ver->minor = ver->patch = 0;
    const char *p = str;
    char num_buf[16];
    int num_idx = 0, part = 0;
    while (*p && part < 3) {
        if (*p >= '0' && *p <= '9') {
            num_buf[num_idx++] = *p;
        } else if (*p == '.' || *p == '\0') {
            num_buf[num_idx] = '\0';
            if (part == 0) ver->major = atoi(num_buf);
            else if (part == 1) ver->minor = atoi(num_buf);
            else ver->patch = atoi(num_buf);
            part++; num_idx = 0;
        }
        p++;
    }
    return 0;
}

static int compare_kernel_version(kernel_version_t *v1, kernel_version_t *v2) {
    if (v1->major != v2->major) return (v1->major > v2->major) ? 1 : -1;
    if (v1->minor != v2->minor) return (v1->minor > v2->minor) ? 1 : -1;
    if (v1->patch != v2->patch) return (v1->patch > v2->patch) ? 1 : -1;
    return 0;
}

static int check_kernel_version_filter(const char *min_ver, const char *max_ver) {
    kernel_version_t current = {18, 0, 0};
    kernel_version_t min, max;
    int result = 1;
    if (min_ver && parse_kernel_version(min_ver, &min) == 0) {
        if (compare_kernel_version(&current, &min) < 0) result = 0;
    }
    if (result && max_ver && parse_kernel_version(max_ver, &max) == 0) {
        if (compare_kernel_version(&current, &max) > 0) result = 0;
    }
    return result;
}

static int check_arch_filter(const char *arch_filter) {
    if (!arch_filter) return 1;
    if (strcmp(arch_filter, "Any") == 0) return 1;
#if ARCH_ARM64
    if (strcmp(arch_filter, "ARM64") == 0) return 1;
#else
    if (strcmp(arch_filter, "ARM32") == 0) return 1;
#endif
    return 0;
}

static void plist_skip_ws(plist_parser_t *p) {
    while (p->pos < p->len && (p->data[p->pos] == ' ' ||
                               p->data[p->pos] == '\t' ||
                               p->data[p->pos] == '\n' ||
                               p->data[p->pos] == '\r')) p->pos++;
}

static int plist_read_tag(plist_parser_t *p, char *tag_buf, size_t buf_size) {
    plist_skip_ws(p);
    if (p->pos >= p->len || p->data[p->pos] != '<') return -1;
    p->pos++;
    size_t idx = 0;
    while (p->pos < p->len && idx < buf_size - 1) {
        char c = p->data[p->pos];
        if (c == '>') { tag_buf[idx] = '\0'; p->pos++; return 0; }
        tag_buf[idx++] = c;
        p->pos++;
    }
    return -1;
}

static int plist_is_close_tag(const char *tag, const char *name) {
    return (tag[0] == '/' && strcmp(tag + 1, name) == 0);
}

static void plist_get_tag_name(const char *tag, char *name_buf) {
    size_t i = 0;
    while (tag[i] && tag[i] != ' ' && tag[i] != '>') { name_buf[i] = tag[i]; i++; }
    name_buf[i] = '\0';
}

static uint32_t plist_parse_kexts(const char *plist_data, size_t plist_size) {
    if (!plist_data || plist_size == 0) { debug_print("[PLIST] ERROR: No plist data\n"); return 0; }
    plist_parser_t parser = {0};
    parser.data = plist_data; parser.pos = 0; parser.len = plist_size;
    plist_kext_count = 0;
    char tag_buf[128]; char tag_name[32];
    debug_print("[PLIST] Parsing loader.plist ("); debug_hex(plist_size, 6); debug_print(" bytes)\n");
    while (parser.pos < parser.len && plist_kext_count < PLIST_KEXT_MAX) {
        if (plist_read_tag(&parser, tag_buf, sizeof(tag_buf)) != 0) break;
        plist_get_tag_name(tag_buf, tag_name);
        if (!plist_is_close_tag(tag_buf, tag_name)) {
            if (strcmp(tag_name, "dict") == 0) {
                if (parser.in_kext_array) {
                    parser.in_kext_dict = 1;
                    if (plist_kext_count < PLIST_KEXT_MAX) {
                        parser.current_kext = &plist_kext_list[plist_kext_count];
                        memset(parser.current_kext, 0, sizeof(plist_kext_entry_t));
                        parser.current_kext->enabled = 1;
                    }
                }
            } else if (strcmp(tag_name, "array") == 0) {
                if (strcmp(parser.current_key, "Kexts") == 0) parser.in_kext_array = 1;
            } else if (strcmp(tag_name, "key") == 0) {
                size_t start = parser.pos;
                while (parser.pos < parser.len && strncmp(parser.data + parser.pos, "</key>", 6) != 0) parser.pos++;
                size_t key_len = parser.pos - start;
                if (key_len < sizeof(parser.current_key)) {
                    memcpy(parser.current_key, parser.data + start, key_len);
                    parser.current_key[key_len] = '\0';
                }
                parser.pos += 6;
            } else if (strcmp(tag_name, "string") == 0) {
                size_t start = parser.pos;
                while (parser.pos < parser.len && strncmp(parser.data + parser.pos, "</string>", 9) != 0) parser.pos++;
                size_t str_len = parser.pos - start;
                if (str_len < sizeof(parser.current_string)) {
                    memcpy(parser.current_string, parser.data + start, str_len);
                    parser.current_string[str_len] = '\0';
                    if (parser.current_kext) {
                        if (strcmp(parser.current_key, "Path") == 0) parser.current_kext->path = parser.current_string;
                        else if (strcmp(parser.current_key, "Plist") == 0) parser.current_kext->plist_path = parser.current_string;
                        else if (strcmp(parser.current_key, "Arch") == 0) parser.current_kext->arch = parser.current_string;
                        else if (strcmp(parser.current_key, "MinKernel") == 0) parser.current_kext->min_kernel = parser.current_string;
                        else if (strcmp(parser.current_key, "MaxKernel") == 0) parser.current_kext->max_kernel = parser.current_string;
                    }
                }
                parser.pos += 9;
            } else if (strcmp(tag_name, "true") == 0 || strcmp(tag_name, "true/") == 0) {
                if (parser.current_kext && strcmp(parser.current_key, "Enabled") == 0) parser.current_kext->enabled = 1;
            } else if (strcmp(tag_name, "false") == 0 || strcmp(tag_name, "false/") == 0) {
                if (parser.current_kext && strcmp(parser.current_key, "Enabled") == 0) parser.current_kext->enabled = 0;
            }
        } else {
            if (strcmp(tag_name, "dict") == 0) {
                if (parser.in_kext_dict) {
                    parser.in_kext_dict = 0;
                    if (parser.current_kext && parser.current_kext->path) {
                        plist_kext_count++;
                        debug_print("[PLIST] Found: "); debug_print(parser.current_kext->path); debug_print("\n");
                    }
                    parser.current_kext = NULL;
                }
            } else if (strcmp(tag_name, "array") == 0) {
                if (parser.in_kext_array) parser.in_kext_array = 0;
            }
        }
    }
    debug_print("[PLIST] Parsed "); debug_hex(plist_kext_count, 2); debug_print(" Kexts\n");
    return plist_kext_count;
}

static void loader_plist_load(void) {
    plist_kext_count = 0;
    const char *plist_data = (const char *)LOADER_PLIST_BASE;
    if (strncmp(plist_data, "<?xml", 5) != 0) {
        debug_print("[PLIST] No loader.plist at 0x"); debug_hex(LOADER_PLIST_BASE, 8); debug_print("\n");
        return;
    }
    const char *plist_end = plist_data;
    while (*plist_end != '\0' && plist_end < (const char *)(LOADER_PLIST_BASE + LOADER_PLIST_SIZE)) plist_end++;
    size_t plist_size = plist_end - plist_data;
    if (plist_size > LOADER_PLIST_SIZE) plist_size = LOADER_PLIST_SIZE;
    plist_parse_kexts(plist_data, plist_size);
}

static void loader_plist_process_kexts(void) {
    if (plist_kext_count == 0) { debug_print("[PLIST] No Kexts to process\n"); return; }
    debug_print("[PLIST] Processing "); debug_hex(plist_kext_count, 2); debug_print(" Kexts\n");
    uint32_t loaded = 0, skipped = 0;
    for (uint32_t i = 0; i < plist_kext_count; i++) {
        plist_kext_entry_t *kext = &plist_kext_list[i];
        if (!kext->enabled) { debug_print("[PLIST] Skipping (disabled): "); debug_print(kext->path); debug_print("\n"); skipped++; continue; }
        if (!check_arch_filter(kext->arch)) { debug_print("[PLIST] Skipping (arch): "); debug_print(kext->path); debug_print("\n"); skipped++; continue; }
        if (!check_kernel_version_filter(kext->min_kernel, kext->max_kernel)) { debug_print("[PLIST] Skipping (kernel): "); debug_print(kext->path); debug_print("\n"); skipped++; continue; }
        debug_print("[PLIST] Loading: "); debug_print(kext->path); debug_print("\n");
        /* TODO: Load actual kext binary from partition */
        loaded++;
    }
    debug_print("[PLIST] Loaded "); debug_hex(loaded, 2); debug_print(", skipped "); debug_hex(skipped, 2); debug_print("\n");
}

static void loader_plist_init(void) {
    plist_kext_count = 0;
    debug_print("[PLIST] Loader.plist system initialized\n");
    debug_print("[PLIST] Plist region: 0x"); debug_hex(LOADER_PLIST_BASE, 8); debug_print("\n");
    loader_plist_load();
    loader_plist_process_kexts();
}
