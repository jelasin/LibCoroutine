#ifndef __CO_CONTROL_H__
#define __CO_CONTROL_H__

#include "co_types.h"

extern coroutine_t *coroutine_create(scheduler_t *sched, void (*func)(void *), void *arg, size_t stack_size);

extern void coroutine_yield(scheduler_t *sched);

extern void coroutine_cond_init(coroutine_cond_t *cond);

extern int coroutine_cond_wait(scheduler_t *sched, coroutine_cond_t *cond);

extern int coroutine_cond_signal(scheduler_t *sched, coroutine_cond_t *cond);

// 设置内存分配函数
extern void coroutine_set_memory_alloc_ct(void* (*alloc)(size_t size));

// 设置内存释放函数
extern void coroutine_set_memory_free_ct(void (*release)(void* ptr));

#endif /* __CO_CONTROL_H__ */