
#ifndef __CO_SCHEDULER_H__
#define __CO_SCHEDULER_H__

#include "co_types.h"

extern scheduler_t *coroutine_scheduler_create();

extern void coroutine_scheduler_destroy(scheduler_t *sched);

extern void coroutine_scheduler_run(scheduler_t *sched);


#endif /* __CO_SCHEDULER_H__ */
