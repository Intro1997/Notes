# Question

1. nodejs 和 v8 的区别是什么？
   nodejs 天然支持模块化：v8 是 js 引擎，并不提供简化的 module 相关的接口（例如 require）。虽然提供 import，但是需要自己手动编写并维护读取文件的代码<a href="#r1"><sup>[1]</sup></a>；nodejs 则天然提供 module 相关的接口<a href="#r2"><sup>[2]</sup></a>，js 可以直接通过 require 的方式引入 C++ addon 打包好的产物。

2. binding.gyp 中的配置可以参考 gyp 文档<a href="#r3"><sup>[3]</sup></a>。

3. NODE_MODULE 和 NODE_MODULE_INIT / NODE_MODULE_INITIALIZE 的区别
   NODE_MODULE_INIT / NODE_MODULE_INITIALIZE 提供当前上下文信息，对于需要使用（感知到）当前上下问的对象提供便利；NODE_MODULE 则无法提供当前上下文信息。

4. v8 的 Locker 有什么作用呢？
   由于在多线程中环境中，一次只能有一个线程占用 isolate 对象，为了方便用户不违反这个规定，v8 提供了一个 Locker，确保用户多线程使用 isolate 时，同时只有一个线程访问 isolate。具体例子可以参考 `TestLocker()`；它内部创建了两个运行在不同线程的 c++ addon 函数，一个负责修改 js 对象，一个负责输出 js 对象

5. nodejs 的启动过程是否用到了 Locker？
   用到了，在 [node_main_instance.cc](https://github.com/nodejs/node/blob/5db35b4d7d73ab6484785f3f19fe709e33174c6b/src/node_main_instance.cc#L90) 中

6. 如何在 nodejs 中，写一个异步执行的 C++ addon？

7. nodejs 的调用 `uv_run` 之后，为什么需要调用 `platform->DrainTasks(isolate);`，`DrainTasks` 做了什么？
   DrainTasks 做了一下几件事情：

   - `worker_thread_task_runner_` 等待它的 `pending_worker_tasks_` 中所有的任务被执行完成
   - 将 `foreground_delayed_tasks_` 中的任务通过 `uv_timer_t` 转移到 `scheduled_delayed_tasks_`，并在符合 timeout 条件后，将 Task 加入 `pending_worker_tasks_` 中等待执行。
   - 执行所有 `foreground_tasks_` 中的任务，若任务是 `ScheduleTask` 类型，则在符合 timeout 条件后，将 Task 加入 `pending_worker_tasks_` 中等待执行。

   之所以需要 `BlockingDrain()`，是因为之后的执行流程中，需要修改 `pending_worker_tasks_`，所以需要将之前的任务都执行一遍。
   DrainTasks() 负责添加任务到执行执行队列（不是数据结构中的 queue）中去，所以需要在每次调用 `uv_run` 之后，调用一次
   详细过程可以参考下图：
    <div style="text-align: center">
      <img src="imgs/DrainTask.svg" width=600/>
   </div>

8. node 情形下，什么时候需要使用 isolate::scope?
 isolate::scope 确保了多线程，多 isolate 的情形下，对 v8 的使用是安全的；所以对于 nodejs 来说，在使用到 node env 的地方，必须是要 locker 和 isolate::scope 确保线程和多上下文安全

# Reference

1. <a id = "r1" href="https://gist.github.com/surusek/4c05e4dcac6b82d18a1a28e6742fc23e">V8 module importing - simple example -- Github Gist</a>
2. <a id = "r2" href="https://nodejs.org/docs/latest/api/addons.html">C++ addons -- Node.js</a>
3. <a id = "r3" href="https://gyp.gsrc.io/index.md">GYP Home Page</a>
