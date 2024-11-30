# 程序员的自我修养

## 第一章

- 而 CPU 采用倍频的方式与系统总线通信
  - 什么是 CPU 倍频
    答：CPU 倍频用于使 CPU 工作频率调整到系统总线频率倍数的状态，但不清楚工作频率不成倍数或不一致的后果是什么
  - CPU 倍频是通过调整系统总线频率来提高，还是通过调整 CPU 频率来提高？
    答：主板的 PCH-H 和 XTAL 共同作用输出 BCLK（参考：https://www.zhihu.com/question/322405320/answer/680088260），在 BIOS 中，通过调整 BCLK 值，间接改变 CPU 的工作频率
  - 主板支持的倍频是固定的，举一例说明某主板支持的倍频，以及支持的 CPU 型号
    答：没找到。。。官网也没有标注 BCLK 的信息（无论是 CPU 官网还是主板官网）
- 系统自带脚本语言运行环境是什么？compiler、linker 和脚本语言环境的地位是否一致？
  答：与 compiler、linker 地位不一样；大概找了一下，shell 作为操作系统本身的一个可运行软件或者一个功能，可以是编程语言编译生成的，具体是什么后续必要的话再看看（参考：https://www.zhihu.com/question/21126582）
- 为什么使用软中断方式提供系统调用？是因为需要暂停系统当前服务，转而去使用系统提供的其他服务吗？
  答：系统调用一般需要切换到内核态由操作系统完成，因此会引发一次 CPU 中断，将 CPU 控制权由用户进程转交给操作系统
- 分时系统中的监控程序是如何实现的？监控程序如果持续占用 CPU，如何做到不暂停监控程序本身的情况下，切换占用 CPU 的程序？
  答：<font color="red">没找到答案</font>
- 多任务系统中，什么是运行在受硬件保护的级别？硬件保护了什么？
  答：我理解“受硬件保护”是指操作系统使硬件处于安全使用的状态，对于硬件保护一般有三个方面：CPU、内存和 IO。对于 CPU 来说，硬件保护使得单个进程无法长时间占用 CPU 本身；对于内存来说，进程无法访问限制以外的地址；对 IO 来说有三点：不能终止其他 IO 进程、不能读取其他 IO 进程数据、任何进程都可以拥有比其他进程更高的优先级
- 用户指定优先级是一种建议吗？被用户指定优先级的线程是否意味着其优先级不会再改变？
  答：对于 Windows 的 setThreadPriority 接口来说，用户对优先级的设置并不是一种建议，但要注意，系统本身仍然会进行一些操作：若线程优先级较低（例如设置为 THREAD_MODE_BACKGROUND_BEGIN），且与其他线程共享了大量临界区、堆和 handle 等资源，那么系统会对优先级进行干预。即使线程的优先级是最低的，也不会出现饿死的情况（参考：https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-setthreadpriority#:~:text=When%20a%20thread,never%20be%20starved.）
- 信号量的使用场景有哪些？信号量读取和修改的例子
  答：多元信号量可以控制资源的有限访问，例如数据库只接受 n 个线程的访问。
- 条件变量的使用场景
  答：符合某种条件时，进行锁的释放，否则等待；条件变量的使用需要注意虚假唤醒（参考：https://www.zhihu.com/question/271521213/answer/363337814）
- 可重入锁的作用
  答：若同一线程有重复获取锁的逻辑时，避免等待锁释放而继续进行；例如递归加锁时可通过可重入锁避免陷入死锁；其他使用场景见 https://my.oschina.net/noahxiao/blog/101558
- 寄存器回写时机（volatile 解决）；双重 if 对 lock 的效率有何提升？
  答：第一个 if 使得重入 GetInstance 时，只有 pInst 为 NULL 时才加锁
  ```C
  volatile T* pInst = 0;
  T* GetInstance() {
    if (pInst == NULL) {
      lock();
      if(pInst == NULL) {
        pInst = new T;
      }
      unlock();
    }
    return pInst;
  }
  ```
- 跨平台禁止 CPU 乱序执行的方法？以及检测方法？
  答：参考这里的内存屏障：https://zhuanlan.zhihu.com/p/43526907，通过这里的代码验证https://blog.csdn.net/striver1205/article/details/115183007
- 用户线程和内核线程的区别
  答：https://www.jianshu.com/p/5a4fc2729c17
- 一对多和多对多线程模型在不同 OS 的使用方式？以及使用场景？
  答：<font color='red'>没找到，或许会在《C++ 并发编程实战》找到答案</font>

## 第二章

- Ubuntu gcc -E 编译后会生成一些行，它们的含义是什么？书中有提到过 #pragma 会被保留，为什么没有在 .i 文件中看到？
  这些行被称作 linemakers，用于生成 debug 和 error 信息，但是不清楚具体如何使用。
  main.i

```
# 1 "main.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "main.c"
# 1 "sum.h" 1

int sum(int a, int b) {
  return a + b;
}
# 2 "main.c" 2

int sum(int a, int b);

int main() {
  return 0;
}
```

- 使用 /usr/lib/gcc/x86_64-linux-gnu/4.8.4/cc1 编译时输出的 perf 信息各阶段表示的含义是什么？

```
func1 main
Analyzing compilation unit
Performing interprocedural optimizations
 <*free_lang_data> <visibility> <early_local_cleanups> <*free_inline_summary> <whole-program>Assembling functions:
 func1 main
Execution times (seconds)
 phase setup             :   0.00 ( 0%) usr   0.00 ( 0%) sys   0.01 (33%) wall    1095 kB (85%) ggc
 phase parsing           :   0.00 ( 0%) usr   0.00 ( 0%) sys   0.01 (33%) wall     109 kB ( 8%) ggc
 phase opt and generate  :   0.01 (50%) usr   0.00 ( 0%) sys   0.00 ( 0%) wall      63 kB ( 5%) ggc
 phase finalize          :   0.01 (50%) usr   0.00 ( 0%) sys   0.00 ( 0%) wall       0 kB ( 0%) ggc
 parser (global)         :   0.00 ( 0%) usr   0.00 ( 0%) sys   0.01 (33%) wall     101 kB ( 8%) ggc
 unaccounted todo        :   0.01 (50%) usr   0.00 ( 0%) sys   0.00 ( 0%) wall       0 kB ( 0%) ggc
 TOTAL                 :   0.02             0.00             0.03               1283 kB
```

<font color='red'>没找到信息</font>，这个可能是预编译和编译阶段耗时，具体项目含义可能需要再找找资料

- 尝试使用 lex 进行词法扫描，验证书中的结论
  <font color='orange'>后续尝试一下</font>
- 尝试使用 yacc 进行语法树构建
  <font color='orange'>后续尝试一下</font>

## 第三章

- a.out 魔术中并没有 01 或者 07 查询下为什么
  答：不同系统下的 a.out magic number 不一样，对于 ELF 文件来说，magic number 一直都是 7f 'E' 'L' 'F'
- a.out 的 Machine Type 没有 Advanced Micro Devices X86-64 类型，查询下这个类型在 elf.h 中对应的 value 是什么
  <font color='red'>明天尝试写一个 c 代码获取对应的值</font>
- a.out 为什么有 NULL section？
  <font color='red'>可能是编译器的行为，可参考 [stckoverflow](https://stackoverflow.com/questions/26812142/what-is-the-use-of-the-sht-null-section-in-elf)</font>
- .note.GNU-stack 段为什么 size 为 0，但也占用了 4 字节？（没有其他段的地址和它一样）
  答：可能是为了对齐。
- flag 中的 A 表示 ALLOC，W 表示 WRITE，X 表示 EXECUTINSTR；那么 MS 表示该段的什么属性？
  答：不太常用，参考[这里](https://reverseengineering.stackexchange.com/questions/21460/meaning-of-flags-in-elf-section-header) 以及 [这里](https://refspecs.linuxbase.org/elf/gabi4+/ch4.sheader.html)
- .rela.text 的 sh_type 是 RELA，他的 sh_link 表示该段使用的符号表在段表的下标，那么如何查看它使用了什么符号？
  以下面这个为例

```
Relocation section '.rela.text' at offset 0x6b0 contains 5 entries:
  Offset             Info             Type               Symbol's Value  Symbol's Name + Addend
0000000000000011  000000050000000a R_X86_64_32            0000000000000000 .rodata + 0
```

Offset 表示在 text 段中的偏移：

```
0000000000000000 <func1>:
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	48 83 ec 10          	sub    $0x10,%rsp
   8:	89 7d fc             	mov    %edi,-0x4(%rbp)
   b:	8b 45 fc             	mov    -0x4(%rbp),%eax
   e:	89 c6                	mov    %eax,%esi
> 10:	bf 00 00 00 00       	mov    $0x0,%edi
  15:	b8 00 00 00 00       	mov    $0x0,%eax
  1a:	e8 00 00 00 00       	callq  1f <func1+0x1f>
  1f:	c9                   	leaveq
  20:	c3                   	retq
```

Info 前 8 字节表示在 .rodata+0 中的偏移：

```
Hex dump of section '.rodata':
  0x00000000 25640a00                            %d..
```

%=25, d=64，偏移是 5 字节，可以看出读取的是 %d
Info 后 8 字节表示 Type。Info 的定义可以在 elf.h 中的 Elf64 类型定义中找到。

- SYMTAB 和 DYNSYS 的 link 和 info 均表示操作系统相关信息，这个信息具体是什么呢？
  <font color='red'>没找到。。。</font>
- .strtab 被什么使用呢？

```
$ readelf -p .strtab main.o
String dump of section '.strtab':
  [     1]  main.c
  [     8]  static_var.1731
  [    18]  staic_var2.1732
  [    28]  global_init_var
  [    38]  global_uninit_var
  [    4a]  func1
  [    50]  printf
  [    57]  main
```

能看到是一些变量名，被 .symtab 使用，参考[这里](https://stackoverflow.com/questions/64967077/distinguish-shstrtab-and-strtab-in-elf-file#:~:text=well%20in%20practice).-,.strtab,contain%20the%20section%20index%20of%20the%20string%20table%20in%20most%20cases.,-.shstrtab)

- .symtab 中只有八个段符号，并且没有段名称，原因是什么

```
$ readelf -s main.o --wide
Symbol table '.symtab' contains 16 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS main.c
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    3
     4: 0000000000000000     0 SECTION LOCAL  DEFAULT    4
     5: 0000000000000000     0 SECTION LOCAL  DEFAULT    5
     6: 0000000000000004     4 OBJECT  LOCAL  DEFAULT    3 static_var.1731
     7: 0000000000000000     4 OBJECT  LOCAL  DEFAULT    4 staic_var2.1732
     8: 0000000000000000     0 SECTION LOCAL  DEFAULT    7
     9: 0000000000000000     0 SECTION LOCAL  DEFAULT    8
    10: 0000000000000000     0 SECTION LOCAL  DEFAULT    6
    11: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    3 global_init_var
    12: 0000000000000004     4 OBJECT  GLOBAL DEFAULT  COM global_uninit_var
    13: 0000000000000000    33 FUNC    GLOBAL DEFAULT    1 func1
    14: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND printf
    15: 0000000000000021    51 FUNC    GLOBAL DEFAULT    1 main

$ readelf -h main.o
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              REL (Relocatable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          0 (bytes into file)
  Start of section headers:          400 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           0 (bytes)
  Number of program headers:         0
  Size of section headers:           64 (bytes)
  Number of section headers:         13
  Section header string table index: 10

$ readelf -p .shstrtab main.o
String dump of section '.shstrtab':
  [     1]  .symtab
  [     9]  .strtab
  [    11]  .shstrtab
  [    1b]  .rela.text
  [    26]  .data
  [    2c]  .bss
  [    31]  .rodata
  [    39]  .comment
  [    42]  .note.GNU-stack
  [    52]  .rela.eh_frame
```

<font color='red'>可以看到 .symtab 中有 7 个段，Header 中有 13 个 section header；.shstrtab 中有 10 个 section name；不太明白这里为什么对应不上</font>

## 第四章

- ld 默认入口函数是 \_start，这个函数在哪儿被定义？是否可以在 c 文件中重写该函数？
  答： \_start 被 define 在 /usr/lib/x86_64-linux-gnu/crt1.o 内，也有答案提到它被定义在 crt0.o，他们之间的区别可以参考[这里](https://stackoverflow.com/questions/2709998/crt0-o-and-crt1-o-whats-the-difference).
- 对于 C++ 来说，若多个编译单元中存在同一个虚函数的实现，G++ 会在什么情况下生成重复的虚函数表？G++又是如何去除重复的？在使用了与模版函数相同的去重方式后，为什么会出现相同名称段有不同内容的情况？
  <font color='red'>阅读完 C++对象模型后来看</font>
- 测试 G++ 编译链接含有 template 的编译单元时，是否会生成 .gnu.linkonce.name 段，若没有，它是用什么方式代替的？
  <font color='red'>无法找到 linkonce ，它直接使用了 C++ 符号混淆的方式将函数实现放在 .text._Z3sumIfET_RKS0_S2_ text 段里</font>
- 测试 ffunction-section 和 fdata-section
  答：通过制定以下两个配置可以去除没有使用的符号
  ```shell
  # 函数和变量符号分别按照段落存储
  gcc -ffunction-sections -fdata-sections -c main.cpp
  # -Wl 表示指定链接阶段执行的操作，--gc-sections 表示链接时去除没有被使用的段
  gcc -Wl,--gc-sections -o $@ main.o
  ```
- 由于不同平台甚至是同一个编译器不同版本的 C++ 产物都会存在二进制不兼容问题，这是否是阻碍 C++ 库生态向 js/python 等脚本语言库生态发展的因素之一？
  答：参考[知乎](https://www.zhihu.com/question/26117075/answer/86056267)这里的回答，确实是一部分原因。
- whereis 原理是什么？什么文件是能被找到的，什么是不能被找到的。
- int 0x80 指令对应的参数在 64 bit 下如何传递？
  答：参考这里 https://syscall.sh/about
- 使用 ld 脚本 lds 配置链接过程时，对于 a.out 格式的文件，输出段名只能使用 .text .data .bss，可以举出依据吗？其他格式的文件有不同限制吗？
  答：可参考[这里](https://web.archive.org/web/20110103155407/http://osxfaq.com/man/5/a.out.ws)，这个只是 a.out 文件的格式要求，可能没有说明为什么段名只能使用 .text .data .bss.

  ## 第五章

  ## 第六章

- CPU 的位数指的到底是什么？位数如何决定寻址能力以及如何决定虚拟内存大小？
  答：CPU 位指的是 CPU 最多支持读取的数据长度以及 ALU 支持的最大运算位数，参考 [wikipedia](https://zh.wikipedia.org/wiki/64%E4%BD%8D%E5%85%83)；位数越多，代表能寻址到底地址范围越广；CPU 不决定虚拟内存大小，由 OS 决定；虚拟地址空间大小也不由 cpu 位数决定，也由 OS 决定，CPU 位数只决定物理地址空间大小
- Address Window Extension 的具体做法是什么？
  答：AWE 内包含[未分页的地址以及物理内存的部分区域](https://learn.microsoft.com/en-us/windows/win32/memory/address-windowing-extensions#:~:text=AWE%20uses%20physical%20nonpaged%20memory%20and%20window%20views%20of%20various%20portions%20of%20this%20physical%20memory%20within%20a%2032%2Dbit%20virtual%20address%20space.)，[未分页的地址其实是虚拟内存地址的组合](https://answers.microsoft.com/en-us/windows/forum/all/whats-non-paged-pool-memory/46d33fe6-58d7-4c32-a37b-90b22789fd43#:~:text=The%20nonpaged%20pool%20consists%20of%20virtual%20memory%20addresses%20that%20are%20guaranteed%20to%20reside%20in%20physical%20memory%20as%20long%20as%20the%20corresponding%20kernel%20objects%20are%20allocated.)。
- program header 中 FileSize 和 MemSize 为什么会不一样？
  答：额外部分数据可能是 BSS 段分配的数据
- 什么是 COW（Copy on Write）?
  答：在父进程创建子进程时，如果子进程想要处理父进程部分数据的同时，不影响父进程的数据以及处理，需要将父进程的大部分内容进行复制，这会浪费大量内存；COW 只会复制页表，使得父子进程同时可访问到同一个物理地址；但当子进程对数据进行修改时，OS 就会拷贝一份数据出来，避免影响父进程的运行。参考：https://imageslr.com/2020/copy-on-write.html
- 什么是主设备号？什么是次设备号？
  答：linux /dev 文件夹下，使用 ls -l 命令

  ```
  intro@intro:/dev$ ls -l
  total 0
  crw-r--r--  1 root  root     10, 235 Jul 27 02:11 autofs
  drwxr-xr-x  2 root  root         300 Jul 27 02:11 block
  drwxr-xr-x  2 root  root          60 Jul 27 02:11 bsg
  crw-rw----  1 root  disk     10, 234 Jul 27 02:11 btrfs-control
  drwxr-xr-x  3 root  root          60 Jul 27 02:11 bus
  drwxr-xr-x  2 root  root        3820 Jul 27 02:11 char
  crw--w----  1 root  tty       5,   1 Jul 27 02:12 console
  lrwxrwxrwx  1 root  root          11 Jul 27 02:11 core -> /proc/kcore
  drwxr-xr-x 10 root  root         200 Jul 27 02:11 cpu
  crw-------  1 root  root     10, 124 Jul 27 02:11 cpu_dma_latency
  crw-------  1 root  root     10, 203 Jul 27 02:11 cuse
  drwxr-xr-x  7 root  root         140 Jul 27 02:11 disk
  brw-rw----  1 root  disk    253,   0 Jul 27 02:11 dm-0
  drwxr-xr-x  2 root  root          60 Jul 27 02:11 dma_heap
  drwxr-xr-x  3 root  root         100 Jul 27 02:11 dri
  crw-------  1 root  root     10, 126 Jul 27 02:11 ecryptfs
  crw-rw----  1 root  video    29,   0 Jul 27 02:11 fb0
  lrwxrwxrwx  1 root  root          13 Jul 27 02:11 fd -> /proc/self/fd
  crw-rw-rw-  1 root  root      1,   7 Jul 27 02:11 full
  crw-rw-rw-  1 root  root     10, 229 Jul 27 02:11 fuse
  ...
  ```

  其中

  ```
  crw-r--r--  1 root  root     10, 235 Jul 27 02:11 autof
  ```

  的 `10, 235`，10 为主设备号，表明该设备使用的驱动；235 为次设备号，表示该设备具体是什么，驱动文件类型可以参考这里：https://www.kernel.org/doc/Documentation/admin-guide/devices.txt

- 什么是映像文件的节点号？
  答：我们可以运行下面这个可执行文件看看：

```c
#include <unistd.h>
int main() {
  sleep(-1);
  return 0;
}
```

```
$ gcc main.c && ./a.out &
[1] 466896
$ cat /proc/466896/maps
55c74e4c9000-55c74e4ca000 r--p 00000000 fd:00 264344                     /home/intro/learn/c/a.out
55c74e4ca000-55c74e4cb000 r-xp 00001000 fd:00 264344                     /home/intro/learn/c/a.out
55c74e4cb000-55c74e4cc000 r--p 00002000 fd:00 264344                     /home/intro/learn/c/a.out
55c74e4cc000-55c74e4cd000 r--p 00002000 fd:00 264344                     /home/intro/learn/c/a.out
55c74e4cd000-55c74e4ce000 rw-p 00003000 fd:00 264344                     /home/intro/learn/c/a.out
7f98503ce000-7f98503d1000 rw-p 00000000 00:00 0
7f98503d1000-7f98503f9000 r--p 00000000 fd:00 534308                     /usr/lib/x86_64-linux-gnu/libc.so.6
7f98503f9000-7f985058e000 r-xp 00028000 fd:00 534308                     /usr/lib/x86_64-linux-gnu/libc.so.6
7f985058e000-7f98505e6000 r--p 001bd000 fd:00 534308                     /usr/lib/x86_64-linux-gnu/libc.so.6
7f98505e6000-7f98505ea000 r--p 00214000 fd:00 534308                     /usr/lib/x86_64-linux-gnu/libc.so.6
7f98505ea000-7f98505ec000 rw-p 00218000 fd:00 534308                     /usr/lib/x86_64-linux-gnu/libc.so.6
7f98505ec000-7f98505f9000 rw-p 00000000 00:00 0
7f98505ff000-7f9850601000 rw-p 00000000 00:00 0
7f9850601000-7f9850603000 r--p 00000000 fd:00 534157                     /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
7f9850603000-7f985062d000 r-xp 00002000 fd:00 534157                     /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
7f985062d000-7f9850638000 r--p 0002c000 fd:00 534157                     /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
7f9850639000-7f985063b000 r--p 00037000 fd:00 534157                     /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
7f985063b000-7f985063d000 rw-p 00039000 fd:00 534157                     /usr/lib/x86_64-linux-gnu/ld-linux-x86-64.so.2
7ffcf59d5000-7ffcf59f6000 rw-p 00000000 00:00 0                          [stack]
7ffcf59f6000-7ffcf59fa000 r--p 00000000 00:00 0                          [vvar]
7ffcf59fa000-7ffcf59fc000 r-xp 00000000 00:00 0                          [vdso]
ffffffffff600000-ffffffffff601000 --xp 00000000 00:00 0                  [vsyscall]
```

最后两行，中倒数第二行为 inode 就是映像文件的节点号，它是指出了该文件在 inode table 中的索引；inode table 中的每个节点都是一个数据结构，它存储了文件的信息，具体可参考这里：https://zh.wikipedia.org/wiki/Inode。<font color='red'>至于 inode table 在什么地方，这里没有找到</font>。

## 第七章

- 链接两个动态链接库时，为什么 gcc 无法告知重复符号？
  答：这个和 gcc 是否指定了 -c 选项有关：

```c
// main.c
int sum1(int, int);
int sum2(int, int);

int main() {
  sum1(1, 2);
  sum2(1, 2);
  return 0;
}
```

```c
// sum1.c
#include <stdio.h>
int inner_sum(int a, int b) { return a + b; }

int sum1(int a, int b) {
  printf("sum 1\n");
  printf("f: %d, s: %d\n", a, b);
  int val = inner_sum(a, b);
  printf("addition: %d\n", val);
  return val;
}
```

```c
// sum2.c
#include <stdio.h>
int inner_sum(int a, int b) { return a - b; }

int sum2(int a, int b) {
  printf("sum 2\n");
  printf("f: %d, s: %d\n", a, b);
  int val = inner_sum(a, b);
  printf("addition: %d\n", val);
  return val;
}
```

```bash
# compile.sh
gcc sum1.c -fPIC -shared -o libsum1.so
# gcc sum1.c -fPIC -shared -o libsum1_no-c.so
gcc sum2.c -fPIC -shared -o libsum2.so
# gcc sum2.c -fPIC -shared -o libsum2_no-c.so
gcc main.c  ./libsum1.so ./libsum2.so
```

```
$ ./compile.sh
$ ./a.out
sum 1
f: 1, s: 2
addition: 3
sum 2
f: 1, s: 2
addition: 3
```

不加 -c 的时候没有任何报错信息，得到正常结果，第一 so 的 inner_sum 地址会覆盖第二个 so 的 inner_sum 地址。修改一下 compile.sh

```bash
# compile.sh
gcc -c sum1.c -fPIC -shared -o libsum1.so
# gcc sum1.c -fPIC -shared -o libsum1_no-c.so
gcc -c sum2.c -fPIC -shared -o libsum2.so
# gcc sum2.c -fPIC -shared -o libsum2_no-c.so
gcc main.c  ./libsum1.so ./libsum2.so
```

```
$ ./compile.sh
/usr/bin/ld: ./libsum2.so: in function `inner_sum':
sum2.c:(.text+0x0): multiple definition of `inner_sum'; ./libsum1.so:sum1.c:(.text+0x0): first defined here
collect2: error: ld returned 1 exit status
```

这时就会提示你编译错误，符号定义重复，我们尝试对比一下两个 so 有什么不同：

```bash
# compile.sh
gcc -c sum1.c -fPIC -shared -o libsum1.so
gcc sum1.c -fPIC -shared -o libsum1_no-c.so
gcc -c sum2.c -fPIC -shared -o libsum2.so
gcc sum2.c -fPIC -shared -o libsum2_no-c.so
gcc main.c  ./libsum1.so ./libsum2.so
```

```
$ ./compile.sh
/usr/bin/ld: ./libsum2.so: in function `inner_sum':
sum2.c:(.text+0x0): multiple definition of `inner_sum'; ./libsum1.so:sum1.c:(.text+0x0): first defined here
collect2: error: ld returned 1 exit status
```

```
$ readelf -sW libsum1.so

Symbol table '.symtab' contains 8 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS sum1.c
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 .text
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    5 .rodata
     4: 0000000000000000    24 FUNC    GLOBAL DEFAULT    1 inner_sum
     5: 0000000000000018   109 FUNC    GLOBAL DEFAULT    1 sum1
     6: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND puts
     7: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND printf
$ readelf -sW libsum1_no-c.so

Symbol table '.dynsym' contains 9 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND
     1: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_deregisterTMCloneTable
     2: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND puts@GLIBC_2.2.5 (2)
     3: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND printf@GLIBC_2.2.5 (2)
     4: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
     5: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_registerTMCloneTable
     6: 0000000000000000     0 FUNC    WEAK   DEFAULT  UND __cxa_finalize@GLIBC_2.2.5 (2)
     7: 0000000000001159    24 FUNC    GLOBAL DEFAULT   14 inner_sum
     8: 0000000000001171   109 FUNC    GLOBAL DEFAULT   14 sum1
     ...
```

可以看到 libsum1_no-c.so 中的 inner_sum 是有确定的地址的，而 libsum1.so 的 inner_sum 的地址是 0x0。个人认为当 gcc 没有 -c 选项时，产物是为了装载准备的；当加上 -c 选项后，产物是为了链接准备的，所以链接的时候会去找没有明确地址的符号，最后发现符号重定义。

- dynstr 被什么使用呢？
  答：是 dynsym 符号名称的索引，可以尝试读取一下 elf 文件来确定，这里不用通用的方式去尝试了，以一个 libsum1.so 为例：

```
$ readelf -SW libsum1.so
There are 30 section headers, starting at offset 0x35b0:

Section Headers:
  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            0000000000000000 000000 000000 00      0   0  0
  [ 1] .note.gnu.property NOTE            00000000000002a8 0002a8 000020 00   A  0   0  8
  [ 2] .note.gnu.build-id NOTE            00000000000002c8 0002c8 000024 00   A  0   0  4
  [ 3] .gnu.hash         GNU_HASH        00000000000002f0 0002f0 000028 00   A  4   0  8
  [ 4] .dynsym           DYNSYM          0000000000000318 000318 0000d8 18   A  5   1  8
  [ 5] .dynstr           STRTAB          00000000000003f0 0003f0 000086 00   A  0   0  1
  [ 6] .gnu.version      VERSYM          0000000000000476 000476 000012 02   A  4   0  2
  [ 7] .gnu.version_r    VERNEED         0000000000000488 000488 000020 00   A  5   1  8
  ...
$ readelf -h libsum1.so
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              DYN (Shared object file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          64 (bytes into file)
  Start of section headers:          13744 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           56 (bytes)
  Number of program headers:         11
  Size of section headers:           64 (bytes)
  Number of section headers:         30
  Section header string table index: 29

$ readelf -p .shstrtab libsum1.so

String dump of section '.shstrtab':
  [     1]  .symtab
  [     9]  .strtab
  [    11]  .shstrtab
  [    1b]  .note.gnu.property
  [    2e]  .note.gnu.build-id
  [    41]  .gnu.hash
  [    4b]  .dynsym
  [    53]  .dynstr
  [    5b]  .gnu.version
  [    68]  .gnu.version_r
  ...

$ readelf -p .dynstr libsum1.so

String dump of section '.dynstr':
  [     1]  __gmon_start__
  [    10]  _ITM_deregisterTMCloneTable
  [    2c]  _ITM_registerTMCloneTable
  [    46]  __cxa_finalize
  [    55]  inner_sum
  [    5f]  sum1
  [    64]  puts
  [    69]  printf
  [    70]  libc.so.6
  [    7a]  GLIBC_2.2.5
```

```c
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
int read_elf_file(const char *path) {
  FILE *fp = fopen(path, "r");
  if ((unsigned long)fp == (unsigned long)-1) {
    return 0;
  }
  char *buf = (char *)malloc(1024 * 50);
  fread(buf, sizeof(Elf64_Ehdr), 1, fp);
  Elf64_Ehdr *pdr = (Elf64_Ehdr *)buf;
  uint64_t section_header_offset = pdr->e_shoff;
  printf("section_header_offset = %lu\n", section_header_offset);
  fseek(fp, section_header_offset + 4 * sizeof(Elf64_Shdr), SEEK_SET);

  fread(buf, sizeof(Elf64_Shdr), 1, fp);
  Elf64_Shdr *shdrp = (Elf64_Shdr *)buf;
  // check in .shstrtab
  printf("section item name = %x\n", shdrp->sh_name);
  printf("section item offset = %lu\n", shdrp->sh_offset);

  fseek(fp, shdrp->sh_offset + sizeof(Elf64_Sym) * 7, SEEK_SET);
  fread(buf, sizeof(Elf64_Sym), 1, fp);
  Elf64_Sym *symp = (Elf64_Sym *)buf;
  printf("dynsym section item idx = %u\n", symp->st_shndx);
  printf("dynsym section item name = %x\n", symp->st_name);

  fclose(fp);

  return 1;
}
int main() {
  if (!read_elf_file("./libsum1.so")) {
    printf("Read elf file failed\n");
    return 1;
  }
  return 0;
}
```

```
$ gcc main.c && ./a.out
section_header_offset = 13744
section item name = 4b
section item offset = 792
dynsym section item idx = 14
dynsym section item name = 55
```

可以看到 item name 的数据就是 .dynstr 中 0x55 的数据 inner_sum。

- 如何查看 gnu.hash 段
  答：这个 gnu.hash 在链接阶段生成，可以通过指定 --hash-style 来生成

```
$ gcc -Wl,--hash-style="both" main.c
```

关于 gnu.hash 原理可以参考[这里](https://www.bilibili.com/read/cv15286255/)和[这里](https://www.entry0.cn/2020/05/05/755.html)

- gcc -shared 生成动态库时，printf 和 sleep 函数仍然在 .rel.plt 中，看起来仍然是设置了 PIC
答：<font color='red'>仍不清楚原因</font>
- C 进程初始化结构是什么样的？
  答：https://refspecs.linuxfoundation.org/ELF/zSeries/lzsabi0_zSeries/x895.html
- linux 在执行一个依赖 so 文件的可执行文件时，动态链接器被第一个执行，且被操作系统赋予修改控制进程的权限，那么，动态链接器运行在其他进程嘛？动态链接器为什么不能在使用内部函数之前先修改进程的 GOT/PLT 信息达到可以调用的目的呢？（有机会还是学习一下 Linkers and Loaders 和 CSAPP 这两本书，自我修养很多地方没有讲述清楚）
- RTLD_NOW 为什么会使得动态链接库加载速度变慢
答：因为 RTLD_NOW 被指定时，需要在 dlopen 返回之前，将动态链接库中的符号全部解析完成。

## 第八章

- 动态链接过程中，系统如何识别到动态链接库的版本更新了
答：使用 gcc -Wl,--version-script xxx.ver 指定编译出的动态链接库版本信息后，带上动态链接库编译产物得到的 a.out，其以来的外部符号会带上一个版本号：
```
$ readelf -sW a.out

Symbol table '.dynsym' contains 8 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND __libc_start_main@GLIBC_2.34 (2)
     2: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_deregisterTMCloneTable
     3: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND sum@VERS_1.1 (3)
     4: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
     5: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_registerTMCloneTable
     6: 0000000000000000     0 FUNC    GLOBAL DEFAULT  UND sleep@GLIBC_2.2.5 (4)
     7: 0000000000000000     0 FUNC    WEAK   DEFAULT  UND __cxa_finalize@GLIBC_2.2.5 (4)
     ...
```
另外查看动态链接库的符号也能看到版本信息：
```
c$ readelf -sW sum.so

Symbol table '.dynsym' contains 7 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __cxa_finalize
     2: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_registerTMCloneTable
     3: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND _ITM_deregisterTMCloneTable
     4: 0000000000000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
     5: 0000000000000000     0 OBJECT  GLOBAL DEFAULT  ABS VERS_1.1
     6: 00000000000010f9    24 FUNC    GLOBAL DEFAULT   12 sum@@VERS_1.1
    ...
```
<font color='red'>有兴趣可以尝试修改一下 so 或 a.out 文件，验证一下是否是通过这两个字段进行的匹配</font>

## 第九章

- 可以通过 malloc 检测进程分配的堆空间，但是通过 maps 发现 heap 标记的空间和堆空间分配的空间大小不符，这是为什么？maps 中的 heap 空间表示什么？
答：在做完 mini-crt 之后就会了解到，在 malloc 进行分配之前，_start 相关链路逻辑就已经开辟好了一块 heap 空间，malloc 使用这块空间来回应调用方的申请。如果 _start 未指定 heap，则进程运行的时候甚至没有 heap 空间；而自己编写 _start 时，每次调用 brk/sbrk 增加的 heap 大小和操作系统有关，本机上是 4096bytes，不满 4096*n 则向上取整

## 第十一章

- glibc 2.34 之前，**libc_start_main 调用 **libc_csu_init，2.34 之后用什么函数替代了**libc_csu_init？
  答：参考 [stackoverflow](https://stackoverflow.com/a/69837439/14419237)，使用了 call_init 代替了 **libc_csu_init，作用都是查询动态链接库的 INIT and INIT_ARRAY 表，找到对应的 init 函数并执行
- C++ 在定义了全局变量后为什么没有找到 .ctor 段？
  答：c++ 中初始化全局变量的 .ctor 段在 2010 年后由 .init_array 和 .fini_array 代替，参考这里：[GCC Bugzilla](https://gcc.gnu.org/bugzilla/show_bug.cgi?id=46770)；以下代码举例：

```c++
// main.cpp
#include <string>
class Student {
public:
  std::string name;
  int id;
  Student() {
    name = "tmp";
    id = 1;
  }
};

Student s;

int main() {
  s.name = "asdf";
  return s.id;
}
```

查看 a.out 符号，可以找到一个 \_GLOBAL**sub_I_s 符号，这个符号的功能应该与 GLOBAL**I_Hw 一样

```
$ g++ --version
g++ (Ubuntu 11.3.0-1ubuntu1~22.04.1) 11.3.0
...
$ readelf -sW a.out
...
  13: 0000000000001248    25 FUNC    LOCAL  DEFAULT   16 _GLOBAL__sub_I_s
...
```

书中说，如果存在 GLOBAL\_\_I_Hw 这个 symbol，那么会有一个 .ctor 的 section 存放这个 symbol，但是在 g++ 11 里面并没有这个东西，取而代之的是 .init_array。

```
$ readelf -SW a.out
...
  [22] .init_array       INIT_ARRAY      0000000000003d70 002d70 000010 08  WA  0   0  8
  [23] .fini_array       FINI_ARRAY      0000000000003d80 002d80 000008 08  WA  0   0  8
...
```

根据之前的 GNU Bug 链接可以找到，书中描述的 .ctor 已经被 .init_array 和 .fini_array 取代；我们直接来看看 .init_array：

```
$ readelf -x .init_array a.out

Hex dump of section '.init_array':
  0x00003d70 c0110000 00000000 48120000 00000000 ........H.......
```

发现地址对应的值有两个：0x11c0 以及 0x1248，而 0x1248 正好是 \_GLOBAL\_\_sub_I_s 的地址。同时可以尝试以下实验：

```c
#include <stdio.h>

typedef void (*ctor_t)(void);

void before_main() { printf("before main\n"); }

ctor_t __attribute__((section(".init_array"))) my_init = &before_main;

int main() {
  printf("in main\n");
  return 0;
}
```

编译运行后输出：

```
before main
in main
```

- MSVC 下，file 默认缓冲设置是什么（不设置 setbuf 或者 setvbuf 的情况下）
- setvbuf 在 mode 是 \_IONBF 的情况下是否可以为 NULL？

## 第十二章

- 如何使用 man 手册，里面都存储了什么 doc
答：manpath 可以查看
- regparm(0) 设置函数只能从栈上获取参数，是否可以拿来做通用 so 内函数调用的方法
答：应该不行的，除非知道 so 内部函数的调用方式
- 如何将 /proc/PID/mem 中的数据提取出来？

## Extra
- C BNF
- C++ BNF