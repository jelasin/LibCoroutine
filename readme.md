# libCoroutine

C 语言实现的协程库, 使用调度器实现协程的动态添加,切换,同步等操作.

## Usage

main.c 是一个示例文件, 展示了如何使用 libcoroutine 库.

## API

### 协程控制

- `coroutine_t *co_create(scheduler_t *sched, void (*func)(void *), void *arg, size_t stack_size);` 创建一个协程, 并将其加入调度器的调度队列中.
- `void co_yield(scheduler_t *sched)` 让出当前协程的执行权, 切换到其他调度器, 并由调度器选择一个调度队列中的协程执行.

### 协程调度

- `scheduler_t *co_sch_create();` 创建一个调度器.
- `void co_sch_destroy(scheduler_t *sched);` 销毁一个调度器.
- `void co_sch_run(scheduler_t *sched);` 运行一个调度器, 直到所有协程执行完毕.

### 协程同步

- `void co_cond_init(coroutine_cond_t *cond);` 初始化协程条件变量.
- `int co_cond_signal(scheduler_t *sched, coroutine_cond_t *cond);` 唤醒一个等待在条件变量上的协程.
- `int co_cond_wait(scheduler_t *sched, coroutine_cond_t *cond);` 等待在条件变量上的协程.

## Development

- [x] 增加协程同步机制, 支持wait/signal.
- [x] 动态创建添加协程.
- [x] 支持malloc_hook/free_hook来自定义内存操作.
