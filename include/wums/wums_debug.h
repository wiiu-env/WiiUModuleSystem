#pragma once

extern "C" void OSReport(const char *fmt, ...);

#ifdef __cplusplus
extern "C" {
#endif
extern const char wums_meta_info_dump[];
#ifdef __cplusplus
}
#endif

#ifdef DEBUG
#define WUMS_DEBUG_REPORT(fmt, ...) OSReport("[%s] " fmt, wums_meta_info_dump, ##__VA_ARGS__)
#else
#define WUMS_DEBUG_REPORT(fmt, ...)
#endif

#define WUMS_DEBUG_WARN(fmt, ...) OSReport("\033[33m[%s] " fmt "\033[0m", wums_meta_info_dump, ##__VA_ARGS__)