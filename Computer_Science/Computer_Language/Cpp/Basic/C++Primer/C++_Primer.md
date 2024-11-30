# C++ Primer

## Chapter 1

- cout, cerr 和 clog 这三者的不同是什么？
  答：cout 和 clog 有 buffer，cerr 没有 buffer
- 多线程 cout，cerr 和 clog 输出是同步的吗？在 std::cout << "hello" << "world\n"; 会出现断开的情况吗？
  答：c++11 引入了 sync_with_stdio，默认开启以保证同步，下面是多线程的例子：

```c++
#include <chrono>
#include <iostream>
#include <thread>

void foo() {
  std::cout << "print by cout...";
  std::cout << "...print by cout" << std::endl;
}

int main(int argc, char **argv) {
  bool isSync = true;
  if (argc > 1) {
    std::string tmp = argv[1];
    if (tmp == "no") {
      isSync = false;
    }
  }
  if (isSync) {
    std::cout << "open sync\n";
  } else {
    std::cout << "close sync\n";
  }

  std::ios_base::sync_with_stdio(isSync);
  std::thread t1(foo);
  std::clog << "print by clog\n";
  std::cerr << "print by cerr\n";
  t1.join();

  return 0;
}
```

输出如下：

```
$ ./a.out no
close sync
print by clog
print by cerr
print by cout......print by cout
$ ./a.out yes
open sync
print by clog
print by cout......print by cout
print by cerr
```

关闭 sync 时，cerr 会提前输出，开启 sync 时，cerr 就在 std::endl 执行后输出。

- <font color="red">endl 起到刷新缓冲区的作用，那么在 c 中，printf 输出到默认缓冲区后，flush 被默认调用的时机是什么</font>

## Chapter 4

- 左值表达式有哪些？
  答：引用内存位置的表达式称为左值表达式 <sup><a href="#r1">[1]</a></sup>

## Chapter 7

- 可以使用代理构造函数替代重载

```cpp
#include <iostream>

class Test {
public:
  Test(int a, int b) : t1(a), t2(b) {}
  Test() : Test(1000, 100) {} // 代理构造函数
  int t1;
  int t2;
private:
};

int main() {
  Test t;
  std::cout << t.t1 << std::endl;
  std::cout << t.t2 << std::endl;
  return 0;
}
```

- 如果使用 explicit 关键字禁止类构造函数的隐式转换，可以使用 static_cast 强制执行隐式转换:

```c++
#include <iostream>
#include <string>
class A {
public:
  std::string name;
  explicit A(std::string _name) : name(_name) {
    std::cout << "call constructor\n";
  }

  void printName(const A &a) { std::cout << a.name << std::endl; }
};
int main() {
  A a("asdf");
  a.printName(static_cast<A>("hello"));
  return 0;
}
```

- 静态类成员变量被所有对象共享
- 可以使用静态私有方法或者私有静态成员变量初始化静态类成员变量

## Chapter 8

- 什么是宽字符
  答：wchar_t，16 bit 存储空间的字符类型。
- iostream 在使用时会改变状态，因此无法使用 const 限定符；如何设计一个类，使其无法使用 const，无法使用拷贝构造以及拷贝赋值
  答：拷贝构造和拷贝赋值可以通过制定 delete 来禁用；应该不能阻止一个类使用 const。
- 当一个输出流被关联到另一个输出流时，缓冲区为什么会刷新？例如：当 cin cerr 被关联到 cout 时，读写 cin/cerr 都会刷新 cout 的缓冲区
  答：若不刷新，则会将本来要输出到之前缓冲区的内容，在刷新后输出到新绑定的缓冲区中去。
- 编写一个例子验证程序崩溃时，写入缓冲区的代码被执行，但是却没有刷新

```cpp
#include <iostream>
int main() {
  int*ptr = nullptr;
  std::cout << std::endl;
  // 不输出 \n 是因为 \n 也会导致缓冲区刷新
  std::cout << "hello";
  *ptr = 123;
  return 0;
}
```

- <font color = "red">如何验证 ostream 的缓冲区是否被刷新</font>
- 文件模式中，什么是截断文件模式(trunc)？
  答：丢弃文件内容，从头开始写入，和仅指 std::fstream::out 效果相同；（仅指定 std::fstream::truc 无效）

```cpp
#include <fstream>
#include <iostream>

#define CHECK_FILE(f)                                                          \
  do {                                                                         \
    if (!f) {                                                                  \
      std::cout << "open file failed\n";                                       \
      return -1;                                                               \
    }                                                                          \
  } while (false)

int main() {
  std::fstream f("log");
  CHECK_FILE(f);
  std::string content = "", line;
  while (std::getline(f, line)) {
    content += line;
    content += "\n";
  }
  std::cout << content << std::endl;
  f.close();

  f.open("log", std::fstream::out | std::fstream::trunc);
  CHECK_FILE(f);
  f << "hello\n";
  f.close();

  return 0;
}
```

- stringstream 重新指定 str 后，是否可以通过 seekg 重制 stringstream 位置？
  答：可以，前提是不进行任何会改变 stringstream 状态的操作（例如输出等）。下面这个例子会失败

```cpp
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void printStringStream(std::istringstream &inString) {
  std::string str;
  while (std::getline(inString, str)) {
    std::cout << str;
  }
}

std::vector<std::string> strLists;

int main() {
  std::istringstream inString;
  std::ifstream inFile("entry.txt");
  std::string content = "", str;
  while (std::getline(inFile, str)) {
    strLists.push_back(str);
  }

  std::string word;
  for (const auto &i : strLists) {
    inString.str(i);
    inString.seekg(0); // failed in second loop
    std::cout << "before pass" << inString.tellg() << std::endl;
    while (inString >> word) {
      std::cout << word << std::endl;
    }
    std::cout << "after pass" << inString.tellg() << std::endl;
  }

  inFile.close();
  return 0;
}
```

for loop 第一次 loop 会将 inString 中的所有内容输出到 word 中：

1. std::istringstream::operator>>(...) 返回 \*this
2. \*this 会隐式转换成 bool，这个过程会调用 std::istringstream::operator bool(...)

过程 1 中，当 stringstream 中最后一个字符满足下面三个条件时：

- N characters are read, where N is is.width() if is.width() > 0, otherwise N is str.max_size(),
- the end-of-file condition occurs in the stream is, or
- std::isspace(c, is.getloc()) is true for the next character c in is (this whitespace character remains in the input stream). <sup><a href="#r2">[2]</a></sup>

stringstream 的状态被置为 eof，所以此时如果调用 stringstream.good() 会返回 false。
而调用 seekg 时，seekg 内部会构造 std::basic_istream<CharT,Traits>::sentry，构造内部会调用 stringstream 的 good() 判断 goodbit，若 good() 为 false，则调用 is.setstate(std::ios_base::failbit) <sup><a href="#r3">[3]</a></sup>。
good() 内部的实现非常简单，判断 state 是否完全等于 0:

```cpp
inline _LIBCPP_INLINE_VISIBILITY
bool
ios_base::good() const
{
    return __rdstate_ == 0;
}
```

显然，`state |= ios_base::eofbit` 的值不等于 0。因此，对 `tellg()` 和 `seekg()` 的调用是失败的。

## chapter 12

- shared_ptr 是如何保证出现 exception 时，被管理对象（指针）也能被正常释放的？
  答：做不到，下面这个例子无法做到在 throw 异常之后调用 Test 的析构函数

```cpp
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

class Test {
public:
  Test() { std::cout << "constructor Test\n"; }
  ~Test() { std::cout << "destructor Test\n"; }
};

int main() {
  using namespace std::chrono_literals;
  std::shared_ptr<Test> st(new Test);
  throw std::out_of_range("aaa");
  std::this_thread::sleep_for(2000ms);
  return 0;
}
```

## chater 16

- 以下例子使用 extern template 时，为什么产物体积会变大？
  答：在 ubuntu 上用 readelf -h 看了一下两个产物的头信息(详情参见 issues/templateIssue
  /cmp0 和 issues/templateIssue/cmp1 两个文件夹)
  cmp0:

```
ELF Header:
  ...
  Start of section headers:          12152 (bytes into file)
  Flags:                             0x0
  ...
```

cmp1:

```
ELF Header:
...
  Start of section headers:          12096 (bytes into file)
  Flags:                             0x0
...
```

cmp0 的 section 比 cmp1 多一些 56 个 bytes。使用`readelf --section-headers` 查看后，发现下面两个 section
的 size 有区别：
cmp0

```
...
  [25] .symtab           SYMTAB          0000000000000000 002040 000a50(2640) 18     26  75  8
  [26] .strtab           STRTAB          0000000000000000 002a90 0003ea(1002) 00      0   0  1
...
```

cmp1

```
...
  [25] .symtab           SYMTAB          0000000000000000 002040 000a20(2592) 18     26  73  8
  [26] .strtab           STRTAB          0000000000000000 002a60 0003e1(993) 00      0   0  1
...
```

不过奇怪的是 `2640+1002-2592-993=57`，并不是 56；后来检查了 .symtab 以及 .strtab，可以发现体积的增多是由于编译单元的文件名称以及编译单元中的额外的符号导致的。多出来的符号都和 tApp.cpp 有关系。这个多出来的符号以及字符串就是体积增大的原因。

- 添加 inline 后产物体积增大？（代码见 issues/inlineIssue）
  答：挺奇怪的，macOS 下体积差距有一倍多：

```
% ./compile.sh
-rwxr-xr-x  1 outro  staff  33048 Sep 19 23:21 inlineDemo.out
-rwxr-xr-x  1 outro  staff  16608 Sep 19 23:21 withoutInlineDemo.out

% g++ --version
Apple clang version 14.0.0 (clang-1400.0.29.202)
Target: x86_64-apple-darwin22.3.0
Thread model: posix
InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin
```

ubuntu(unbuntu on android termux) 上差距就很小

```
# ./compile.sh
-rwxr-xr-x. 1 root root 8976 Sep 19 15:23 inlineDemo.out
-rwxr-xr-x. 1 root root 8952 Sep 19 15:23 withoutInlineDemo.out
# g++ --version
g++ (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
Copyright (C) 2021 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

先来看 macOS 吧：

```
% otool -h inlineDemo.out
inlineDemo.out:
Mach header
      magic  cputype cpusubtype  caps    filetype ncmds sizeofcmds      flags
 0xfeedfacf 16777223          3  0x00           2    17       1008 0x00218085
% otool -h withoutInlineDemo.out
withoutInlineDemo.out:
Mach header
      magic  cputype cpusubtype  caps    filetype ncmds sizeofcmds      flags
 0xfeedfacf 16777223          3  0x00           2    16        776 0x00200085
```

列信息可以参考 `<mach-o/loader.h>`<sup><a href="#r4">[4]</a></sup>
可以看到 inline 的命令明显多于非 inline；
再深入的话就需要去了解 mach-o 文件格式了，这里转而看看 ELF 格式：

```
# ./compile.sh
-rwxr-xr-x. 1 root root 8976 Sep 19 15:43 inlineDemo.out
-rwxr-xr-x. 1 root root 8952 Sep 19 15:43 withoutInlineDemo.out
```

能看到与 macOS 类似，inline 的体积较大，大 24 字节。
看下两个产物的 header：
inlineDemo.out:

```
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              DYN (Position-Independent Executable file)
  Machine:                           AArch64
  Version:                           0x1
  Entry point address:               0x700
  Start of program headers:          64 (bytes into file)
  Start of section headers:          7184 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         28
  Section header string table index: 27
```

withoutInlineDemo.out:

```
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              DYN (Position-Independent Executable file)
  Machine:                           AArch64
  Version:                           0x1
  Entry point address:               0x700
  Start of program headers:          64 (bytes into file)
  Start of section headers:          7160 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         9
  Size of section headers:           64 (bytes)
  Number of section headers:         28
  Section header string table index: 27
```

仍然是和 section 有关，继续看看 section header:
inlineDemo.out:

```
Section Headers:
  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al
  ...
  [25] .symtab           SYMTAB          0000000000000000 001040 000870 18     26  65  8
  [26] .strtab           STRTAB          0000000000000000 0018b0 000264 00      0   0  1
  [27] .shstrtab         STRTAB          0000000000000000 001b14 0000fa 00      0   0  1
```

withoutInlineDemo.out:

```
There are 28 section headers, starting at offset 0x1bf8:

Section Headers:
...
  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al
  ...
  [25] .symtab           SYMTAB          0000000000000000 001040 000858 18     26  64  8
  [26] .strtab           STRTAB          0000000000000000 001898 000264 00      0   0  1
  [27] .shstrtab         STRTAB          0000000000000000 001afc 0000fa 00      0   0  1
```

比较后发现 .symtab 尺寸不一致，相差 24 字节，输出后发现 inline 多了一个条目：

```
Num:    Value          Size Type    Bind   Vis      Ndx Name
54: 0000000000000880     0 NOTYPE  LOCAL  DEFAULT   13 $x
```

<font color="red">0x880 是 main 的地址，13 表示 $x 处于 .text 段，但不清楚该 880 被用在哪里</font>
<font color="red">macOS 那边差距这么大是因为将函数直接 return 了，但是这样做为什么会导致体积增大 2 倍也不清楚：</font>

```cpp
inline int add(const int &a, const int &b) {
  return a + b;
}
int main() {
  return add(1, 2);
}
```

起初以为是 return function 导致的，不过代码改成下面这样后出现了相同的情况

```cpp
int add(const int &a, const int &b) { return a + b; }

int main() {
  int a = add(1, 2);
  return 0;
}
```

这样之后也出现了同样的情况，猜测可能是 withoutInlin 情况下 clang 进行了优化。如果将 a 实际的使用一下，就会出现和 g++ 相同的情况（输出 a 后，产物之间相差 24 字节）。但是不清楚为什么会这样

- string_view 没有存储 \0，string 保证 \0
- <font color="red">clang/macos clang 如何进行增量编译</font>
  答：make 似乎天生支持增量编译，因此默认使用 cmake 是可行的<sup><a href="#r5">[5]</a></sup>，如果想要加速编译可以尝试 incremental linking/ninja<sup><a href="#r6">[6]</a></sup>，以及这篇文章<sup><a href="#r7">[7]</a></sup>
- <font color="orange">能否有办法在不使用递归或者不增加辅助函数的情况下做到输出变参变量</font>

```cpp
#include <iostream>
template<typename T>
void print(const T& val) {
  std::cout << val << std::endl;
}
template<typename T, typename... Args>
void print(const T& val, const Args& ...args) {
  std::cout << val << std::endl;
  print(args...);
}
int main() {
  print(1, 42.2, 'a', "hello");
  return 0;
}
```

## Chapter 18

- 如何保证异常发生在对象构造期间，已被构造的对象得到安全释放？
  答：

1. 尝试将构造对象语句用 try catch 包裹

```cpp
#include <iostream>
class A {
public:
  int *a = nullptr;
  int *b = nullptr;
  A() {
    std::cout << "constructing A...\n";
    a = new int(1);
    throw std::out_of_range("out of range!");
    b = new int(2);
  }
  ~A() {
    std::cout << "destruct... B\n";
    if (a) {
      delete a;
    }
    if (b) {
      delete b;
    }
  }
};

int main() {
try {
A a;
} catch (std::exception e) {
}
return 0;
}

```

2. 尝试在构造函数定义中使用 try catch（但仍需要将构造对象语句放到 try catch 中）：

```cpp
#include "tAdd.hpp"
#include <iostream>
class A {
public:
  A() try { throw std::out_of_range("error"); } catch (...) {
    std::cout << "catch! in A\n";
  }
};

class Test : public A {
public:
  Test() try : A() {
  } catch (...) {
    std::cout << "catch in Test!\n";
  }
};

int main() {
  try {
    Test t;
  } catch (...) {
  }
  return 0;
}
```

- throw 一个 int 为什么会有 main.cpp:38:12: warning: exception of type 'int' will be caught by earlier handler [-Wexceptions] 的 warning?

```cpp
#include <iostream>
int main() {
  int a = 1;
  try {
    throw a;
  } catch (int &a) {
    a = 12;
    std::cout << a << std::endl;
    throw;
  } catch (int a) {
    a = 1;
    std::cout << a << std::endl;
  }
  std::cout << a << std::endl;
  return 0;
}
```

- inline namespace 以及更新问题

```
1. 向 namespace 中添加 1.0 版本代码，生成 so 文件，在编译时时候这个 so；在 2.0 版本代码中向相同的 namespace 中更新代码，生成 so，更新引用程序使用此 so 文件，是否会出现 so 文件更新问题？
2. 若 1 有问题，是否可以通过 inline namespace 解决？
3. 若不可以，是否有其他手段解决这个问题？
```

答：首先 inline namespace 的作用有两个：

1. 对于 C++ 库开发者来说，可以无需库使用者修改代码即可使用更新后的版本<sup><a href="#r8">[8]</a></sup>
2. 对于在 namespace 中嵌套一个 namespace，并且在最内层的 namespace 中声明对象/函数，且在外层使用 using 内层 namespace 的情况，无需在外部定义时，精确到内层 namespace 名称。<sup><a href="#r9">[9]</a></sup>
   其次，so 的作用就是无需重新编译应用程序时，也能获得 so 中代码的最新版本，因此更新并不会出现问题（部分场景下注意符号覆盖问题）。
   最后，inline namespace 的作用不是用来热更新的，它需要带上最新的 so 文件重新编译应用程序，可以参考 inline_namespace/ 下的例子

- 不同编译单元不可以使用同名 namespace！否则将覆盖 namespace 中的实现
- 为什么多重继承中，多个基类同时拥有一个函数名相同，参数不同的函数时，会出现二义性错误？

```cpp
#include <iostream>
#include <string>
#include <vector>

struct Base {
  void bar(int) { std::cout << "call bar(int)\n"; } // publicbydefault
protected:
  int ival;
};
struct Derived1 : virtual public Base {
  void bar(char) { std::cout << "call bar(char)\n"; } // publicbydefault
  void foo(std::string) { std::cout << "call foo(char)\n"; }

protected:
  char cval;
};
struct Derived2 : virtual public Base {
  void foo(int) { std::cout << "call foo(int)\n"; } // public by default
protected:
  int ival;
  char cval;
};
class VMI : public Derived1, public Derived2 {};
int main() {
  VMI v;
  v.bar(1);
  v.bar('c');
  v.foo(1); // 二义性错误
  v.foo('c'); // 二义性错误
  std::cout << v.ival << std::endl;
  std::cout << v.Derived2::cval << std::endl;
  std::cout << v.Derived1::cval << std::endl;
  return 0;
}
```

- 为什么当一个派生类通过不同路径继承的同一个基类时，该基类指针接受该派生类对象时会出现二义性？

```cpp
#include <iostream>
#include <string>
#include <vector>
class Class {
public:
  Class() { std::cout << "constructor Class\n"; }
};
class Base : public Class {
public:
  Base() { std::cout << "constructor Base\n"; }
};
class D1 : virtual public Base {
public:
  D1() { std::cout << "constructor D1\n"; }
};
class D2 : virtual public Base {
public:
  D2() { std::cout << "constructor D2\n"; }
};
class MI : public D1, public D2 {
public:
  MI() { std::cout << "constructor MI\n"; }
};
class Final : public MI, public Class {
public:
  Final() { std::cout << "constructor Final\n"; }
};
int main() {
  Base *pb;
  Class *pc;
  MI *pmi;
  D2 *pd2;

  pb = new Class;
  pc = new Final; // 二义性错误
  pmi = pb;
  pd2 = pmi;

  return 0;
}
```

答：派生类若与基类具备同名但签名不同的函数，则导致歧义<sup><a href="#r10">[10]</a></sup>

- <font color="orange">enum 的编译产物是什么？对于不限定作用域的 enum，为什么不允许在同一个编译单元中定义多个含有相同名称的 enum？为什么允许含有相同名称的多个 enum，单独定义在各自的编译单元中？C 中编译后的产物是否和 C++ 一致？</font>
  单编译单元例子

```cpp
// main.cpp
#include <iostream>
enum Number : unsigned int {
  one = 1,
  two = 2, // error
};

enum AppleCnt : unsigned int {
  one = 1,
  two = 2, // error
};

int main() {
  return 0;
}
```

多编译单元例子：

```cpp
// Color.cpp
enum Color : unsigned int{
  Blue = 1,
  Red = 2,
};

void initColor(Color &c) { c = Red; }

// Emotion.cpp
enum Emotion : unsigned int { Happy = 1, Blue = 2 };

void initEmotion(Emotion& e) {
  e = Blue;
}

// main.cpp
#include <iostream>

enum Color : unsigned int;
enum Emotion : unsigned int;
void initColor(Color &c);
void initEmotion(Emotion &e);

int main() {
  Color c;
  Emotion e;
  initColor(c);
  initEmotion(e);
  std::cout << c << std::endl;
  std::cout << e << std::endl;
  return 0;
}
```

- 类成员函数指针是如何索引到类成员的？
  答：用下面代码做测试：

```cpp
#include <iostream>

class Base {
public:
  virtual void foo() const { std::cout << "base foo" << std::endl; }
};

class Derived : public Base {
public:
  virtual void foo() const override { std::cout << "derived foo" << std::endl; }
};

int main() {
  void (Base::*fp)() const = &Base::foo;
  Base b;
  Derived d;
  (b.*fp)();
  (d.*fp)();
  return 0;
}
```

compiler explorer 使用 x86-64 gcc 13.2 编译<sup><a href="#r11">[11]</a></sup>：
```
(cpp code): void (Base::*fp)() const = &Base::foo;
mov     QWORD PTR [rbp-16], 1
mov     QWORD PTR [rbp-8], 0
```
可以看到初始化类函数指针时，申请了两个栈上空间
```
(cpp code): Base b;
mov     eax, OFFSET FLAT:vtable for Base+16
mov     QWORD PTR [rbp-24], rax
vtable for Base:
        .quad   0
        .quad   typeinfo for Base
        .quad   Base::foo() const
```
此时 rbp-24 中存储着 Base::foo() const 地址
```
(cpp code): Derived d;
mov     eax, OFFSET FLAT:vtable for Derived+16
mov     QWORD PTR [rbp-32], rax
vtable for Derived:
        .quad   0
        .quad   typeinfo for Derived
        .quad   Derived::foo() const
```
此时 rbp-32 中存储着 Derived::foo() const 地址
可以看到构造基类和派生类的时候分别将对应的 vtable 存储到了栈上。
调用函数指针对应函数的代码较多，就把对应的含义注释到后面了：
```
(cpp code): (b.*fp)();
        mov     rax, QWORD PTR [rbp-16]   # rax = 1
        and     eax, 1
        test    rax, rax                  # rax & rax = 1, jmp to .L4
        je      .L4                       # rax = 1 
        mov     rax, QWORD PTR [rbp-8]    # rax = 0
        mov     rdx, rax                  # rdx = rax = 0
        lea     rax, [rbp-24]             # rax = [rbp-24] 的地址
        add     rax, rdx                  # rax = rax + rdx + 0
        mov     rax, QWORD PTR [rax]      # rax = *[rbp-24] = Base::foo() const 地址
        mov     rdx, QWORD PTR [rbp-16]   # rdx = 1
        sub     rdx, 1                    # rdx = rdx - 1 = 0
        add     rax, rdx                  # rax = rax + rdx = rax + 0 = Base::foo() const 地址
        mov     rax, QWORD PTR [rax]      # rax = Base::foo() const 函数
        jmp     .L5
.L4:
        mov     rax, QWORD PTR [rbp-16]   # rax = 1
.L5:
        mov     rdx, QWORD PTR [rbp-8]    # rdx = 0
        mov     rcx, rdx                  # rcx = rdx = 0
        lea     rdx, [rbp-24]             # rdx = [rbp-24] 的地址
        add     rdx, rcx                  # rdx = rdx + 0
        mov     rdi, rdx                  # rdi = rdx = [rbp-24] 的地址
        call    rax                       # 调用 Base::foo() const 函数，rdi 是 this 指针
```
综上可以看出，通过函数指针调用类成员函数的步骤如下：
获取 vtable 地址 -> 获取对应函数偏移地址 -> 存储 this 指针作为第一个参数 -> 调用函数
派生类调用也是类似，不赘述

- 如何重载 ->* 以及 .* 运算符？
答：
1. .* 运算符不可以被重载<sup><a href="#r12">[12]</a></sup>；
2. ->* 虽然可以被重载，但是很少用到，并且重载后的 ->* 可以与原含义完全无关；Boost.Phoenix 中对它进行了重载<sup><a href="#r13">[13]</a></sup>；看起来用法类似构建了一个函数对象
- union 若包含类类型，则 union 的构造和析构默认删除，需要自己定义；若自定义的构造和析构函数未显式调用，则不会掉用对应的构造和析构函数，析构出现 UB。除了初始化列表调用默认构造之外，还可以使用 placement_new 来构造对象。

```c++
#include <iostream>
#include <string>

class Test {
public:
  Test() { std::cout << "default constructor of Test\n"; }
  ~Test() { std::cout << "default destructor of Test\n"; }

private:
  std::string val;
};

union Token {
  Token() : sVal() // 显式调用成员默认构造函数！
  {}
  ~Token() {
    Val.~Test();// 必须显式调用！！
  }
  Test sVal;
};

int main() {
  Token t;
  return 0;
}
```

## Reference

1. <a id="r1" href="https://learn.microsoft.com/zh-cn/cpp/c-language/l-value-and-r-value-expressions?view=msvc-170&viewFallbackFrom=msvc-170s">Microsoft Learn 左值和右值表达式</a>
2. <a id="r2" href="https://en.cppreference.com/w/cpp/string/basic_string/operator_ltltgtgt">cppreference operator<<,>>(std::basic_string)</a>
3. <a id="r3" href="https://en.cppreference.com/w/cpp/io/basic_istream/sentry">cppreference std::basic_ostream<CharT,Traits>::sentry</a>
4. <a id="r4" href="https://opensource.apple.com/source/cctools/cctools-921/include/mach-o/loader.h.auto.html"><mach-o/loader.h></a>
5. <a id="r5" href="https://www.reddit.com/r/cpp_questions/comments/1117cpr/does_cmake_suffice_to_make_incremental_builds/">Reddit-Does cmake suffice to make incremental builds ?</a>
6. <a id="r6" href="https://stackoverflow.com/questions/37327526/how-to-speed-up-compile-time-of-my-cmake-enabled-c-project">StackOverflow-How to speed up Compile Time of my CMake enabled C++ Project?</a>
7. <a id="r7" href="https://bot-man-jl.github.io/articles/?post=2022/Cpp-Project-Compile-Optimization">C++ 项目编译优化</a>
8. <a id="r8" href="https://stackoverflow.com/a/11018418/14419237">StackOverflow-What are inline namespaces for? [answer: 11018418]</a>
9. <a id="r9" href="https://stackoverflow.com/a/60901989/14419237">StackOverflow-What are inline namespaces for? [answer: 60901989]</a>
10. <a id="r10" href="https://stackoverflow.com/questions/411103/function-with-same-name-but-different-signature-in-derived-class-not-found#:~:text=If%20the%20resulting%20set%20of%20declarations%20are%20not%20all%20from%20sub%2Dobjects%20of%20the%20same%20type%2C%20or%20the%20set%20has%20a%20nonstatic%20member%20and%20includes%20members%20from%20distinct%20sub%2Dobjects%2C%20there%20is%20an%20ambiguity%20and%20the%20program%20is%20ill%2Dformed.%20Otherwise%20that%20set%20is%20the%20result%20of%20the%20lookup.">
    Function with same name but different signature in derived class not found [answer: 411112]</a>
11. <a id="r11" href="https://godbolt.org/z/516bccEqj">Compiler Explorer 类函数指针例子</a>
12. <a id="r12" href="https://en.cppreference.com/w/cpp/language/operators#:~:text=.*%20(member%20access%20through%20pointer%20to%20member)">Cppreference-operator overloading::Restrict</a>
13. <a id="r13" href="https://stackoverflow.com/questions/2696864/are-free-operator-overloads-evil">Are free operator->* overloads evil?</a>