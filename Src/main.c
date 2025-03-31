#include "coroutine.h"
#include <stdio.h>

struct arg
{
    int num;
    scheduler_t *sched;
};


void producer(void *arg) {
    struct arg *p = (struct arg *)arg;
    scheduler_t *sched = p->sched;
    for (int i = 0; i < 3; i++) {
        printf("Producing %d %d\n", i, ++p->num);
        coroutine_yield(sched);
    }
}

void consumer(void *arg) 
{
    struct arg *p = (struct arg *)arg;
    scheduler_t *sched = p->sched;
    for (int i = 0; i < 3; i++) 
    {
        printf("Consuming %d %d\n", i, --p->num);
        coroutine_yield(sched);
    }
}

int main() 
{
    struct arg arg;
    scheduler_t *sched = coroutine_scheduler_create();
    arg.sched = sched;
    arg.num = 10;
    coroutine_create(sched, producer, &arg, 1 << 20); // 1MB栈
    coroutine_create(sched, consumer, &arg, 1 << 20);
    coroutine_scheduler_run(sched);
    coroutine_scheduler_destroy(sched);
    return 0;
}
