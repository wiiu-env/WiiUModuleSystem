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
#include "defines/module_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WUMS_HOOK_EX(type_def, original_func)                                  \
    extern const wums_hook_t wums_hooks_##original_func WUMS_SECTION("hooks"); \
    const wums_hook_t wums_hooks_##original_func = {                           \
            .type   = type_def,                                                \
            .target = (const void *) &(original_func)}

typedef enum wums_hook_type_t {
    WUMS_HOOK_INIT_WUT_MALLOC,
    WUMS_HOOK_FINI_WUT_MALLOC,
    WUMS_HOOK_INIT_WUT_NEWLIB,
    WUMS_HOOK_FINI_WUT_NEWLIB,
    WUMS_HOOK_INIT_WUT_STDCPP,
    WUMS_HOOK_FINI_WUT_STDCPP,
    WUMS_HOOK_INIT_WUT_DEVOPTAB,
    WUMS_HOOK_FINI_WUT_DEVOPTAB,
    WUMS_HOOK_INIT_WUT_SOCKETS,
    WUMS_HOOK_FINI_WUT_SOCKETS,

    WUMS_HOOK_INIT_WRAPPER, /* Calls __init */
    WUMS_HOOK_FINI_WRAPPER, /* Calls __fini */

    WUMS_HOOK_INIT,
    WUMS_HOOK_APPLICATION_STARTS,
    WUMS_HOOK_APPLICATION_ENDS,
    WUMS_HOOK_RELOCATIONS_DONE,
    WUMS_HOOK_APPLICATION_REQUESTS_EXIT,
    // Introduced in 0.3.2
    WUMS_HOOK_DEINIT,
    // Introduced in 0.3.3
    WUMS_HOOK_ALL_APPLICATION_STARTS_DONE,
    WUMS_HOOK_ALL_APPLICATION_ENDS_DONE,
    WUMS_HOOK_ALL_APPLICATION_REQUESTS_EXIT_DONE,
    // Introduced in 0.3.4
    WUMS_HOOK_GET_CUSTOM_RPL_ALLOCATOR,  // for internal usage only
    WUMS_HOOK_CLEAR_ALLOCATED_RPL_MEMORY // for internal usage only
} wums_hook_type_t;

typedef uint32_t (*WUMSRPLAllocatorAllocFn)(int32_t size, int32_t align, void **outAddr);
typedef void (*WUMSRPLAllocatorFreeFn)(void *addr);

typedef struct wums_internal_custom_rpl_allocator_t {
    WUMSRPLAllocatorAllocFn allocFn;
    WUMSRPLAllocatorFreeFn freeFn;
} wums_internal_custom_rpl_allocator_t;

typedef struct wums_hook_t {
    wums_hook_type_t type; /*  Defines the type of the hook */
    const void *target;    /*  Address of our own, new function */
} wums_hook_t;

typedef struct wums_app_init_args_t {
    module_information_t *module_information;
} wums_app_init_args_t;

typedef struct wums_relocs_done_args_t {
    module_information_t *module_information;
} wums_relocs_done_args_t;

#define WUMS_INITIALIZE(myargs)                 \
    void __wums__init(wums_app_init_args_t);    \
    WUMS_HOOK_EX(WUMS_HOOK_INIT, __wums__init); \
    void __wums__init(wums_app_init_args_t myargs)

#define WUMS_DEINITIALIZE()                         \
    void __wums__deinit();                          \
    WUMS_HOOK_EX(WUMS_HOOK_DEINIT, __wums__deinit); \
    void __wums__deinit()

#define WUMS_APPLICATION_STARTS()                             \
    void __wums_start(void);                                  \
    WUMS_HOOK_EX(WUMS_HOOK_APPLICATION_STARTS, __wums_start); \
    void __wums_start()

#define WUMS_APPLICATION_ENDS()                           \
    void __wums_end(void);                                \
    WUMS_HOOK_EX(WUMS_HOOK_APPLICATION_ENDS, __wums_end); \
    void __wums_end()

#define WUMS_APPLICATION_REQUESTS_EXIT()                                     \
    void __wums_requests_exit(void);                                         \
    WUMS_HOOK_EX(WUMS_HOOK_APPLICATION_REQUESTS_EXIT, __wums_requests_exit); \
    void __wums_requests_exit()

#define WUMS_RELOCATIONS_DONE(myargs)                                  \
    void __wums_relocations_done(wums_relocs_done_args_t);             \
    WUMS_HOOK_EX(WUMS_HOOK_RELOCATIONS_DONE, __wums_relocations_done); \
    void __wums_relocations_done(wums_relocs_done_args_t myargs)

#define WUMS_ALL_APPLICATION_STARTS_DONE()                                      \
    void __wums_all_start_done(void);                                           \
    WUMS_HOOK_EX(WUMS_HOOK_ALL_APPLICATION_STARTS_DONE, __wums_all_start_done); \
    void __wums_all_start_done()

#define WUMS_ALL_APPLICATION_REQUESTS_EXIT_DONE()                                              \
    void __wums_all_requests_exit_done(void);                                                  \
    WUMS_HOOK_EX(WUMS_HOOK_ALL_APPLICATION_REQUESTS_EXIT_DONE, __wums_all_requests_exit_done); \
    void __wums_all_requests_exit_done()

#define WUMS_ALL_APPLICATION_ENDS_DONE()                                     \
    void __wums_all_ends_done(void);                                         \
    WUMS_HOOK_EX(WUMS_HOOK_ALL_APPLICATION_ENDS_DONE, __wums_all_ends_done); \
    void __wums_all_ends_done()

#define WUMS_INTERNAL_GET_CUSTOM_RPL_ALLOCATOR()                                       \
    wums_internal_custom_rpl_allocator_t __wums_get_custom_rpl_allocator(void);        \
    WUMS_HOOK_EX(WUMS_HOOK_GET_CUSTOM_RPL_ALLOCATOR, __wums_get_custom_rpl_allocator); \
    wums_internal_custom_rpl_allocator_t __wums_get_custom_rpl_allocator()

#define WUMS_INTERNAL_HOOK_CLEAR_ALLOCATED_RPL_MEMORY()                                    \
    void __wums_clear_allocated_rpl_memory(void);                                          \
    WUMS_HOOK_EX(WUMS_HOOK_CLEAR_ALLOCATED_RPL_MEMORY, __wums_clear_allocated_rpl_memory); \
    void __wums_clear_allocated_rpl_memory()

#ifdef __cplusplus
#define __EXTERN_C_MACRO extern "C"
#else
#define __EXTERN_C_MACRO
#endif

#define WUMS_USE_WUT_MALLOC()                                    \
    __EXTERN_C_MACRO void __init_wut_malloc();                   \
    void on_init_wut_malloc() {                                  \
        __init_wut_malloc();                                     \
    }                                                            \
    WUMS_HOOK_EX(WUMS_HOOK_INIT_WUT_MALLOC, on_init_wut_malloc); \
    __EXTERN_C_MACRO void __fini_wut_malloc();                   \
    void on_fini_wut_malloc() {                                  \
        __fini_wut_malloc();                                     \
    }                                                            \
    WUMS_HOOK_EX(WUMS_HOOK_FINI_WUT_MALLOC, on_fini_wut_malloc)

#define WUMS_USE_WUT_DEVOPTAB()                                      \
    __EXTERN_C_MACRO void __init_wut_devoptab();                     \
    void on_init_wut_devoptab() {                                    \
        __init_wut_devoptab();                                       \
    }                                                                \
    WUMS_HOOK_EX(WUMS_HOOK_INIT_WUT_DEVOPTAB, on_init_wut_devoptab); \
    __EXTERN_C_MACRO void __fini_wut_devoptab();                     \
    void on_fini_wut_devoptab() {                                    \
        __fini_wut_devoptab();                                       \
    }                                                                \
    WUMS_HOOK_EX(WUMS_HOOK_FINI_WUT_DEVOPTAB, on_fini_wut_devoptab)

#define WUMS_USE_WUT_NEWLIB()                                    \
    __EXTERN_C_MACRO void __init_wut_newlib();                   \
    void on_init_wut_newlib() {                                  \
        __init_wut_newlib();                                     \
    }                                                            \
    WUMS_HOOK_EX(WUMS_HOOK_INIT_WUT_NEWLIB, on_init_wut_newlib); \
    __EXTERN_C_MACRO void __fini_wut_newlib();                   \
    void on_fini_wut_newlib() {                                  \
        __fini_wut_newlib();                                     \
    }                                                            \
    WUMS_HOOK_EX(WUMS_HOOK_FINI_WUT_NEWLIB, on_fini_wut_newlib)

#define WUMS_USE_WUT_STDCPP()                                    \
    __EXTERN_C_MACRO void __init_wut_stdcpp();                   \
    void on_init_wut_stdcpp() {                                  \
        __init_wut_stdcpp();                                     \
    }                                                            \
    WUMS_HOOK_EX(WUMS_HOOK_INIT_WUT_STDCPP, on_init_wut_stdcpp); \
    __EXTERN_C_MACRO void __fini_wut_stdcpp();                   \
    void on_fini_wut_stdcpp() {                                  \
        __fini_wut_stdcpp();                                     \
    }                                                            \
    WUMS_HOOK_EX(WUMS_HOOK_FINI_WUT_STDCPP, on_fini_wut_stdcpp)

#define WUMS_USE_WUT_SOCKETS()                                   \
    __EXTERN_C_MACRO void __init_wut_socket();                   \
    WUMS_HOOK_EX(WUMS_HOOK_INIT_WUT_SOCKETS, __init_wut_socket); \
    __EXTERN_C_MACRO void __fini_wut_socket();                   \
    WUMS_HOOK_EX(WUMS_HOOK_FINI_WUT_SOCKETS, __fini_wut_socket)

#ifdef __cplusplus
extern "C" uint32_t __attribute__((weak)) wut_get_thread_specific(int id);
extern "C" const char wums_meta_info_linking_order[];
#else
extern uint32_t __attribute__((weak)) wut_get_thread_specific(int id);
extern const char wums_meta_info_linking_order[];
#endif

#define WUMS___INIT_WRAPPER()                                    \
    __EXTERN_C_MACRO void __init();                              \
    void __init_wrapper() {                                      \
        if (wut_get_thread_specific(0x13371337) != 0x42424242) { \
            OSFatal(wums_meta_info_linking_order);               \
        }                                                        \
        __init();                                                \
    }                                                            \
    WUMS_HOOK_EX(WUMS_HOOK_INIT_WRAPPER, __init_wrapper);

#define WUMS___FINI_WRAPPER()       \
    __EXTERN_C_MACRO void __fini(); \
    void __fini_wrapper() {         \
        __fini();                   \
    }                               \
    WUMS_HOOK_EX(WUMS_HOOK_FINI_WRAPPER, __fini_wrapper);

#ifdef __cplusplus
}
#endif