/* based on blsug.h
 *   by Alex Chadwick
 *
 * Copyright (C) 2014, Alex Chadwick
 * Modified by Maschell, 2018-2022
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum wums_entry_type_t {
    WUMS_FUNCTION_EXPORT,
    WUMS_DATA_EXPORT,
} wums_entry_type_t;

typedef struct wums_entry_t {
    wums_entry_type_t type;
    const char *name;    /* name of the export */
    const void *address; /* pointer to the export */
} wums_loader_entry_t;

#define WUMS_EXPORT_FUNCTION(function) WUMS_EXPORT(WUMS_FUNCTION_EXPORT, function, function)
#define WUMS_EXPORT_DATA(pointer)      WUMS_EXPORT(WUMS_DATA_EXPORT, pointer, &pointer)

#define WUMS_EXPORT(_type, pointer, value)                \
    extern const wums_loader_entry_t wums_entry_##pointer \
            WUMS_SECTION("exports");                      \
    const wums_loader_entry_t wums_entry_##pointer = {    \
            .type    = _type,                             \
            .name    = #pointer,                          \
            .address = (const void *) value}

#ifdef __cplusplus
}
#endif