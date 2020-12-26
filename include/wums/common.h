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

#ifndef WUMS_COMMON_DEF_H_
#define WUMS_COMMON_DEF_H_

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdbool.h>
#include "defines/module_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WUMS_SECTION(x) __attribute__((__section__ (".wums." x)))

#define WUMS_META(id, value) \
    extern const char wums_meta_ ## id [] WUMS_SECTION("meta"); \
    const char wums_meta_ ## id [] = #id "=" value

#define WUMS_MODULE_EXPORT_NAME(x) WUMS_META(export_name, x); WUMS_META(wum, "0.1"); WUMS_META(buildtimestamp, __DATE__ " " __TIME__)
#define WUMS_MODULE_AUTHOR(x)  WUMS_META(author, x)
#define WUMS_MODULE_VERSION(x) WUMS_META(version, x)
#define WUMS_MODULE_LICENSE(x) WUMS_META(license, x)
#define WUMS_MODULE_DESCRIPTION(x) WUMS_META(description, x)
#define WUMS_MODULE_SKIP_ENTRYPOINT() WUMS_META(skipEntrypoint, "true")
#define WUMS_MODULE_INIT_BEFORE_RELOCATION_DONE_HOOK() WUMS_META(initBeforeRelocationDoneHook, "true")

#ifdef __cplusplus
}
#endif

#endif /* WUMS_COMMON_DEF_H_ */
