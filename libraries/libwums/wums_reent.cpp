#include "wums_reent.h"
#include "wums_thread_specific.h"
#include <cstring>
#include <stdint.h>
#include <stdlib.h>
#include <wums/wums_debug.h>

#define __WUMS_CONTEXT_THREAD_SPECIFIC_ID WUT_THREAD_SPECIFIC_0
#define WUMS_REENT_ALLOC_SENTINEL         ((__wums_reent_node *) 0xFFFFFFFF)

extern "C" __attribute__((weak)) void wut_set_thread_specific(__wut_thread_specific_id id, void *value);
extern "C" __attribute__((weak)) void *wut_get_thread_specific(__wut_thread_specific_id);

typedef uint32_t OSThread;

extern "C" void OSFatal(const char *);
extern "C" void OSReport(const char *, ...);

extern "C" OSThread *OSGetCurrentThread();

typedef void (*OSThreadCleanupCallbackFn)(OSThread *thread, void *stack);

extern "C" OSThreadCleanupCallbackFn
OSSetThreadCleanupCallback(OSThread *thread,
                           OSThreadCleanupCallbackFn callback);

#define WUMS_REENT_NODE_VERSION 1
#define WUMS_REENT_NODE_MAGIC   0x57554D53 // WUMS
static const int sReentModuleId = 0;

struct __wums_reent_node {
    // FIXED HEADER (Never move or change these offsets!)
    uint32_t magic; // Guarantees this is a __wums_reent_node
    uint32_t version;
    __wums_reent_node *next;

    // Node Version 1 Payload
    const void *moduleId;
    void *reentPtr;
    void (*cleanupFn)(__wums_reent_node *);
    OSThreadCleanupCallbackFn savedCleanup;
};

static void reclaim_reent_trampoline(__wums_reent_node *node) {
    WUMS_DEBUG_REPORT("reclaim_reent_trampoline: Destroying node %p (reent: %p)\n", node, node->reentPtr);

    if (node->reentPtr) {
        _reclaim_reent(static_cast<_reent *>(node->reentPtr));
        free(node->reentPtr);
    }
    free(node);
}

static void __wums_thread_cleanup(OSThread *thread, void *stack) {
    auto *head = static_cast<__wums_reent_node *>(wut_get_thread_specific(__WUMS_CONTEXT_THREAD_SPECIFIC_ID));

    if (!head || head == WUMS_REENT_ALLOC_SENTINEL) {
        return;
    }

    if (head->magic != WUMS_REENT_NODE_MAGIC) {
        WUMS_DEBUG_WARN("__wums_thread_cleanup: Unexpected node magic word: %08X (expected %08X).\n", head->magic, WUMS_REENT_NODE_MAGIC);
        return;
    }

    WUMS_DEBUG_REPORT("__wums_thread_cleanup: Triggered for thread %p\n", thread);

    OSThreadCleanupCallbackFn savedCleanup = nullptr;
    if (head->version >= 1) {
        savedCleanup = head->savedCleanup;
    }

    // Set to effective global during free to prevent malloc re-entrancy loops
    wut_set_thread_specific(__WUMS_CONTEXT_THREAD_SPECIFIC_ID, WUMS_REENT_ALLOC_SENTINEL);

    // Safely iterate the ABI-stable list.
    auto *curr = head;
    while (curr) {
        // Read the "next" pointer BEFORE destroying the current node.
        __wums_reent_node *next = curr->next;

        // Trigger the self-destruct sequence. Frees curr
        if (curr->cleanupFn) {
            curr->cleanupFn(curr);
        }

        curr = next;
    }

    wut_set_thread_specific(__WUMS_CONTEXT_THREAD_SPECIFIC_ID, nullptr);

    if (savedCleanup) {
        WUMS_DEBUG_REPORT("__wums_thread_cleanup: Chaining to saved cleanup for thread %p\n", thread);
        savedCleanup(thread, stack);
    }
}

struct _reent *__wums_getreent() {
    if (!wut_get_thread_specific || !wut_set_thread_specific || OSGetCurrentThread() == nullptr) {
        return _GLOBAL_REENT;
    }

    auto head = static_cast<__wums_reent_node *>(wut_get_thread_specific(__WUMS_CONTEXT_THREAD_SPECIFIC_ID));

    if (head == WUMS_REENT_ALLOC_SENTINEL) {
        return _GLOBAL_REENT;
    }

    if (head && head->magic != WUMS_REENT_NODE_MAGIC) {
        WUMS_DEBUG_WARN("__wums_getreent: Unexpected node magic word: %08X (expected %08X).\n", head->magic, WUMS_REENT_NODE_MAGIC);
        return _GLOBAL_REENT;
    }

    // Check for already allocated reent ptr.
    // (Intentionally not logging here to prevent console spam on the fast path)
    const __wums_reent_node *curr = head;
    while (curr) {
        // Use a memory address as a unique id
        if (curr->version >= 1 && curr->moduleId == &sReentModuleId) {
            return static_cast<_reent *>(curr->reentPtr);
        }
        curr = curr->next;
    }

    WUMS_DEBUG_REPORT("__wums_getreent: Allocating new context for thread %p\n", OSGetCurrentThread());

    // If not found allocate a new for THIS module.
    // Temporarily effectively use global reent during context allocation
    wut_set_thread_specific(__WUMS_CONTEXT_THREAD_SPECIFIC_ID, WUMS_REENT_ALLOC_SENTINEL);

    auto *newNode  = static_cast<__wums_reent_node *>(malloc(sizeof(__wums_reent_node)));
    auto *newReent = static_cast<struct _reent *>(malloc(sizeof(struct _reent)));

    if (!newNode || !newReent) {
        WUMS_DEBUG_WARN("__wums_getreent: Failed to allocate context! Falling back to _GLOBAL_REENT.\n");
        if (newNode) {
            free(newNode);
        }
        if (newReent) {
            free(newReent);
        }
        // reset on error
        wut_set_thread_specific(__WUMS_CONTEXT_THREAD_SPECIFIC_ID, head);
        return _GLOBAL_REENT;
    }

    _REENT_INIT_PTR(newReent);

    newNode->magic        = WUMS_REENT_NODE_MAGIC;
    newNode->version      = WUMS_REENT_NODE_VERSION;
    newNode->next         = head;
    newNode->moduleId     = &sReentModuleId;
    newNode->reentPtr     = newReent;
    newNode->cleanupFn    = reclaim_reent_trampoline;
    newNode->savedCleanup = nullptr;

    auto oldHead = head;

    // Hook cleanup logic
    if (oldHead == nullptr) {
        WUMS_DEBUG_REPORT("__wums_getreent: Hooking OSSetThreadCleanupCallback for thread %p\n", OSGetCurrentThread());
        newNode->savedCleanup = OSSetThreadCleanupCallback(OSGetCurrentThread(), &__wums_thread_cleanup);
    } else {
        WUMS_DEBUG_REPORT("__wums_getreent: Prepending to existing list for thread %p\n", OSGetCurrentThread());
        // We prepend, so we must inherit the saved cleanup from the previous head
        if (oldHead->version >= 1) {
            newNode->savedCleanup = oldHead->savedCleanup;
            oldHead->savedCleanup = nullptr;
        }
    }

    wut_set_thread_specific(__WUMS_CONTEXT_THREAD_SPECIFIC_ID, newNode);

    return newReent;
}