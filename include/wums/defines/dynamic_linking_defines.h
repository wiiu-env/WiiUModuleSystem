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

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// clang-format off
typedef struct _dyn_linking_import_t {
    const char *    importName;
    bool            isData;
} dyn_linking_import_t;

typedef struct _dyn_linking_relocation_entry_t {
    const char *            functionName;
    dyn_linking_import_t *  importEntry;
    void *                  destination;
    char                    type;
    size_t                  offset;
    int32_t                 addend;
} dyn_linking_relocation_entry_t;
// clang-format on

#ifdef __cplusplus
}
#endif
