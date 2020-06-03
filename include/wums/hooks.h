/* based on blsug.h
 *   by Alex Chadwick
 *
 * Copyright (C) 2014, Alex Chadwick
 * Modified by Maschell, 2018-2020
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

#define WUMS_HOOK_EX(type_def,original_func) \
    extern const wums_hook_t wums_hooks_ ## original_func WUMS_SECTION("hooks"); \
    const wums_hook_t wums_hooks_ ## original_func = { \
        .type = type_def, \
        .target = (const void*)&(original_func) \
    }

typedef enum wums_hook_type_t {
    WUMS_HOOK_INIT,
    WUMS_HOOK_APPLICATION_STARTS,
} wums_hook_type_t;

typedef struct wums_hook_t {
    wums_hook_type_t type;          /*  Defines the type of the hook */
    const void *target;             /*  Address of our own, new function */
} wums_hook_t;


#define WUMS_INITIALIZE() \
    WUMS_INIT_WUT(); \
    WUMS_FINI_WUT(); \
    void __wums__init(void);\
    WUMS_HOOK_EX(WUMS_HOOK_INIT, __wums__init); \
    void __wums__init()

#define WUMS_APPLICATION_STARTS() \
    void __wums_start(void);\
    WUMS_HOOK_EX(WUMS_HOOK_APPLICATION_STARTS, __wums_start); \
    void __wums_start()
    

#ifdef __cplusplus
}
#endif
