# Qt 6 中文教程：从 C++ 基础到桌面项目实战

一套可以离线阅读、配套编译、逐章练习的 Qt 6 学习项目。以 **C++17 + Qt Widgets** 为主线，包含 **Qt Quick/QML 与 C++ 交互**，最终完成使用 SQLite 保存数据的 **TaskDesk 任务管理器**。

适合刚接触 Qt、希望逐步独立开发桌面软件的学习者。没有 C++ 基础时先读第 01 章，再完成第 04、05 章补课；第 02 章讲解 Kit、编辑器、构建与断点。课程包含 **67 章中文教程、18 个源码示例（Vulkan 为可选构建）、1 个综合项目和 Qt Test 自动测试**。串口、TCP、音视频与国际化提供选修路线，不冒充已经实现的完整硬件案例。

## 从这里开始

- **离线网页**：双击 [开始学习.html](开始学习.html)，或打开 [课程首页](site/index.html)。无需联网、无需启动网页服务器，支持正文搜索和前后章翻页。
- **GitHub 在线阅读**：[完整章节目录](docs/00-目录.md)。GitHub 仓库页面通常不会直接执行 HTML，请在线读 Markdown，或下载后打开网页。
- **先运行示例**：按下方步骤安装环境、构建，然后启动 `01_hello`。

- **逐步实操**：[21 个详细实验](docs/WORKBOOK.md)，覆盖搭建新目标、取消保存、线程退出、数据库事务、QML 绑定和部署验收。

## 新手推荐路线

按 [八阶段目录](docs/00-目录.md) 学习，章节已按学习顺序连续编号为 00～66，目录、文件名和离线页面保持一致。先读预备课与工具入门，再学对象、控件和布局，随后学习文件与模型、异步与通信、QML，最后进入测试和项目扩展。每章先运行，再修改，最后验证正常、边界和失败路径。

示例目标名（如 `01_hello`、`14_controls`）使用独立编号，运行命令按示例名称使用；它们不代表章节号。

新增章节覆盖常用控件、多窗口、QSettings、焦点和事件过滤、TCP 拆包、模型委托、异步状态、SQLite 迁移、QML 组件、交互测试、架构、性能、日志与发布，并附术语答疑。高级章节中的设计片段会明确前提，不代表现有 TaskDesk 已实现全部扩展。

## Qt Creator 图形化操作

新增 [第 59～66 章操作实训](docs/59.md)，以本机 `D:\Qt\Tools\QtCreator\bin\qtcreator.exe`（20.0.2）为版本参考，覆盖工作区、Kit、打开和新建 CMake 工程、Designer 拖拽、属性、布局、Tab/buddy、信号槽、CDB 断点调试和运行设置。配套三项综合实验、16 张本机真实截图和 16 幅教学示意图。每幅主要配图说明看哪里、怎样操作、结果与检查，并区分当前默认值与课程目标值。

初学者可在第 02 章之后提前学习，不用等学完 OpenGL/Vulkan。项目 Qt SDK 仍是课程的 6.8.3，Creator 自身的 Qt 版本不等于项目 Kit。

## 按函数深入阅读

每个主要接口增加了参数、作用与返回结果、调用时机和易错点。可以直接在相关正文查阅，或使用 [164 组主要接口详解](docs/API.md)。这里讲解课程使用的接口和部分对照接口，不代表覆盖整个 Qt 的所有模块与全部重载。

第 49～58 章详解 Qt 与 OpenGL/Vulkan：渲染管线、上下文、shader、VAO/VBO、矩阵和纹理、QVulkanWindow、render pass、Vulkan pipeline、显存、描述符和同步。OpenGL 提供完整彩色三角形，Vulkan 提供完整清屏；后续三角形与纹理是明确标注的扩展实验。

## 环境基线

| 组件 | 课程配置 |
|---|---|
| 系统 | Windows x64 |
| Qt | **6.8.3 / msvc2022_64**，固定版本便于重现 |
| 编译器 | Visual Studio 2022 / Build Tools，MSVC v143 x64 |
| C++ | C++17 |
| 构建 | CMake 3.21+，Visual Studio 17 2022 生成器 |
| 安装辅助 | Python 3.10+，aqtinstall 3.3.0（项目独立环境） |
| 编辑器 | Qt Creator、Visual Studio、VS Code 或文本编辑器均可 |

**先决条件**：安装 VS 2022 的“使用 C++ 的桌面开发”、MSVC 和 Windows SDK；安装 CMake 并加入 PATH；安装 Python 并确认 `python --version` 可用。Qt Creator 是可选编辑器，不随本仓库脚本安装。

SDK 下载模块：`qtbase`、`qtdeclarative`、`qttools`、`qtsvg`、`qttranslations`，覆盖 Core/Gui/Widgets/Network/Sql/Test/Quick/QuickControls2、Designer 与部署工具。SDK、编译器和运行程序必须使用同一架构与兼容工具链。项目使用 `find_package(Qt6)` 配置依赖。

## 安装 Qt 6 到项目内

在项目根目录打开 PowerShell：

```powershell
powershell -ExecutionPolicy Bypass -File scripts/bootstrap-qt.ps1
```

脚本读取 [qt-version.json](qt-version.json)，将安装工具放入 `.tools/`，Qt 放入 `sdk/6.8.3/msvc2022_64/`。首次安装需要网络和数 GB 可用空间；完成后可以离线学习和编译。使用第三方开源工具 aqtinstall 获取 Qt 分发包，不要求把账号或密钥写进仓库。网络失败可修复连接后重试。

也可以使用 Qt 官方安装器安装同版本的 MSVC x64 SDK，再在当前终端设置 `QT_ROOT` 为 SDK 根目录。根目录应同时包含 bin、include、lib、plugins，而不是只指向 bin：

```powershell
$env:QT_ROOT = Read-Host '请输入 Qt 6 SDK 根目录'
scripts/build.cmd
```

脚本只修改当前进程环境，不永久覆盖系统 PATH。更多配置解释见 [环境安装章节](docs/03.md)。

## 一键构建、测试与运行

```powershell
scripts/build.cmd
scripts/run.cmd 01_hello
scripts/run.cmd 02_counter
scripts/run.cmd taskdesk
scripts/run.cmd 13_quick
```

也可以双击根目录 `build.cmd`，完成后会保留窗口供查看结果。构建脚本自动查找 VS 2022 C++ 工具链，使用项目内 Qt 配置全部目标，编译 Release 并运行测试。运行脚本临时设置 DLL、插件、QML 路径。

手动构建（在 VS 2022 x64 开发者终端中）：

```powershell
cmake --preset windows-msvc
cmake --build --preset release
ctest --preset release
```

Qt Creator 用户打开根目录 `CMakeLists.txt`，选择 SDK 中的 qmake 与 MSVC x64 Kit，构建目录使用 `build-creator`。不要把 MSVC Qt 与 MinGW 编译器混用。

## 示例索引

| 目标 | 内容 | 重点 |
|---|---|---|
| 01_hello | 第一个窗口 | QApplication、事件循环 |
| 02_counter | 计数器 | 自定义信号、lambda、AUTOMOC |
| 03_form | 录入表单 | 布局、验证、反馈 |
| 04_editor | 文本编辑器 | 菜单、UTF-8、安全保存、关闭确认 |
| 05_json | JSON 读写 | 临时目录、序列化与解析 |
| 06_model | 可编辑表格 | 自定义模型、排序、过滤 |
| 07_paint | 动态仪表 | QPainter、缩放、定时器 |
| 08_worker | 后台任务 | Worker、进度、取消与退出 |
| 09_http | HTTP 客户端 | 异步请求、超时、错误 |
| 10_sql | SQLite 入门 | 建表、绑定参数、查询 |
| 11_designer | 可视化表单 | .ui、AUTOUIC |
| 12_resources | 资源与样式 | .qrc、AUTORCC、QSS |
| 13_quick | QML + C++ | 属性、NOTIFY、QML_ELEMENT |
| 14_controls | 常用控件与同步 | 输入、选择、QSignalBlocker |
| 15_settings | 配置保存与恢复 | QSettings、用户路径、默认值 |
| 16_tcp | 本机 TCP 回显 | 异步 socket、拆帧、缓冲上限 |
| 17_opengl | OpenGL 3.3 Core 三角形 | shader、VAO、VBO、像素读回自检 |
| 18_vulkan | Vulkan 清屏（可选） | instance、设备、swapchain、命令录制 |
| taskdesk | 综合任务管理器 | SQLite、搜索、主键操作、持久化 |

网络例子默认请求本地测试服务，在另一个终端运行：

```powershell
python -m http.server 8765 --bind 127.0.0.1 --directory examples/09_http
```

TaskDesk 数据保存在 `QStandardPaths::AppDataLocation` 对应用户目录，应用标识为 `QtLearning/TaskDesk`；窗口位置由 QSettings 保存。它不会把真实任务数据写进仓库。示例 05_json 使用临时文件，10_sql 使用内存数据库，退出后不保留。

## 课程内容

C++ 必要基础 → 工具链与 CMake → QObject 与信号槽 → Widgets 布局和表单 → Designer → 文件/JSON/设置/资源 → Model/View → 事件和绘图 → 多线程 → 网络 → SQLite → QML 与 C++ → 自动测试 → 调试与部署 → 综合项目扩展。

每章包含概念、操作或源码分析、练习及验收要点。建议用 12 周完成主线，零基础可放慢预备课和控件阶段的进度，每天 60~90 分钟。[查看全部 67 章](docs/00-目录.md) · [官方参考](docs/REFERENCES.md)。

## 验证与分发

```powershell
ctest --test-dir build -C Release --output-on-failure
powershell -ExecutionPolicy Bypass -File scripts/check.ps1
scripts/deploy.cmd 01_hello
scripts/deploy.cmd taskdesk
```

测试覆盖标题边界、引号、CRUD、磁盘重开与异步信号，以及 TCP 半包、粘包、UTF-8 分段、帧长边界和真实回环往返。check 脚本对全部 17 个应用执行自动退出启动检查；它不能替代真实交互验收。deploy 生成 `dist/<目标>/` 并调用本项目 Qt 的 windeployqt。分发目录包含 MSVC 运行库安装程序 `vc_redist.x64.exe`，目标机器缺少运行库时先安装它。分发前仍需在干净 Windows 环境验证，并核对第三方许可。

## 项目结构

```text
docs/                    中文 Markdown 课程、参考与验证记录
site/                    无外部依赖的离线网页
examples/                18 个配套源码示例
projects/taskdesk/       综合项目与可测试仓储
tests/                   Qt Test 测试
scripts/                 安装、构建、运行、部署和检查脚本
.github/workflows/       GitHub Windows 自动构建配置
qt-version.json          SDK 与安装工具的固定版本
CMakePresets.json        可移植构建预设
sdk/                     本地 Qt SDK（忽略）
.tools/                  本地安装工具（忽略）
build/                   编译输出（忽略）
dist/                    应用分发包（忽略）
```

## 仓库维护与持续集成

仓库包含 MIT License、第三方依赖说明、忽略规则、贡献说明、发布清单和 Windows CI 工作流。测试基线见 [验证记录](docs/VALIDATION.md)，仓库发布与维护步骤见 [PUBLISHING.md](PUBLISHING.md)。GitHub Actions 的执行结果以仓库中的实际运行记录为准。

课程的验证平台是 Windows x64/MSVC。Linux/macOS 需要另外配置工具链并完成验证。

## 更新离线教程

Markdown 为编辑源，网页从它生成：

```powershell
.tools\Scripts\python.exe -m pip install -r tools/requirements-docs.txt
.tools\Scripts\python.exe tools/render_site.py
.tools\Scripts\python.exe tools/verify_docs.py
```

阶段顺序定义在 tools/course.json，首页、侧栏、搜索及前后翻页使用同一份路线。生成器不会改写 Markdown 源文件。配置示例只在点击保存时写入偏好；--smoke 使用临时目录。TCP 示例只监听本机，不依赖外网或额外 Qt 模块。

## 许可

原创文字、网页和示例源码使用 [MIT License](LICENSE)。Qt 与安装工具保持其自身许可；[第三方说明](THIRD_PARTY_NOTICES.md) 和 [Qt 官方许可说明](https://doc.qt.io/qt-6.8/licensing.html) 应一并阅读。

## 图形扩展的构建与检查

普通 build.cmd 默认编译 OpenGL 示例；Vulkan 需提供头文件并显式启用。获取方式见 [第 54 章](docs/54.md)，然后执行：

```powershell
scripts\build-graphics.cmd
scripts\run.cmd 17_opengl
scripts\run.cmd 18_vulkan
powershell -ExecutionPolicy Bypass -File scripts/check-graphics.ps1
```

图形检查需要正常 Windows 图形会话和合适的驱动，独立于 check.ps1 的 offscreen 启动检查。Vulkan 的 --validation 还需要实际安装验证层。头文件、运行时、驱动和 SDK 工具是不同组件。
