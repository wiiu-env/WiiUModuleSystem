/****************************************************************************
* Copyright (C) 2022 Maschell
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

// clang-format off
typedef struct hook_data_t {
    uint32_t type;
    uint32_t target;
} hook_data_t;

typedef struct module_function_symbol_data_t {
    const char *    name;
    void *          address;
    uint32_t        size;
} module_function_symbol_data_t;

typedef struct module_information_single_t {
    dyn_linking_relocation_entry_t *    linking_entries;
    uint32_t                            number_linking_entries;
    const char *                        module_export_name;
    export_data_t *                     export_entries;
    uint32_t                            number_export_entries;
    hook_data_t *                       hook_entries;
    uint32_t                            number_hook_entries;
    uint32_t                            bssAddr;
    uint32_t                            bssSize;
    uint32_t                            sbssAddr;
    uint32_t                            sbssSize;
    uint32_t                            startAddress;
    uint32_t                            endAddress;
    uint32_t                            entrypoint;
    uint8_t                             skipInitFini;
    uint8_t                             initBeforeRelocationDoneHook;
    module_function_symbol_data_t *     function_symbol_entries;
    uint32_t                            number_function_symbols;
} module_information_single_t;

#define MODULE_INFORMATION_VERSION      0x0000000E
#define MODULE_LITE_INFORMATION_VERSION 0x80000001

typedef struct module_information_t {
    uint32_t                        version;
    uint32_t                        number_modules;
    module_information_single_t *   modules;
    uint32_t                        number_acquired_rpls;
    uint32_t *                      acquired_rpls;
} module_information_t;

typedef struct module_lite_information_t {
    uint32_t                        version;
} module_lite_information_t;

// clang-format on

#ifdef __cplusplus
}
#endif
