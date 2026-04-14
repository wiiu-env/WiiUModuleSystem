#include "wums_reent.h"

#include "wums/hooks.h"
#include "wums/reent_internal.h"
#include "wums/wums_debug.h"

#include <stdlib.h>

extern "C" void OSFatal(const char *format, ...);
extern "C" void *OSGetCurrentThread();

typedef void *(*MEMAllocFromDefaultHeapFn)(uint32_t size);
typedef void *(*MEMAllocFromDefaultHeapExFn)(uint32_t size, int32_t alignment);
typedef void (*MEMFreeToDefaultHeapFn)(void *ptr);

extern MEMAllocFromDefaultHeapFn MEMAllocFromDefaultHeap;
extern MEMAllocFromDefaultHeapExFn MEMAllocFromDefaultHeapEx;
extern MEMFreeToDefaultHeapFn MEMFreeToDefaultHeap;


struct wums_loader_init_reent_args_t {
    WUMSReent_GetReentContext get_context_ptr       = nullptr;
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
    __internal_functions.add_reent_context_ptr = args.add_reent_context_ptr;
}

// use variable in the .data section as unique module id
static const int sReentModuleId = 0;

bool wums_backend_get_context(const void *id, void **outPtr) {
    return __internal_functions.get_context_ptr(id, outPtr);
}

bool wums_backend_register_context(const void *moduleId, void *reentPtr, void (*cleanupFn)(void *)) {
    return __internal_functions.add_reent_context_ptr(moduleId, reentPtr, cleanupFn);
}

static void reclaim_reent_trampoline(void *payload) {
    WUMS_DEBUG_REPORT("reclaim_reent_trampoline: Destroying reent payload: %p\n", payload);

    if (payload) {
        auto *reentPtr = static_cast<_reent *>(payload);
        _reclaim_reent(reentPtr);

        // Make sure to use MEMFreeToDefaultHeap
        MEMFreeToDefaultHeap(reentPtr);
    }
}

struct _reent *__wums_getreent() {
    if (!OSGetCurrentThread()) {
        return _GLOBAL_REENT;
    }
    void *existingCtx = nullptr;
    if (!wums_backend_get_context(&sReentModuleId, &existingCtx)) {
        return _GLOBAL_REENT;
    }
    // if non-null we can use it
    if (existingCtx) {
        return static_cast<_reent *>(existingCtx);
    }

    // Use `MEMAllocFromDefaultHeap` to avoid creating a new reent for allocating the reent
    auto *newReent = static_cast<struct _reent *>(MEMAllocFromDefaultHeap(sizeof(struct _reent)));
    if (!newReent) {
        return _GLOBAL_REENT;
    }

    WUMS_DEBUG_REPORT("Allocated context! for thread %p: %p\n", OSGetCurrentThread(), newReent);

    _REENT_INIT_PTR(newReent);

    const bool result = wums_backend_register_context(
            &sReentModuleId,
            newReent,
            reclaim_reent_trampoline);

    if (!result) {
        WUMS_DEBUG_WARN("Failed to register context for thread %p\n", OSGetCurrentThread());
        _reclaim_reent(newReent);
        // Make sure to call the free function
        MEMFreeToDefaultHeap(newReent);
        return _GLOBAL_REENT;
    }

    return newReent;
}