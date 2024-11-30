# 如何在 Cpp 中初始化 NodeJS 环境

## Android

### 打包

因为 nodejs 不保证对 Android 的支持<a href="#r1"><sup>[1]</sup></a>，打包工作必须要在以下环境中进行（这是其中一种验证过的，可行的打包环境）<a href="#r2"><sup>[2]</sup></a>：

- 编译环境：Ubuntu 20.04（非必要）
- Node.js 版本：目前最新的 LTS v14.16.0
- NDK 版本：21.0.6113669
- Python 版本：3.8.5（非必要，但受限于 NodeJS 项目打包依赖的版本）

# Reference

1. <a id="r1" href="https://github.com/nodejs/node/blob/main/BUILDING.md#:~:text=Android%20is%20not%20a%20supported%20platform.">node/BUILDING.md -- Github.com</a>
2. <a id="r2"  href="https://toyobayashi.github.io/2021/03/29/NodeEmbedding">在 Android 应用中嵌入 Node.js</a>
