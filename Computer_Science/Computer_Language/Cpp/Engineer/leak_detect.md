内存泄漏的检测可以参考这里：https://www.zhihu.com/question/382668081

# 使用方法

## macOS

### 终端运行

1. 安装 llvm
2. zshrc 添加一下内容：

```shell
export LLVM_BIN_PATH="/opt/homebrew/opt/llvm/bin"
export LDFLAGS="-L/opt/homebrew/opt/llvm/lib"
export CPPFLAGS="-I/opt/homebrew/opt/llvm/include"
PATH="$LLVM_BIN_PATH":$PATH
```

3. 使用以下命令编译：

```shell
clang++ -std=c++11  $LDFLAGS $CPPFLAGS -fsanitize=address main.cpp
```

4. 使用以下命令运行：

```shell
ASAN_OPTIONS=detect_leaks=1 ./a.out
```

# 问题

## macOS

1. 终端运行时，即使是什么都没有做的 main 函数也会出现内存泄漏，参考[这里](https://stackoverflow.com/a/36897737/14419237)的回答，可能是 startup code 的问题。