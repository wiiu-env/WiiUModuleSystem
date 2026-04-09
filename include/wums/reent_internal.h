#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum wums_loader_init_reent_errors_t_ {
    WUMSReent_ERROR_NONE                   = 0,
    WUMSReent_ERROR_GLOBAL_REENT_REQUESTED = 1,
    WUMSReent_ERROR_NO_THREAD              = 2,
} wums_loader_init_reent_errors_t_;

typedef void *(*WUMSReent_GetReentContext)(const void *moduleId, wums_loader_init_reent_errors_t_ *outError);
typedef void *(*WUMSReent_SetSentinel)();
typedef void (*WUMSReent_RestoreHead)(void *oldHead);
typedef bool (*WUMSReent_AddReentContext)(const void *moduleId, void *reentPtr, void (*cleanupFn)(void *), void *oldHead);

typedef uint32_t WUMS_REENT_API_VERSION;

#define WUMS_REENT_CUR_API_VERSION 0x01

typedef struct wums_loader_init_reent_args_t_ {
    WUMS_REENT_API_VERSION version;
    WUMSReent_GetReentContext get_context_ptr;
    WUMSReent_SetSentinel set_sentinel_ptr;
    WUMSReent_RestoreHead restore_head_ptr;
    WUMSReent_AddReentContext add_reent_context_ptr;
} wums_loader_init_reent_args_t_;

void WUMSReentAPI_InitInternal(wums_loader_init_reent_args_t_ args);

#ifdef __cplusplus
}
#endif