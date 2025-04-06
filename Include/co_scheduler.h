#ifndef __CO_SCHEDULER_H__
#define __CO_SCHEDULER_H__

#include "co_types.h"

extern scheduler_t *coroutine_scheduler_create();

extern void coroutine_scheduler_destroy(scheduler_t *sched);

extern void coroutine_scheduler_run(scheduler_t *sched);

// 设置内存分配函数
extern void coroutine_set_memory_alloc_sh(void* (*alloc)(size_t size));

// 设置内存释放函数
extern void coroutine_set_memory_free_sh(void (*release)(void* ptr));

#endif /* __CO_SCHEDULER_H__ */
