#pragma once

#include <stdint.h>

typedef enum RelocationTrampolineStatus {
    RELOC_TRAMP_FREE               = 0,
    RELOC_TRAMP_FIXED              = 1,
    RELOC_TRAMP_IMPORT_IN_PROGRESS = 2,
    RELOC_TRAMP_IMPORT_DONE        = 3,
} RelocationTrampolineStatus;

typedef enum RelocationType {
    RELOC_TYPE_FIXED  = 0,
    RELOC_TYPE_IMPORT = 1
} RelocationType;

typedef struct relocation_trampoline_entry_t {
    uint32_t id;
    uint32_t trampoline[4];
    RelocationTrampolineStatus status;
} relocation_trampoline_entry_t;
