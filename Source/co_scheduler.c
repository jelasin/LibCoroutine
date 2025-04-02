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
        if (COROUTINE_READY == co->state)
        {
            sched->ready_queue = co->next;
            co->state = COROUTINE_RUNNING;
            sched->current = co;
        }
        else
        {
            // 如果当前协程不是就绪状态，跳过它
            static int count = 0; // 防止死循环
            if (++count > 100) 
            {
                return;
            }
            coroutine_t *tail = sched->ready_queue; // 找到队列尾部
            while (NULL != tail->next)
            {
                tail = tail->next; // 遍历到队列的最后一个协程
            }
            tail->next = co; // 将当前协程重新加入到队列尾部
            co->next = NULL; // 确保当前协程的next指针为NULL
            sched->ready_queue = sched->ready_queue->next; // 移出队头
            continue;
        }

        // 保存当前调度器上下文，切换至协程
        swapcontext(&sched->main_context, &co->context);

        // 协程执行完毕后的清理
        if (COROUTINE_FINISHED == co->state) 
        {
            free(co->stack);
            co->stack = NULL;
            free(co);
            co = NULL;
        }
        else if (COROUTINE_SUSPENDED == co->state || COROUTINE_SIGNALER == co->state)
        {
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
                co->state = COROUTINE_READY; // 恢复为就绪状态
            }
            co->next = NULL;
        }
    }
}