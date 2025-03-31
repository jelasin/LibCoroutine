#include "co_control.h"

#include <stdlib.h>
// 协程入口函数
static void coroutine_entry(coroutine_t *co);

// 创建协程
coroutine_t *coroutine_create(scheduler_t *sched, void (*func)(void *), void *arg, size_t stack_size) 
{
    coroutine_t *co = (coroutine_t *)malloc(sizeof(coroutine_t));
    if (NULL == co) 
    {
        return NULL;
    }

    co->stack = (char *)malloc(stack_size);
    if (NULL == co->stack) 
    {
        free(co);
        co = NULL;
        return NULL;
    }

    if (getcontext(&co->context) < 0) 
    {
        free(co->stack);
        co->stack = NULL;
        free(co);
        co = NULL;
        return NULL;
    }

    co->context.uc_stack.ss_sp = co->stack;
    co->context.uc_stack.ss_size = stack_size;
    co->context.uc_link = &sched->main_context; // 关联主上下文
    co->func = func;
    co->arg = arg;
    co->state = COROUTINE_READY;
    co->stack_size = stack_size;
    co->next = NULL;

    // 加入就绪队列尾部
    if (NULL == sched->ready_queue) 
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
    }

    makecontext(&co->context, (void (*)())coroutine_entry, 1, co);
    return co;
}

// 协程入口函数
static void coroutine_entry(coroutine_t *co) 
{
    co->func(co->arg);          // 执行用户函数
    co->state = COROUTINE_FINISHED; // 协程结束
}

// 协程让出CPU
void coroutine_yield(scheduler_t *sched) 
{
    coroutine_t *current = sched->current;
    if (NULL == current)
    {
        return;
    }

    // 将协程移到就绪队列尾部
    if (NULL != sched->ready_queue) 
    {
        coroutine_t *tail = sched->ready_queue;
        while (NULL != tail->next) 
        {
            tail = tail->next;
        }
        tail->next = current;
    } 
    else 
    {
        sched->ready_queue = current;
    }
    current->next = NULL;
    current->state = COROUTINE_SUSPENDED;

    // 切换回调度器
    swapcontext(&current->context, &sched->main_context);
}

