#include "wums_thread_specific.h"
#include <cstring>
#include <stdint.h>
#include <stdlib.h>

#define __WUMS_CONTEXT_THREAD_SPECIFIC_ID WUT_THREAD_SPECIFIC_1

typedef uint32_t OSThread;

extern const char wums_meta_info_dump[];

extern void OSFatal(const char *);
extern void OSReport(const char *, ...);

extern OSThread *OSGetCurrentThread();

typedef void (*OSThreadCleanupCallbackFn)(OSThread *thread, void *stack);

OSThreadCleanupCallbackFn
OSSetThreadCleanupCallback(OSThread *thread,
                           OSThreadCleanupCallbackFn callback);


struct __wums_thread_context {
    struct _reent reent;
    OSThreadCleanupCallbackFn savedCleanup;
};

static void
__wums_thread_cleanup(OSThread *thread,
                      void *stack) {
    struct __wums_thread_context *context;

    context = (struct __wums_thread_context *) wums_get_thread_specific(__WUMS_CONTEXT_THREAD_SPECIFIC_ID);
    if (!context || &context->reent == _GLOBAL_REENT) {
        OSReport("[%s] __wums_thread_cleanup: Context was NULL or reent was global\n", wums_meta_info_dump);
        OSFatal("__wums_thread_cleanup: Context was NULL or reent was global");
    }

    if (context->savedCleanup) {
        context->savedCleanup(thread, stack);
    }

    _reclaim_reent(&context->reent);

    // Use global reent during free since the current reent is getting freed
    wums_set_thread_specific(__WUMS_CONTEXT_THREAD_SPECIFIC_ID, _GLOBAL_REENT);

    free(context);

    wums_set_thread_specific(__WUMS_CONTEXT_THREAD_SPECIFIC_ID, NULL);
}

struct _reent *
__wums_getreent(void) {
    struct __wums_thread_context *context;

    context = (struct __wums_thread_context *) wums_get_thread_specific(__WUMS_CONTEXT_THREAD_SPECIFIC_ID);
    if (!context) {
        // Temporarily use global reent during context allocation
        wums_set_thread_specific(__WUMS_CONTEXT_THREAD_SPECIFIC_ID, _GLOBAL_REENT);

        context = (struct __wums_thread_context *) malloc(sizeof(*context));
        if (!context) {
            OSReport("[%s] __wums_getreent: Failed to allocate reent context\n", wums_meta_info_dump);
            OSFatal("__wums_getreent: Failed to allocate reent context");
            wums_set_thread_specific(__WUMS_CONTEXT_THREAD_SPECIFIC_ID, NULL);
            return NULL;
        }

        _REENT_INIT_PTR(&context->reent);
        context->savedCleanup = OSSetThreadCleanupCallback(OSGetCurrentThread(), &__wums_thread_cleanup);

        wums_set_thread_specific(__WUMS_CONTEXT_THREAD_SPECIFIC_ID, context);
    }

    return &context->reent;
}
