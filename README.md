# Qt 6 中文教程：从 C++ 基础到桌面项目实战

一套可以离线阅读、配套编译、逐章练习的 Qt 6 学习项目。以 **C++17 + Qt Widgets** 为主线，包含 **Qt Quick/QML 与 C++ 交互**，最终完成使用 SQLite 保存数据的 **TaskDesk 任务管理器**。

适合有少量 C++ 基础、希望独立开发桌面软件的学习者。刚开始接触 C++ 的读者可以先完成第 02、03 章补课。课程包含 **33 章中文教程、13 个示例、1 个综合项目和 Qt Test 自动测试**。串口、TCP、音视频与国际化提供选修路线，不冒充已经实现的完整硬件案例。

## 从这里开始

- **离线网页**：双击 [开始学习.html](开始学习.html)，或打开 [课程首页](site/index.html)。无需联网、无需启动网页服务器，支持正文搜索和前后章翻页。
- **GitHub 在线阅读**：[完整章节目录](docs/00-目录.md)。GitHub 仓库页面通常不会直接执行 HTML，请在线读 Markdown，或下载后打开网页。
- **先运行示例**：按下方步骤安装环境、构建，然后启动 `01_hello`。

- **逐步实操**：[12 个详细实验](docs/WORKBOOK.md)，覆盖搭建新目标、取消保存、线程退出、数据库事务、QML 绑定和部署验收。

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

脚本只修改当前进程环境，不永久覆盖系统 PATH。更多配置解释见 [环境安装章节](docs/01.md)。

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
| taskdesk | 综合任务管理器 | SQLite、搜索、主键操作、持久化 |

网络例子默认请求本地测试服务，在另一个终端运行：

```powershell
python -m http.server 8765 --bind 127.0.0.1 --directory examples/09_http
```

TaskDesk 数据保存在 `QStandardPaths::AppDataLocation` 对应用户目录，应用标识为 `QtLearning/TaskDesk`；窗口位置由 QSettings 保存。它不会把真实任务数据写进仓库。示例 05_json 使用临时文件，10_sql 使用内存数据库，退出后不保留。

## 课程内容

C++ 必要基础 → 工具链与 CMake → QObject 与信号槽 → Widgets 布局和表单 → Designer → 文件/JSON/设置/资源 → Model/View → 事件和绘图 → 多线程 → 网络 → SQLite → QML 与 C++ → 自动测试 → 调试与部署 → 综合项目扩展。

每章包含概念、操作或源码分析、练习及验收要点。建议用 12 周完成主线，每天 60~90 分钟。[查看全部 33 章](docs/00-目录.md) · [官方参考](docs/REFERENCES.md)。

## 验证与分发

```powershell
ctest --test-dir build -C Release --output-on-failure
powershell -ExecutionPolicy Bypass -File scripts/check.ps1
scripts/deploy.cmd 01_hello
scripts/deploy.cmd taskdesk
```

测试覆盖标题边界、引号、CRUD、磁盘重开与异步信号。check 脚本对全部 14 个应用执行自动退出启动检查；它不能替代真实交互验收。deploy 生成 `dist/<目标>/` 并调用本项目 Qt 的 windeployqt。分发目录包含 MSVC 运行库安装程序 `vc_redist.x64.exe`，目标机器缺少运行库时先安装它。分发前仍需在干净 Windows 环境验证，并核对第三方许可。

## 项目结构

```text
docs/                    中文 Markdown 课程、参考与验证记录
site/                    无外部依赖的离线网页
examples/                13 个配套示例
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

## 许可

原创文字、网页和示例源码使用 [MIT License](LICENSE)。Qt 与安装工具保持其自身许可；[第三方说明](THIRD_PARTY_NOTICES.md) 和 [Qt 官方许可说明](https://doc.qt.io/qt-6.8/licensing.html) 应一并阅读。
