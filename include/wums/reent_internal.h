#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef bool (*WUMSReent_GetReentContext)(const void *moduleId, void **outPtr);
typedef bool (*WUMSReent_AddReentContext)(const void *moduleId, void *reentPtr, void (*cleanupFn)(void *));

typedef uint32_t WUMS_REENT_API_VERSION;

#define WUMS_REENT_CUR_API_VERSION 0x02

typedef struct wums_loader_init_reent_args_t_ {
    WUMS_REENT_API_VERSION version;
    WUMSReent_GetReentContext get_context_ptr;
    WUMSReent_AddReentContext add_reent_context_ptr;
} wums_loader_init_reent_args_t_;

void WUMSReentAPI_InitInternal(wums_loader_init_reent_args_t_ args);

#ifdef __cplusplus
}
#endif