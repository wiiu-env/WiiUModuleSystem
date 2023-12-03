#include "wums_reent.h"
#include "wums_thread_specific.h"
#include <cstdio>
#include <cstring>

extern "C" void OSFatal(const char *);

int main(int argc, char **argv) {
    OSFatal("This file needs to be run with the Wii U Module Loader.");
    return 0;
}

extern "C" __attribute__((weak)) void __wut_socket_init_devoptab();

extern "C" __attribute__((weak)) void __wut_socket_fini_devoptab();

static int __wut_socket_devoptab_added = 0;

extern "C" __attribute__((weak)) void socket_lib_init();

extern "C" void __attribute__((weak)) __init_wut_socket();

void __attribute__((weak)) __init_wut_socket() {
    if (!&__wut_socket_init_devoptab || !&socket_lib_init) return;
    if (!__wut_socket_devoptab_added) {
        socket_lib_init();
        __wut_socket_init_devoptab();
        __wut_socket_devoptab_added = 1;
    }
}

extern "C" void __attribute__((weak)) __fini_wut_socket();

void __attribute__((weak)) __fini_wut_socket() {
    if (!&__wut_socket_init_devoptab || !&__wut_socket_fini_devoptab) return;
    if (__wut_socket_devoptab_added) {
        __wut_socket_fini_devoptab();
        __wut_socket_devoptab_added = 0;
    }
}

struct _reent *__getreent(void) {
    return __wums_getreent();
}

extern "C" void __attribute__((weak)) wut_set_thread_specific(__wut_thread_specific_id id, void *value);

void wut_set_thread_specific(__wut_thread_specific_id id, void *value) {
    return wums_set_thread_specific(id, value);
}

extern "C" void *__attribute__((weak)) wut_get_thread_specific(__wut_thread_specific_id id);

void *wut_get_thread_specific(__wut_thread_specific_id id) {
    return wums_get_thread_specific(id);
}

extern "C" const char wums_meta_module_name[];
extern "C" void __attribute__((weak)) abort(void);
extern "C" void __attribute__((weak)) __assert_func(const char *file, int line, const char *func, const char *failedexpr);
extern "C" void __attribute__((weak)) __assert(const char *file, int line, const char *failedexpr);

void __attribute__((weak))
abort(void) {
    char buffer[512] = {};
    strcat(buffer, "Wii U Module System (module: \"");
    strcat(buffer, wums_meta_module_name);
    strcat(buffer, "\"):\n Abort called. Uncaught exception?");
    OSFatal(buffer);
    /* NOTREACHED */
    while (1)
        ;
}

void __attribute__((weak))
__assert_func(const char *file,
              int line,
              const char *func,
              const char *failedexpr) {
    char tmp[512]    = {};
    char buffer[512] = {};

    snprintf(tmp, sizeof(tmp), "Wii U Module System (module: \"%s\"):\n\n"
                               "assertion \"%s\" failed:\n\n"
                               "file \"%s\", line %d%s%s",
             wums_meta_module_name, failedexpr, file, line, func ? ", function: " : "", func ? func : "");

    // make sure to add a \n every 64 characters to fit on the DRC screen.
    char *target_ptr = buffer;
    int i = 0, j = 0, lineLength = 0;
    while (tmp[i] != '\0' && j < (int) sizeof(buffer) - 2) {
        if (tmp[i] == '\n') {
            lineLength = 0;
        } else if (lineLength >= 64) {
            target_ptr[j++] = '\n';
            lineLength      = 0;
        }
        target_ptr[j++] = tmp[i++];
        lineLength++;
    }

    OSFatal(buffer);
    /* NOTREACHED */
}

void __attribute__((weak))
__assert(const char *file,
         int line,
         const char *failedexpr) {
    __assert_func(file, line, NULL, failedexpr);
    /* NOTREACHED */
}