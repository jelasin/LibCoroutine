#include "coroutine.h"
#include <stdio.h>

struct arg
{
    int num;
    scheduler_t *sched;
    coroutine_cond_t *cond;
};

void producer(void *arg)
{
    struct arg *p = (struct arg *)arg;
    scheduler_t *sched = p->sched;
    for (int i = 0; i < 3; i++)
    {
        printf("Producing %d %d\n", i, ++p->num);
        coroutine_cond_wait(sched, p->cond); // 等待条件变量
    }
}

void consumer(void *arg)
{
    struct arg *p = (struct arg *)arg;
    scheduler_t *sched = p->sched;
    for (int i = 0; i < 3; i++) 
    {
        printf("Consuming %d %d\n", i, --p->num);
        coroutine_cond_signal(sched, p->cond); // 通知生产者
    }
}


void other(void *arg)
{
    struct arg *p = (struct arg *)arg;
    scheduler_t *sched = p->sched;
    for (int i = 0; i < 3; i++) 
    {
        printf("other     %d\n", i);
        coroutine_yield(sched); // 切换到其他协程
    }
}

int main() 
{
    struct arg arg;
    scheduler_t *sched = coroutine_scheduler_create();
    arg.sched = sched;
    arg.num = 10;
    coroutine_cond_t cond;
    coroutine_cond_init(&cond); // 初始化条件变量
    arg.cond = &cond; // 传递条件变量给协程

    coroutine_create(sched, producer, &arg, 1 << 20); // 1MB栈
    coroutine_create(sched, consumer, &arg, 1 << 20);
    coroutine_create(sched, other, &arg, 1 << 20);

    coroutine_scheduler_run(sched);
    coroutine_scheduler_destroy(sched);
    return 0;
}
