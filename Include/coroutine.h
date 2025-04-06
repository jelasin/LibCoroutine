#ifndef __COROUTINE_H__
#define __COROUTINE_H__

#include "co_control.h"
#include "co_scheduler.h"

inline void coroutine_set_memory_hook(void* (*alloc)(size_t size), void (*release)(void* ptr))
{
    coroutine_set_memory_alloc_ct(alloc);
    coroutine_set_memory_free_ct(release);

    coroutine_set_memory_alloc_sh(alloc);
    coroutine_set_memory_free_sh(release);
}

#endif /* __COROUTINE_H__ */