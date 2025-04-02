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

void coroutine_cond_init(coroutine_cond_t *cond)
{
    if (NULL == cond)
    {
        return;
    }
    cond->wait_queue = NULL;
    cond->state = COROUTINE_NO; // 初始化为NO
}

int coroutine_cond_wait(scheduler_t *sched, coroutine_cond_t *cond)
{
    coroutine_t *co = sched->current;
    if (NULL == co || NULL == cond || NULL == sched)
    {
        return -1; // 参数错误
    }
    // 条件变量状态为OK, 说明并未先执行本协程, 条件已经满足, 直接返回
    if (cond->state == COROUTINE_OK)
    {
        return 0;
    }

    // 将当前协程加入到条件变量的等待列表
    if (NULL == cond->wait_queue)
    {
        cond->wait_queue = co; // 第一个等待的协程
    }
    else
    {
        coroutine_t *tail = cond->wait_queue;
        while (NULL != tail->next) 
        {
            tail = tail->next;
        }
        tail->next = co; // 加入到等待列表尾部
    }
    co->next = NULL; // 确保next指针为NULL
    co->state = COROUTINE_WAITING; // 等待状态

    swapcontext(&co->context, &sched->main_context);
    /*
     * 当协程被唤醒后，会从这里继续执行。
     * 协程从sched->ready_queue中移除, 加入了cond->waiting_list中, 只有可能被coroutine_cond_signal唤醒, 且条件已经满足.
     */
    cond->state = COROUTINE_NO; // 设置条件变量状态为NO, 表示状态已经被消费
    return 0;
}

int coroutine_cond_signal(scheduler_t *sched, coroutine_cond_t *cond)
{
    coroutine_t *co = sched->current; // 当前协程
    if (NULL == co || NULL == cond || NULL == sched)
    {
        return -1;
    }

    while (NULL == cond->wait_queue)
    {
        cond->state = COROUTINE_NO; // 没有等待的协程, 设置为而OK
        co->state = COROUTINE_SIGNALER; // 当前协程是信号发送者, 便于后续调度器处理
        swapcontext(&co->context, &sched->main_context);
    }

    cond->state = COROUTINE_OK; // 设置条件变量状态为OK, 表示条件已经满足
    
    coroutine_t *waiting_co = cond->wait_queue; // 取出第一个等待的协程
    cond->wait_queue = waiting_co->next; // 移除等待列表中的第一个协程

    // 将waiting_co从等待列表中移除, 加入到就绪队列头部
    waiting_co->next = sched->ready_queue;
    sched->ready_queue = waiting_co;
    waiting_co->state = COROUTINE_READY; // 设置状态为就绪

    co->state = COROUTINE_SIGNALER; // 当前协程是信号发送者, 便于后续调度器处理
    swapcontext(&co->context, &sched->main_context);
    
    return 0;
}