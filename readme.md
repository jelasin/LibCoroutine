# libCoroutine

A coroutine library implemented in C, using a scheduler to achieve dynamic addition, switching, synchronization, and other operations of coroutines.

## Usage

main.c is an example file demonstrating how to use the libcoroutine library.

## API

### Coroutine Control

- `coroutine_t *co_create(scheduler_t *sched, void (*func)(void *), void *arg, size_t stack_size);` creates a coroutine and adds it to the scheduler's queue.
- `void co_yield(scheduler_t *sched)` yields the execution right of the current coroutine, switches to other schedulers, and allows the scheduler to select a coroutine from the queue to execute.

### Coroutine Scheduling

- `scheduler_t *co_sch_create();` creates a scheduler.
- `void co_sch_destroy(scheduler_t *sched);` destroys a scheduler.
- `void co_sch_run(scheduler_t *sched);` runs a scheduler until all coroutines have finished executing.

### Coroutine Synchronization

- `void co_cond_init(coroutine_cond_t *cond);` initializes a coroutine condition variable.
- `int co_cond_signal(scheduler_t *sched, coroutine_cond_t *cond);` wakes up one coroutine waiting on the condition variable.
- `int co_cond_wait(scheduler_t *sched, coroutine_cond_t *cond);` waits on the condition variable.

## Development

- [x] Added coroutine synchronization mechanism, supporting wait/signal.
- [x] Dynamic creation and addition of coroutines.
- [x] Support for malloc_hook/free_hook to customize memory operations.
