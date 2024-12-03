# 解释器和编译器

最初接触到这两个概念，源自于大学那会儿（2017 年），从写 python 的同学哪里听来的关于“cpp 是编译语言，python 是解释语言”这个对两个语言“本质”上的对比，并深以为意，直到我开始接触 React 和 Next.js。

# Babel is a JavaScript compiler.

> Babel is a JavaScript compiler.

出自 [Babel 官网](https://babeljs.io/)，是官网的一个大大的标语。不由得让我重新思考到底什么是 Compiler 什么是 Interpreter。

关于解释器：

> In computer science, an interpreter is a computer program that directly executes instructions written in a programming or scripting language, without requiring them previously to have been compiled into a machine language program.
>
> 1. Parse the source code and perform its behavior directly;
> 2. Translate source code into some efficient intermediate representation or object code and immediately execute that;
> 3. Explicitly execute stored precompiled bytecode[1] made by a compiler and matched with the interpreter's virtual machine.
>
> 摘录自维基百科 —— Interpreter (computing)

关于编译器：

> In computing, a compiler is a computer program that translates computer code written in one programming language (the source language) into another language (the target language). The name "compiler" is primarily used for programs that translate source code from a high-level programming language to a low-level programming language (e.g. assembly language, object code, or machine code) to create an executable program.[1][2]: p1 [3]
>
> 摘录自维基百科 —— Compiler

那么 JavaScript 到底是一个什么样的语言呢？

经常做前端网页开发的同学，可能会第一时间想到浏览器的控制台，想到测试 js 代码时，在控制台里面一行一行执行 js 代码的场景。从这里来看，js 毫无疑问是一个解释语言。

如果有接触过 NodeJS 和 V8 的同学（就是我），可能会因为 `v8::Script::Compile()` 对这个问题产生一丝疑惑，一门解释语言为什么会可以被编译？

打开 V8 的官网，也能看到 V8 对于自己的定位是一个 JavaScript 引擎，这是因为它涵盖了 js 的编译、内存分配以及内存回收，从这里来看，JavaScript 是一个编译型语言。

# 关键是上下文

从上面的例子来看，对于一个语言是否是编译型语言还是解释型语言，还是要考虑它所处的场景，就如[这里的回答](https://stackoverflow.com/a/9623999)所说的那样：
> For me personally, I am somewhat cautious of the idea of calling a language interpreted or compiled. It's an implementation decision, not part of the language specification. If you want to talk about compiled or interpreted JavaScript, ask it in the context of an actual implementation of the language specification.


