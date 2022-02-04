/****************************************************************************
 * Copyright (C) 2018-2019 Maschell
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/

#pragma once

#include "dynamic_linking_defines.h"
#include "export_defines.h"
#include "relocation_defines.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAXIMUM_MODULE_PATH_NAME_LENGTH   256
#define MAXIMUM_EXPORT_MODULE_NAME_LENGTH 51

#define FUNCTION_SYMBOL_LIST_LENGTH       50000
#define DYN_LINK_RELOCATION_LIST_LENGTH   500
#define EXPORT_ENTRY_LIST_LENGTH          100
#define HOOK_ENTRY_LIST_LENGTH            20

typedef struct hook_data_t {
    uint32_t type;
    uint32_t target;
} hook_data_t;

typedef struct module_function_symbol_data_t {
    char *name;
    void *address;
    uint32_t size;
} module_function_symbol_data_t;

// clang-format off
typedef struct module_information_single_t {
    char                            path[MAXIMUM_MODULE_PATH_NAME_LENGTH];                  // Path where the module is stored
    dyn_linking_relocation_entry_t  linking_entries[DYN_LINK_RELOCATION_LIST_LENGTH];
    char                            module_export_name[MAXIMUM_EXPORT_MODULE_NAME_LENGTH];
    export_data_t                   export_entries[EXPORT_ENTRY_LIST_LENGTH];
    hook_data_t                     hook_entries[HOOK_ENTRY_LIST_LENGTH];
    int32_t                         priority;                                               // Priority of this module
    uint32_t                        bssAddr;
    uint32_t                        bssSize;
    uint32_t                        sbssAddr;
    uint32_t                        sbssSize;
    uint32_t                        entrypoint;
    uint32_t                        startAddress;
    uint32_t                        endAddress;
    uint8_t                         skipInitFini;
    uint8_t                         initBeforeRelocationDoneHook;
    module_function_symbol_data_t * function_symbol_entries;
    uint32_t                        number_used_function_symbols;
} module_information_single_t;
// clang-format on

#define MAXIMUM_MODULES            32
#define MODULE_INFORMATION_VERSION 0x00000007

// clang-format off
typedef struct module_information_t {
    uint32_t                        version;
    int32_t                         number_used_modules;                                    // Number of used function. Maximum is MAXIMUM_MODULES
    dyn_linking_relocation_data_t   linking_data;
    relocation_trampoline_entry_t   trampolines[DYN_LINK_TRAMPOLINE_LIST_LENGTH];
    module_function_symbol_data_t   function_symbols[FUNCTION_SYMBOL_LIST_LENGTH];
    uint32_t                        number_used_function_symbols;
    module_information_single_t     module_data[MAXIMUM_MODULES];
} module_information_t;
// clang-format on

#ifdef __cplusplus
}
#endif
