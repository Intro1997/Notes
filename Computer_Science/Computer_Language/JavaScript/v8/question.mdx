# Question

这里列出学习 v8 embedded 碰到的一些问题

1. What is ICU? What is ICU Default Location?

   ICU 是用于处理 Unicode 字符的组件，由于 v8 是一个 js 引擎，所以需要 ICU 来进行字符串处理；用户需要通过 InitializeICUDefaultLocation() 方法来制定 icu 二进制文件 icudtb.dat 的路径，该文件中存储了 icu 各种功能所需要的数据。

2. What is External Startup Data?

   InitializeExternalStartupData 需要用户传入 snapshot_blob.bin 文件的路径。浏览器环境中，每一个页面可能包含很多 iframe，每个 iframe 都对应一个 context，而每个 context 在初始化时，都需要将全局对象和内置函数初始化到 v8 的堆中。这个事情是重复的。因此，v8 会将 context 初始化后的堆内存制作成一个快照，进行下一个 context 的初始化时，将快照中的数据反序列化到 v8 的堆中，加速对 context 的初始化。

3. What is v8 Platform?
   通过 Platform 类提供的一系列接口的能力描述来看，Plaform 类负责以下事项：

   1. 管理任务所在线程的内存分配（用户可自己定制 allocator）
   2. 处理内存分配失败的情形
   3. 创建不同优先级/执行时机（延迟执行）/执行方式（是否并行）的任务

   综上来看，Platform 提供了任务运行需要的 不同机制 和 内存使用方式。

4. What is v8 isolate?

   - 每一个 isolate 都是一个 v8 引擎实例，提供堆和 GC 的管理<a href="#r1"><sup>[1]</sup></a>。HandleScope 栈的创建，对象的创建（全局/局部）都需要 isolate 的参与。
   - 一个 isolate 同时只能被一个 thread 访问；如果要进行多线程对同一个 isolate 访问，必须要是用 Locker/UnLocker

5. What is v8 isolate scope?

   v8 支持一个线程存在多个 isolate，但是一个线程只能同时使用一个 isolate<a href="#r2"><sup>[2]</sup></a>，当 isolate1 在 scope 内进入 isolate2 中，执行一些操作后，退出 isolate2，切换到 isolate1 时，需要恢复到 isolate1 的状态，这就是 isolate scope 的作用。它使用 stack 管理 isolates，并在构造时调用 Enter 方法入栈，析构时调用 Exit 方法 Pop 当前的 isolate，并释放当前的 isolate（若 isolate1 和 isolate2 是同一个，则不进行释放）。

6. What is v8 handle scope?

   所有的 `v8::Local<v8::Type>` 都是 handle，都需要在创建后存在于某个地方，这个地方就是 handle scope。它可以理解为存在于栈上的对象。

7. What is v8 context?

   是 javascript 的执行环境，内部存在一个 global 变量以及所有 javascript 的内置函数，同时可以通过绑定自定义对象到 global 对象的方式，使 javascript 能够直接访问自定一的对象。

8. ~~What is scope in v8? Why both isolate and context have scope~~ What is Context scope?

   v8 context 是 javascript 的执行环境，存在 global 和一系列内置函数，同时，一个 isolate 对象中，能够存在多个 v8 context 对象。当 context1 在 scope 内进入 context2 中，执行一些操作后，退出 context2，切换到 context1 时，需要恢复到 context1 的状态，这就是 context scope 的作用。与 isolate scope 不同的是，context scope 在析构的时候，并不会销毁当前的 context。

9. 使用 EscapableHandleScope.Escape(array) 返回 local handle 时，会拷贝 array 到一个 enclosing scope 中，这个 enclosing scope 是什么？

   源代码中没有找到这个 enclosing scope

10. 创建第一个 V8 上下文时，需要分析的内置 js 代码是什么？

    应该是通过这种方式进行添加的，`TFJ()`<a href="#r3"><sup>[3]</sup></a>，上一份工作，小程序引擎内部是通过 Script Run 的方式添加的。

11. https://v8.dev/docs/embed#advanced-guide:~:text=Next%2C%20wrap%20a%20C%2B%2B%20point%20by%20creating%20a%20new%20instance%20of%20the%20template%20and%20then%20setting%20the%20internal%20field%200%20to%20an%20external%20wrapper%20around%20the%20point%20p. 中，将 Point 对象的指针，作为一个 InterlaField 设置给了 v8::Object，这个 v8::Object 在教程中被视作与该 Point 对象一一对应。但是既然是一一对应的关系，这里设置给 v8::Object 应当是此 Point 对象的 Field（x 或者 y），这里为什么是 Point 对象本身？应该怎么看待这个 Internal Field 呢？

    没找到答案

12. GetArrayFromFuncReturn() 这个例子中，如果不创建 Isolate Scope，则 Create3DCoordinate() 函数中 构造 EscapableHandleScope 类型会卡住（并非循环，活动监视器上类似于等待锁的状态）

    如果不创建 Isolate Scope，Isolate::GetCurrent() 会返回 nullptr。GetCurrent 最终会返回 `g_current_isolate_` 这个指针，它在 Isolate::SetIsolateThreadLocals 中被赋值。而 SetIsolateThreadLocals 会在 Isolate::Delete，Isolate::Init，Isolate::Enter，Isolate::Exit 中被调用，然而 v8::Isolate::New() 调用链中，调用到的初始化方法是 Isolate::Initialize，并没有调用到 Init，因此如果不创建 Isolate::Scope，则 `g_current_isoalte_` 无法被初始化。

    nullptr 卡顿的问题实在是没有找到答案

13. v8 GetZoneBackingAllocator 中的 zone 是什么？

    没有找到答案

14. 创建 isolate 的时候，为什么需要使用 NewDefaultAllocator()，它的作用是什么？
    一般创建 v8 isolate 的时候，需要分配一个 ArrayBuffer Alocator 给 isolate

```c++
  v8::Isolate::CreateParams create_params;
  create_params.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  v8::Isolate* isolate = v8::Isolate::New(create_params);
```

它的作用是为 `const buf = new ArrayBuffer(1024)` 创建 JS ArrayBuffer 对象提供支持；详见 `TestArrayBufferAllocator()` 这个例子；同时也可以参考[ V8 接口指南--ArrayBuffer::Allocator -- 知乎](https://zhuanlan.zhihu.com/p/187550111)

15. v8 的 Locker 有什么作用呢？
    由于在多线程中环境中，一次只能有一个线程占用 isolate 对象，为了方便用户不违反这个规定，v8 提供了一个 Locker，确保用户多线程使用 isolate 时，同时只有一个线程访问 isolate。具体例子可以参考 `TestLocker()`；它内部创建了两个运行在不同线程的 c++ addon 函数，一个负责修改 js 对象，一个负责输出 js 对象

16. SealHandleScope 的作用是什么？
    构造 SealHandleScope 后，必须在接下来的局部作用域内重新创建 HandleScope 对象，才能够继续创建 Local 变量：

    ```cpp
    void SomeFunc(v8::Isolate* isolate) {
       v8::Isolate::Scope isolate_scope(isolate);
       v8::HandleScope handle_scope(isolate);
       v8::SealHandleScope seal_handle_scope(isolate);
       v8::Local<v8::String> v8_str = v8::String::NewFromUtf8Literal(isolate, "hello"); // failed here!
       {
          v8::HandleScope inner_handle_scope(isolate);
          v8::Local<v8::String> v8_str2 = v8::String::NewFromUtf8Literal(isolate, "hello"); // success
       }
    }
    ```

    它的作用在于能够让 event_loop 内的 callback 必须使用 HandleScope 对象<a href="#r5"><sup>[5]</sup></a>。

# Reference

1. <a id="r1" href="https://stackoverflow.com/a/19394400/14419237">What exactly is the difference between v8::Isolate and v8::Context? -- stack overflow</a>
2. <a id="r2" href="https://stackoverflow.com/questions/19383724/what-exactly-is-the-difference-between-v8isolate-and-v8context#:~:text=Only%20one%20thread%20may%20access%20a%20given%20isolate%20at%20a%20time%2C">What exactly is the difference between v8::Isolate and v8::Context? -- stack overflow</a>
3. <a id="r3" href="https://waterbang.top/2023/01/24/%E5%A6%82%E4%BD%95%E5%9C%A8v8%E4%B8%AD%E7%BB%99js%E5%86%85%E7%BD%AE%E5%87%BD%E6%95%B0/">如何在 v8 中给 js 内置函数</a>
4. <a id="r4" href="https://zhuanlan.zhihu.com/p/187550111">V8 接口指南--ArrayBuffer::Allocator -- 知乎</a>
5. <a id="r5" href="https://github.com/nodejs/node/blob/faefe560efbbefb5a58f7198305f417fbba4a007/src/README.md">Node.js C++ codebase</a>
