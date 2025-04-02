#ifndef __CO_TYPES_H__
#define __CO_TYPES_H__

#include <ucontext.h>

typedef enum {
    COROUTINE_READY,
    COROUTINE_RUNNING,
    COROUTINE_SUSPENDED,
    COROUTINE_SIGNALER,
    COROUTINE_WAITING,
    COROUTINE_FINISHED
} CoroutineState;

typedef enum {
    COROUTINE_OK,
    COROUTINE_NO,
} ConditionState;

typedef struct coroutine_t {
    ucontext_t context;
    char *stack;
    size_t stack_size;
    CoroutineState state;
    void (*func)(void *);
    void *arg;
    struct coroutine_t *next;
} coroutine_t;

typedef struct {
    ucontext_t main_context; // the context of the main thread
    coroutine_t *current;   // the current coroutine
    coroutine_t *ready_queue; // the ready queue of coroutines
} scheduler_t; 

typedef struct coroutine_cond_t {
    coroutine_t *wait_queue;
    ConditionState state;
} coroutine_cond_t;

#endif