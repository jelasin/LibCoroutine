#ifndef __CO_CONTROL_H__
#define __CO_CONTROL_H__

#include "co_types.h"

extern coroutine_t *coroutine_create(scheduler_t *sched, void (*func)(void *), void *arg, size_t stack_size);

extern void coroutine_yield(scheduler_t *sched);

#endif /* __CO_CONTROL_H__ */