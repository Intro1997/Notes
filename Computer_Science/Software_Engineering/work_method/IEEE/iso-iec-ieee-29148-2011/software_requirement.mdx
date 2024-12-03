本笔记概述论文中 8.4 Software requirements specification document 章节的内容

# 上下文的词语含义

1. System：system 表示上层视角（高层领域）中的一种运行模式，而我们的软件产品最终需要解决这个领域的某些需求。例如 ATM 机搭载的软件解决了整个金融交易系统（抽象系统，不一定是通过软件来实现的系统）中，客户存款取款查询的需求。如果使用“系统”这个中文词汇，则表示一般情况下的系统。
2. SRS：Software requirements specification

# 概述

## SyRS 和 SRS 之间的关系和区别

SyRS（System Requirement Specification）是对整个系统的需求进行规范化和详细描述的文档。它定义了系统所需的功能、性能、约束、接口和其他非功能性需求。SyRS 关注的是整个系统的需求，包括硬件、软件、外部接口、人机交互等方面。SyRS 描述了系统的总体目标、范围和功能，以及与系统交互的外部实体和系统的约束条件。

SRS（Software Requirement Specification）是基于 SyRS 的基础上，进一步详细定义和规范软件的需求。它关注的是软件系统的需求，包括软件的功能、性能、接口、数据管理等方面。SRS 详细描述了软件的行为、输入输出、数据结构、算法等，并为软件开发团队提供了指导。

系统（System）在这里指的是整个软件系统的上下文，包括软件本身、与之交互的硬件设备、外部接口、用户等。系统需要满足用户和利益相关者的要求，因此系统需求规范（SyRS）涵盖了整个系统的需求，不仅仅局限于软件本身。

软件（Software）是系统的一个组成部分，它实现了系统的功能和逻辑。软件需求规范（SRS）则更加专注于描述软件系统的需求，包括软件的功能、性能、接口等方面。SRS 提供了开发团队所需的指导，以确保软件的设计和实现满足用户的期望。

本笔记主要聚焦于 SRS。

## 软件需求的大体内容

1. 需求分析目标：使得需求对项目相关人员来说是易于理解的
2. 软件需求开发的参考框架：
   - Introduction
     - Purpose
     - Scope
     - Product overview
       - Product perspective
       - Product functions
       - User characteristics
       - Limitations
     - Definitions
   - References
   - Specific requirements
     - External interfaces
     - Functions
     - Usability Requirements
     - Performance requirements
     - Logical database requirements
     - Design constraints
     - Software system attributes
     - Supporting information
   - Verification
     (parallel to subsections in Section 3)
   - Appendices
     - Assumptions and dependencies
     - Acronyms and abbreviations

上述框架的具体含义参见 9.5 Software requirements specification (SRS) document，总结性的解析会放在“软件需求框架和含义”这一节中，为后续其他项目的需求分析做指引。

3. 启发项目需求构建的思路：

   - system 模式：使用有限状态机或状态图会更有帮助
   - 对象：面向对象分析会有所帮助
   - 刺激系统<sup><a href="#r1">[1]</a></sup>：stimulus-response sequences 或许会有帮助
   - 功能层次结构：数据流图和数据字典会有一定程度的帮助
   - system 用户类型：例如电梯的使用方按照用户类型可以划分为乘客、维护工人以及消防员
   - 功能：可以从 system 需要的功能出发来挖掘 system 的需求
   - 响应：可以通过 system 如何响应外部的请求来挖掘 system 的需求
   - **Note**: 多数情况下，功能组织可能是最差的选择。

# 软件需求文档

### 1. Purpose（目的）：

描述此产品开发的目的

### 2. Scope（项目概述）

这里需要阐述以下内容：

1. 项目名称
2. 项目的存在的意义（项目做了什么？）
3. 项目的价值
4. 项目实施的标准（如果有父级项目的话，需要使项目间的标准一致）

### 3. Product Perspective（产品概述）

1. 如果开发的产品是一个大型软件 system 的子模块：
   - 说明子模块在大型软件 system 中起到的作用（大型软件 system 的需求和子模块的功能之间的关系）
   - 说明子模块需要暴露给大型软件 system 的接口
2. 说明所开发的产品在以下几个层面中是如何运行的：
   - System interfaces：描述软件与所处 system 之间交互的接口，例如：软件是如何解决系统与人交互的接口？（键鼠？触控？）
   - User interfaces：阐述用户与软件之间的交互逻辑，主要是针对窗口界面的 UI 交互。
   - Hardware interfaces：阐述软件和硬件之间的逻辑关系，例如：软件是如何被硬件支持的，协议是什么？使用了什么指令集
   - Software interfaces：阐述产品与软件之间的关系，例如：产品基于什么操作系统，使用了什么样的数学库？和其他软件之间是否有交互接口？
     - 对于用到的软件/库，应明确说明软件/库的以下信息：
       - 名称
       - 助记符（Mnemonic）
       - 规格编码（Specification number）
       - 版本号
       - 软件来源
     - 对于和其他软件有交互的接口，应明确说明以下信息：
       - 描述与其他软件进行交互的接口，它的作用和目的
       - 在消息内容和格式方面定义接口（给出接口描述）
   - Communications interfaces（通信层面）：阐述通信方面的需求，例如：本地网络协议
   - Memory constrain：阐述主存和辅存的限制
   - Operations： 阐述用户的正常和特殊交互/操作需求：
     - 用户群体的多种交互/操作模式；例如滑动页面可以使用鼠标滚轮，手指滑动（触屏），拖动滚动条或者键盘的上下键，或者语音控制（无障碍）
     - 交互模式和无人值守模式；例如 [AnyViewer 的远程控制](https://www.anyviewer.com/tw/how-to/anydesk-allow-unattended-access.html)
     - 数据处理支持功能（Data processing support functions，没有找到具体的功能是什么）
     - 备份和恢复操作
   - Site adaptation requirements（站点适应需求）：阐述一些软件需要特殊适配的需求（没有特别理解，感觉接近部署软件的机器提出的特殊需求，例如服务器软件）：
     - 定义特定于给定站点的任何数据或初始化序列的要求，任务或操作模式（例如，网格值、安全限制等）；
     - 应修改站点或任务相关功能的规范以使软件适应特殊安装。

### 4. Product Function

阐述软件执行的主要功能

### 5. User characteristics

描述用户群体的特征，例如教育背景，经历，是否残疾，以及技术专长。这些信息与接下来 “Specific requirements” 中定义的内容息息相关。

### 6. Limitations

这里描述限制供应商选择使用你提供的产品的理由，一般有以下因素：

- 监管政策
- 硬件限制（例如信号时序要求）
- 与其他应用程序的接口
- 并行操作（Parallel operation）
- 审计职能
- 控制功能
- 高阶语言要求
- 信号握手协议（例如，XON-XOFF、ACK-NACK）
- 质量要求（例如可靠性）
- 应用程序的关键性
- 安全和保安考虑
- 身体/精神方面的考虑

### 7. Assumptions and dependencies

此部分用于列举项目中的假设和依赖项，以帮助开发团队和利益相关者理解系统开发和实施的前提条件。下面是一些可能包含在该部分的信息示例：

1. 假设（Assumptions）：

   - 假设特定硬件设备或操作系统已经安装和配置。
   - 假设网络连接是可靠和稳定的。
   - 假设第三方组件或库已经获得许可并可以集成到系统中。
   - 假设用户已经获得适当的培训和说明，以正确使用系统。

2. 依赖项（Dependencies）：
   - 依赖于特定的数据库管理系统（如 MySQL 或 Oracle）。
   - 依赖于第三方 API 或库（如 Google Maps API 或 OpenCV 库）。
   - 依赖于外部系统或服务（如支付网关或短信服务提供商）。
   - 依赖于特定的硬件设备（如传感器或打印机）。

### 8. Apportioning of requirements（需求分配）

这里描述整个软件 system 的需求分配过程，建议用子模块负责部分需求的方式划分。同时也要考虑未来可能作为增量添加的功能。

### 9. Specific requirements

这里需要细化软件软件需求，因为：

1. 方便设计师（不论是 UI 还是代码）对软件进行设计
2. 方便在开发中进行测试

需求的详细程度视情况而定，但至少需要给出输入的数据以及软件对输入的响应（输出数据）。除此之外，软件应该符合三个要求：

1. 符合 “Scope（项目概述）” 中的所有标准
2. 保证每一个请求是独立且清晰的
3. 若提及与前文相关的需求，请贴出引用

### 10 External interfaces（对外接口）

这里包含对接口的定义，包括输入输出。这里同时应该涵盖从 “System interfaces” 到 “Communications interfaces” 的所有接口。
每个接口的定义都应该包含以下信息：

- Name of item;
- Description of purpose;
- Source of input or destination of output;
- Valid range, accuracy（精度）, and/or tolerance（容差）;
- Units of measure;（计量单位）
- Timing;
- Relationships to other inputs/outputs;
- Screen formats/organization;（这里不太明白，是说分辨率？）
- Window formats/organization;（这里不太明白，是说窗口 UI ？）
- Data formats;
- Command formats;（不太明白，指的是汇编语言的命令格式？）
- Endmessages.

对于不同需求可以酌情选择

### 11 Function（函数）

这里定义软件在经历输入、处理、输出期间的所有行为，包括：

- Validity checks on the inputs
- Exact sequence of operations（明确操作顺序）
- Responses to abnormal situations, including
  - Overflow
  - Communication facilities
  - Error handling and recovery
- Effect of parameters（直译为参数的影响，没太理解）
- Relationship of outputs to inputs, including
  - Input/output sequences
  - Formulas for input to output conversion（输入到输出转换的公式）

> Note: It may be appropriate to partition the functional requirements into subfunctions or subprocesses. This does
> not imply that the software design will also be partitioned that way.

### 12 Usability requirements（可用性需求）

这里描述 system 的可用性，包括：

- 可衡量的功能有效性
- 可衡量的软件执行效率
- 特定使用场景下，可衡量的满意度标准

### 13 Performance requirements（性能需求）

使用可量化的数据指出软件需要达到的性能指标。这里不排斥是用动态变化的数字，例如事务和任务的数量并非恒定，一段时间内的需要处理的数据总量也不是恒定的。
以下是一个性能指标的例子：

> 95 % of the transactions shall be processed in less than 1 second.

### 14 Logical database requirements

对需要放入数据库的任何信息，基于以下考虑定义需求：

- Types of information used by various functions;
- Frequency of use;
- Accessing capabilities;（访问能力）
- Data entities and their relationships;
- Integrity constraints;（完整性约束）
- Data retention requirements.（数据保留要求）

### 15 Design constraints

通过指定外部标准、法规要求或项目限制对 system 设计施加的约束。

### 16 Standards compliance（遵循标准）

通过已有的标准和规范来驱动需求产生，包括：

- Report format;
- Data naming;
- Accounting procedures;（会计程序）
- Audit tracing.（审计追踪）

根据这些标准来指定需求，有助于特定软件对处理活动进行跟踪的要求，某些应用程序需要此类跟踪才能满足最低监管或财务标准。 例如，审计跟踪可能要求对工资数据库的所有更改，都应记录在跟踪文件中，并包含更改前后的值。

### 17 Software system attributes（软件系统属性）

阐述软件产品所需要的属性：

1. 可靠性：描述软件在特定情况下，依旧能够保持正常运行的能力。可靠性强调系统在长时间运行中的稳定性和一致性，以及对错误和故障的处理能力。
2. 可用性（Availability）：根据 system 指定的可用性级别，列举保证可用性所需要的因素，例如检查点，恢复和重启。可用性指的是软件系统在用户需要时可供其使用的程度。
3. 安全性： 指定保护软件免遭意外或恶意访问，修改、销毁或披露的需求。该领域的具体要求可能包括：
   - 利用一定的密码技术；
   - 保留特定的日志或历史数据集；
   - 将某些功能分配给不同的模块；
   - 限制程序某些区域之间的通信；
   - 检查关键变量的数据完整性；
   - 确保数据隐私
4. 可维护性：包括对持续优化项，可能发生变动的需求；包括功能模块，接口的复杂性限制的要求（这里参考《代码大全》）
5. 可移植性：将软件迁移到其他 system 的难易程度，包括：
   - Percentage of elements with host-dependent code;
   - Percentage of code that is host dependent;
   - Use of a proven（经过验证的） portable language;
   - Use of a particular compiler or language subset;
   - Use of a particular operating system.

### 18 Verification

提供一些可量化软件 system 的方法，建议通过参考 “External interfaces” 到 “Software system attributes” 来给出指标。

### 19 Supporting information

SRS 应包含额外的支持信息，包括

- 输入/输出格式样本、成本分析研究描述或用户调查结果；
- 可以帮助 SRS 读者的支持或背景信息；
- 软件要解决的问题的描述；
- 代码和介质的特殊包装说明，以满足安全、出口、初始装载或其他要求。

SRS 应明确说明这些信息项是否被视为要求的一部分。

# Reference

1. <a id="r1" href="https://www.cs.ubc.ca/labs/isd/FormalWare/abstracts/tr97-001.pdf">Stimulus Response Requirements Specification Technique</a>
