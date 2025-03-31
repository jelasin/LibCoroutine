# libcoroutine

C 语言实现的协程库, 使用调度器实现协程的切换同步.

## Usage

main.c 是一个示例文件, 展示了如何使用 libcoroutine 库.

## API

### 协程控制

- `coroutine_t *coroutine_create(scheduler_t *sched, void (*func)(void *), void *arg, size_t stack_size);` 创建一个协程, 并将其加入调度器的调度队列中.
- `void coroutine_yield(scheduler_t *sched)` 让出当前协程的执行权, 切换到其他调度器, 并由调度器选择一个调度队列中的协程执行.

### 协程调度

- `scheduler_t *coroutine_scheduler_create();` 创建一个调度器.
- `void coroutine_scheduler_destroy(scheduler_t *sched);` 销毁一个调度器.
- `void coroutine_scheduler_run(scheduler_t *sched);` 运行一个调度器, 直到所有协程执行完毕.

## Development

- [ ] 增加无栈协程的支持.
- [ ] 增加更细粒度的控制, 增加协程CID标识, 让用户定义协程的调度策略.
