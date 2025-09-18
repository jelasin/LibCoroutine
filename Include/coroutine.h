#ifndef __COROUTINE_H__
#define __COROUTINE_H__

#include "co_control.h"
#include "co_scheduler.h"
#include "co_types.h"

inline void co_set_mem_hook(void* (*alloc)(size_t size), void (*release)(void* ptr))
{
    coroutine_set_memory_alloc_ct(alloc);
    coroutine_set_memory_free_ct(release);

    coroutine_set_memory_alloc_sh(alloc);
    coroutine_set_memory_free_sh(release);
}

inline scheduler_t *co_sch_create()
{
    return coroutine_scheduler_create();
}

inline void co_sch_destroy(scheduler_t *sched)
{
    coroutine_scheduler_destroy(sched);
}

inline void co_sch_run(scheduler_t *sched)
{
    coroutine_scheduler_run(sched);
}

inline coroutine_t *co_create(scheduler_t *sched, void (*func)(void *), void *arg, size_t stack_size)
{
    return coroutine_create(sched, func, arg, stack_size);
}

inline void co_yield(scheduler_t *sched)
{
    coroutine_yield(sched);
}
inline void co_cond_init(coroutine_cond_t *cond)
{
    coroutine_cond_init(cond);
}

inline int co_cond_wait(scheduler_t *sched, coroutine_cond_t *cond)
{
    return coroutine_cond_wait(sched, cond);
}

inline int co_cond_signal(scheduler_t *sched, coroutine_cond_t *cond)
{
    return coroutine_cond_signal(sched, cond);
}

#endif /* __COROUTINE_H__ */