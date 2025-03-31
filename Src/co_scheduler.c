#include "co_scheduler.h"

#include <stdlib.h>

// 调度器初始化
scheduler_t *coroutine_scheduler_create() 
{
    scheduler_t *sched = (scheduler_t *)malloc(sizeof(scheduler_t));
    if (NULL == sched) 
    {
        return NULL;
    }
    sched->current = NULL;
    sched->ready_queue = NULL;
    return sched;
}

// 释放资源
void coroutine_scheduler_destroy(scheduler_t *sched) 
{
    if (NULL == sched) 
    {
        return;
    }
    free(sched);
    sched = NULL;
}

// 启动调度器
void coroutine_scheduler_run(scheduler_t *sched)
{
    if (getcontext(&sched->main_context) == -1) 
    {
        return;
    }

    while (sched->ready_queue != NULL) 
    {
        coroutine_t *co = sched->ready_queue;
        sched->ready_queue = co->next;
        co->state = COROUTINE_RUNNING;
        sched->current = co;

        // 保存当前调度器上下文，切换至协程
        swapcontext(&sched->main_context, &co->context);

        // 协程执行完毕后的清理
        if (co->state == COROUTINE_FINISHED) 
        {
            free(co->stack);
            co->stack = NULL;
            free(co);
            co = NULL;
        }
        else
        {
            co->next = NULL;
            if (sched->ready_queue == NULL)
            {
                sched->ready_queue = co;
            }
            else
            {
                coroutine_t *tail = sched->ready_queue;
                while (NULL != tail->next) 
                {
                    tail = tail->next;
                }
                tail->next = co;
                co->next = NULL;
                co->state = COROUTINE_READY;
            }
        }
    }
}