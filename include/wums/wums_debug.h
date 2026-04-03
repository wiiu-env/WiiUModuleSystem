#pragma once

#if defined(__GNUC__) || defined(__clang__)

#define WUMS_FORMAT_PRINTF(fmt, args) __attribute__((__format__(__printf__, fmt, args)))

#else // not __GNUC__ and not __clang__

#define WUMS_FORMAT_PRINTF(fmt, args)

#endif //__GNUC__ or __clang__


#ifdef __cplusplus
extern "C" {
#endif


#ifdef DEBUG
extern void OSReport(const char *fmt, ...) WUMS_FORMAT_PRINTF(1, 2);
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

#define WUMS_DEBUG_WARN(fmt, ...) OSReport("[%s] " fmt, wums_meta_info_dump, ##__VA_ARGS__)