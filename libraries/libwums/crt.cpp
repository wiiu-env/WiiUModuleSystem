#include "wums_reent.h"
#include "wums_thread_specific.h"

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
