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
#include "hooks.h"
#include <coreinit/debug.h>

#ifdef __cplusplus
#define __EXTERN_C_MACRO extern "C"
#else
#define __EXTERN_C_MACRO
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define WUMS_VERSION "0.3.2"

#define WUMS_MODULE_EXPORT_NAME(__module_name)                                                                      \
    WUMS_META(export_name, __module_name);                                                                          \
    WUMS_META(wums, WUMS_VERSION);                                                                                  \
    WUMS_USE_WUT_MALLOC();                                                                                          \
    WUMS_USE_WUT_SOCKETS();                                                                                         \
    WUMS_USE_WUT_NEWLIB();                                                                                          \
    WUMS_USE_WUT_STDCPP();                                                                                          \
    WUMS___INIT_WRAPPER();                                                                                          \
    WUMS___FINI_WRAPPER();                                                                                          \
    WUMS_META(buildtimestamp, __DATE__ " " __TIME__);                                                               \
    extern const char wums_meta_module_name[] WUMS_SECTION("meta");                                                 \
    const char wums_meta_module_name[] = __module_name;                                                             \
    extern const char wums_meta_info_dump[] WUMS_SECTION("meta");                                                   \
    const char wums_meta_info_dump[] = "(module: " __module_name ";"                                                \
                                       "wums " WUMS_VERSION ";"                                                     \
                                       "buildtime: " __DATE__ " " __TIME__ ")";                                     \
    extern const char wums_meta_info_linking_order[] WUMS_SECTION("meta");                                          \
    const char wums_meta_info_linking_order[] = "Loading module \"" __module_name "\" failed.\n"                    \
                                                "Function \"wut_get_thread_specific\" returned unexpected value.\n" \
                                                "Please check linking order (expected \"-lwums -lwut\")";

#define WUMS_MODULE_AUTHOR(x)                          WUMS_META(author, x)
#define WUMS_MODULE_VERSION(x)                         WUMS_META(version, x)
#define WUMS_MODULE_LICENSE(x)                         WUMS_META(license, x)
#define WUMS_MODULE_DESCRIPTION(x)                     WUMS_META(description, x)
#define WUMS_MODULE_SKIP_INIT_FINI()                   WUMS_META(skipInitFini, "true")
#define WUMS_MODULE_INIT_BEFORE_RELOCATION_DONE_HOOK() WUMS_META(initBeforeRelocationDoneHook, "true")

#ifdef __cplusplus
}
#endif
