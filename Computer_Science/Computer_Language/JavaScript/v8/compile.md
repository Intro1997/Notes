以下编译在 v8 1d8f52fa183e0534a931880bdf77710a235bb0e6 commit 中进行。

# Release Version

Release 版本的编译相对来说非常简单，按照[官方 embed 教程](https://v8.dev/docs/embed) 或者[这篇教程](https://v8.dev/docs/build) 进行编译都是可以正常得到产物的。如果失败的话，可以参阅 Question 节中遇到的问题以及解决方案。
由于本地环境是 arm64 macOS，所以以下遇到的问题，其解决方案保证在 macOS 14.4 arm64 环境下有效。

## 遇到的问题

### 1. v8gen.py 失败

按照[官方 embed 教程](https://v8.dev/docs/embed) 执行 `tools/dev/v8gen.py arm64.release.sample` 失败，参考 [这里](https://groups.google.com/g/v8-users/c/5qqum0FTWNE) 给出的 [这篇教程](https://github.com/v8/v8/wiki/Building-from-Source) 执行

```shell
git pull && gclient sync
tools/dev/gm.py arm64.release
```

成功

### 2. gclient sync 代理问题

如果编译出现

```
Also, --no_auth prevents the normal BOTO_CONFIG environment variable from
being used.
To use a proxy in this situation, please supply those settings in a .boto
file pointed to by the NO_AUTH_BOTO_CONFIG environment var.
```

那么需要新建一个文件存放代理地址，NO_AUTH_BOTO_CONFIG 设置为该文件路径，例如：

```shell
touch boto.cfg
```

将下面的配置写入 boto.cfg

```
[Boto]
proxy=http://http.proxy.com
proxy_port=12345
```

之后设置环境变量：

```
NO_AUTH_BOTO_CONFIG=D:\boto.cfg
```

完成后再执行 `gclient sync`

### 3. 找不到 xcodebuild

如果编译时出现：

```
xcode-select: error: tool 'xcodebuild' requires Xcode, but active developer directory '/Library/Developer/CommandLineTools' is a command line tools instance
```

则需要打开 xcode，找到 XCode > Preferences > Locations > Command Line Tools，点击选择一个 xcode 版本即可，参考[这里](https://stackoverflow.com/a/72115137/14419237)

# Debug Version

Debug 版本的编译比较容易解决，问题出现在链接上；当然，下面要列出的方案同样是在 macOS 14.4 arm64 环境下,

## 编译

假设你已经将编译 v8 所需要的所有资源 clone 完成。下面使用这行命令来进行编译：

```shell
tools/dev/gm.py arm64.debug
```

编译完成后，我们应该会得到 `out/arm64.debug` 文件夹，内部文件如下：

```
args.gn                                         libthird_party_abseil-cpp_absl.dylib.TOC
build.ninja                                     libthird_party_icu_icui18n.dylib
build.ninja.d                                   libthird_party_icu_icui18n.dylib.TOC
build.ninja.stamp                               libv8.dylib
bytecode_builtins_list_generator                libv8.dylib.TOC
d8                                              libv8_libbase.dylib
gen                                             libv8_libbase.dylib.TOC
gen-regexp-special-case                         libv8_libplatform.dylib
icudtl.dat                                      libv8_libplatform.dylib.TOC
libc++_chrome.dylib                             mksnapshot
libc++_chrome.dylib.TOC                         obj
libchrome_zlib.dylib                            snapshot_blob.bin
libchrome_zlib.dylib.TOC                        toolchain.ninja
libicuuc.dylib                                  torque
libicuuc.dylib.TOC                              v8_build_config.json
libthird_party_abseil-cpp_absl.dylib
```

这一步我没有遇到其他问题

## 链接&运行

如果你尝试直接参考这行命令<a href="#r2"><sup>[2]</sup></a>，稍作修改（链接动态链接库）来执行你的代码：

```shell
g++ -I. -Iinclude samples/hello-world.cc -o hello_world -fno-rtti -lv8_libbase -lv8_libplatform -ldl -Lout/arm64.debug/ -pthread -std=c++17 -DV8_COMPRESS_POINTERS -DV8_ENABLE_SANDBOX
```

会遇到第一个坑：

```
Undefined symbols for architecture arm64:
  "v8::ArrayBuffer::Allocator::NewDefaultAllocator()", referenced from:
      _main in hello-world-ce0dda.o
  "v8::HandleScope::HandleScope(v8::Isolate*)", referenced from:
      _main in hello-world-ce0dda.o
  "v8::HandleScope::~HandleScope()", referenced from:
      _main in hello-world-ce0dda.o
      _main in hello-world-ce0dda.o
  "v8::api_internal::ToLocalEmpty()", referenced from:
...(还有很多行)
```

这是因为我们少链接了一个 `libv8.dylib`，加上 `-lv8` 之后：

```shell
g++ -I. -Iinclude samples/hello-world.cc -o hello_world -fno-rtti -lv8_libbase -lv8_libplatform -lv8 -ldl -Lout/arm64.debug/ -pthread -std=c++17 -DV8_COMPRESS_POINTERS -DV8_ENABLE_SANDBOX
```

再次运行，发现错误只剩下了一个未找到的符号 `NewDefaultPlatform`：

```
Undefined symbols for architecture arm64:
  "v8::platform::NewDefaultPlatform(int, v8::platform::IdleTaskSupport, v8::platform::InProcessStackDumping, std::__1::unique_ptr<v8::TracingController, std::__1::default_delete<v8::TracingController>>, v8::platform::PriorityMode)", referenced from:
      _main in hello-world-b7583d.o
ld: symbol(s) not found for architecture arm64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
```

我们在 `libv8_libplatform.dylib` 中找找看是否有这个符号：

```shell
% nm -gAC out/arm64.debug/libv8_libplatform.dylib | grep "v8::platform::NewDefaultPlatform"
out/arm64.debug/libv8_libplatform.dylib: 000000000001a204 T v8::platform::NewDefaultPlatform(int, v8::platform::IdleTaskSupport, v8::platform::InProcessStackDumping, std::__Cr::unique_ptr<v8::TracingController, std::__Cr::default_delete<v8::TracingController>>, v8::platform::PriorityMode)
```

发现是 namespace 不同：

```
std::__1::unique_ptr<v8::TracingController, std::__1::default_delete<v8::TracingController>>
std::__Cr::unique_ptr<v8::TracingController, std::__Cr::default_delete<v8::TracingController>>
```

对比 release 版本的 `args.gn` 文件后，发现原来是 v8 在编译 debug 版本时，没有添加 `use_custom_libcxx = false` 配置，使用的是自己的 `libc++`。

在 `out/arm64.debug/args.gn` 中添加 `use_custom_libcxx = false` 配置后，使用 `ninja -C out/arm64.debug` 重新编译，然后获得了一堆错误（粗略扫了一眼，v8 自定义的 libc++ 使用了太多的废弃方法）。没办法，只能尝试使用 v8 自定义的 libc++。

查阅了一些链接 libc++ 的资料过后<a href="#r3"><sup>[3]</sup></a>，我发现需要基于之前的命令，添加以下内容：

1. v8 自定义 libc++ include 路径
2. v8 运行时动态链接库位置
3. v8 自定义 libc++ library 文件

最终的编译指令命令如下：

```shell
clang++ -stdlib=libc++ -nostdinc++ \
-I/path/to/v8/third_party/libc++/src/include \
-I/path/to/v8/buildtools/third_party/libc++ \
-L/path/to/v8/out/arm64.debug \
-rpath /path/to/v8/out/arm64.debug \
-I/Users/intro/Spaces/ThirdParty/v8/v8 \
-I/path/to/v8/include \
/path/to/v8/samples/my_test.cc -o my_test \
-fno-rtti \
-lchrome_zlib  -lc++_chrome  -lv8_libbase -lv8_libplatform -lv8 -ldl \
-L/path/to/v8/out/arm64.debug \
-pthread \
-std=c++17 \
-DV8_COMPRESS_POINTERS -DV8_ENABLE_SANDBOX
```

编译运行，不太熟悉 v8 的话，很有可能会看到以下错误：

```
% ./hello_world
Failed to open startup resource './snapshot_blob.bin'.


#
# Fatal error in ../../src/api/api.cc, line 9594
# Failed to deserialize the V8 snapshot blob. This can mean that the snapshot blob file is corrupted or missing.
...
```

这是因为 v8 的 debug 编译默认参数（args.gn 文件）内，默认没有添加 `v8_use_external_startup_data = false` 配置，如果你尝试过编译 `arm64.release.sample`，会发现得到的 `args.gn` 包含此选项。如果没有配置该选项，则 `InitializeExternalStartupData()` 中的 [V8_USE_EXTERNAL_STARTUP_DATA](https://github.com/v8/v8/blob/1d8f52fa183e0534a931880bdf77710a235bb0e6/src/init/startup-data-util.cc#L79C8-L79C36) 会被定义，因此需要指定 "snapshot_blob.bin" 文件路径（关于这个文件的作用，可以参考这篇文章 [V8 基础学习二：从 Context 创建流程学习 V8 快照机制](https://zhuanlan.zhihu.com/p/32249462)）。该文件可以在 `path/to/v8/out/arm64.debug/` 文件夹下找到。

[hello_world.cc](https://github.com/v8/v8/blob/1d8f52fa183e0534a931880bdf77710a235bb0e6/samples/hello-world.cc) 中调用了 `InitializeExternalStartupData()` 这个方法，我们需要做的就是将这行代码修改为：

```c++
v8::V8::InitializeExternalStartupData(argv[1]);
```

然后在执行的时候，使用以下命令：

```shell
./hello_world /path/to/v8/out/arm64.debug/ # 结尾的 / 是必须的，原因可以自己找找源代码
```

应该就可以成功执行了。

# Reference

1. <a href="https://blog.csdn.net/qhh_qhh/article/details/49078533" id="r1">使用代理同步 Chromium 代码的心得 —— CSDN</a>
2. <a href="https://v8.dev/docs/embed#:~:text=the%20GNU%20compiler%3A-,g%2B%2B,-%2DI.%20%2DIinclude" id="r2">Getting started with embedding V8 </a>
3. <a href="https://stackoverflow.com/a/51842398/14419237" id="#r3">How to specify custom libc++</a>
