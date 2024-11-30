## 1. Handle

### Local Handle

Local Handle 通过 `Local<SomeType>` 创建，被 Handle Scope 管理，Handle Scope 如果被删除，则 Handle Scope 管理的所有 Local Handle 都将可以被 GC 回收。

Local Handle 不存在于 C++ 调用栈，但 Handle Scope 存在于 C++ 调用栈。

Local Handle 必须创建在栈上！（尽管实现中没有禁止调用 new，但是需要使用者来控制）

### Persistent Handle

Persistent Handle 创建一个 JS 对象的持续引用，可以无视 C++ 作用域。

Persistent Handle 按照生命周期管理方式分为两种：

1. `UniquePersistent<SomeType>`：被 C++ 的 constructor 和 destructor 管理。

2. `Persisitent<SomeType>`：被它自己的 constuctor 创建，但是必须通过调用 `Persisitent:Reset` 来释放。
