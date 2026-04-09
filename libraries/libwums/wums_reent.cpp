#include "wums_reent.h"

#include "wums/hooks.h"
#include "wums/reent_internal.h"
#include "wums/wums_debug.h"

extern "C" void OSFatal(const char *format, ...);
extern "C" void *OSGetCurrentThread();

#include <stdlib.h>

struct wums_loader_init_reent_args_t {
    WUMSReent_GetReentContext get_context_ptr       = nullptr;
    WUMSReent_SetSentinel set_sentinel_ptr          = nullptr;
    WUMSReent_RestoreHead restore_head_ptr          = nullptr;
    WUMSReent_AddReentContext add_reent_context_ptr = nullptr;
};

static wums_loader_init_reent_args_t __internal_functions = {};

void WUMSReentAPI_InitInternal(wums_loader_init_reent_args_t_ args) {
    if (args.version > WUMS_REENT_CUR_API_VERSION) {
        OSFatal("Incompatible reent api version");
        return;
    }
    WUMS_DEBUG_REPORT("WUMSReentAPI_InitInternal: Initializing reent module\n");

    __internal_functions.get_context_ptr       = args.get_context_ptr;
    __internal_functions.set_sentinel_ptr      = args.set_sentinel_ptr;
    __internal_functions.add_reent_context_ptr = args.add_reent_context_ptr;
    __internal_functions.restore_head_ptr      = args.restore_head_ptr;
}

// use variable in the .data section as unique module id
static const int sReentModuleId = 0;

void *wums_backend_get_context(const void *id, wums_loader_init_reent_errors_t_ *outError) {
    return __internal_functions.get_context_ptr(id, outError);
}

void *wums_backend_set_sentinel() {
    return __internal_functions.set_sentinel_ptr();
}

void wums_backend_restore_head(void *head) {
    __internal_functions.restore_head_ptr(head);
}

bool wums_backend_register_context(const void *moduleId, void *reentPtr, void (*cleanupFn)(void *), void *oldHead) {
    return __internal_functions.add_reent_context_ptr(moduleId, reentPtr, cleanupFn, oldHead);
}

static void reclaim_reent_trampoline(void *payload) {
    WUMS_DEBUG_REPORT("reclaim_reent_trampoline: Destroying reent payload: %p\n", payload);

    if (payload) {
        auto *reentPtr = static_cast<_reent *>(payload);
        _reclaim_reent(reentPtr);
        free(reentPtr);
    }
}

struct _reent *__wums_getreent() {
    if (!OSGetCurrentThread()) {
        return _GLOBAL_REENT;
    }
    wums_loader_init_reent_errors_t_ error = WUMSReent_ERROR_NONE;
    if (void *existingCtx = wums_backend_get_context(&sReentModuleId, &error)) {
        return static_cast<_reent *>(existingCtx);
    }
    switch (error) {
        case WUMSReent_ERROR_GLOBAL_REENT_REQUESTED:
        case WUMSReent_ERROR_NO_THREAD:
            return _GLOBAL_REENT;
        case WUMSReent_ERROR_NONE:
            break;
    }

    auto *oldHead  = wums_backend_set_sentinel();
    auto *newReent = static_cast<struct _reent *>(malloc(sizeof(struct _reent)));
    if (!newReent) {
        wums_backend_restore_head(oldHead);
        return _GLOBAL_REENT;
    }

    WUMS_DEBUG_REPORT("Allocated context! for thread %p: %p\n", OSGetCurrentThread(), newReent);

    _REENT_INIT_PTR(newReent);

    bool result = wums_backend_register_context(
            &sReentModuleId,
            newReent,
            reclaim_reent_trampoline,
            oldHead);

    if (!result) {
        WUMS_DEBUG_WARN("Failed to register context for thread %p\n", OSGetCurrentThread());
        _reclaim_reent(newReent);
        free(newReent);
        wums_backend_restore_head(oldHead);
        return _GLOBAL_REENT;
    }

    return newReent;
}