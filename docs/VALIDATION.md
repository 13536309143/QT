# 验证记录

验证日期：2026-11-17。环境：Windows 11 x64、Qt 6.8.3、MSVC 2022（19.44）、C++17、Release。

| 检查 | 结果 |
|---|---|
| 项目内 Qt SDK | 6.8.3，MSVC x64，包含 Widgets、Quick、SQL、Test、Designer 和翻译资源 |
| 构建入口 scripts/build.cmd | 配置、编译全部目标成功 |
| Qt Test | course_tests 与 protocol_tests 两组通过，共 20 项结果，0 failed，0 skipped（含两组初始化与清理） |
| 数据规则 | 空白、正常标题、引号、200/201 长度边界通过 |
| 数据库 | CRUD 与关闭连接后重开文件通过 |
| 异步信号 | 单次定时器信号测试通过 |
| 应用启动 | 16 个示例 + TaskDesk 的 --smoke 检查通过 |
| TCP 协议 | UTF-8 分段、半包、粘包、1024/1025 字节边界、跨块超长与实际回环往返通过 |
| 离线教程 | 49 章、16 个示例、18 个实验；路线覆盖、代码围栏和 53 个 HTML 页面本地链接检查通过 |
| Windows 分发 | hello、TaskDesk、Quick 三个目录生成成功 |
| 脱离 SDK 路径启动 | 清除 Qt/QML 环境变量并缩减 PATH 后，三个分发程序启动成功 |
| 文档检查 | 本地相对链接、JSON、PowerShell 脚本语法检查通过 |
| Git 忽略 | sdk、.tools、build、dist 均被忽略 |

两组 Qt Test 各有 10 项结果，包含各自的 initTestCase、cleanupTestCase；另外合计 16 项为数据驱动行和行为检查，不代表 20 项独立业务需求。详细报告位于 build/qt-test-results.xml 与 build/protocol-test-results.xml。协议测试运行真实的本机回环连接，不依赖公共网络服务。

新增示例 14_controls、15_settings、16_tcp 已编译并通过启动检查。QSettings 启动检查使用临时目录；TCP 只监听回环地址且使用系统分配端口。启动检查不能代替控件点击、保存后重开等手工验收，实验十三至十八列出了对应步骤。

新增三个分发目录在清除 Qt/QML 环境变量并缩减 PATH 后启动通过。离线页面在 Chromium 中检查了 49 个章节导航、正文搜索、零结果与清空恢复、当前章节标记、推荐路线翻页，以及 390px 宽度下首页和控件章节无横向溢出。桌面首页做过截图检查；多种真实手机、全部页面和全部交互仍需后续人工检查。

## 适用范围

启动检查验证初始化、资源加载和退出，不代替按钮交互、长时间线程任务、网络异常、视觉缩放与文件对话框取消等手工验收。对应步骤见 [实操实验手册](WORKBOOK.md)。

分发目录在同一台 Windows 机器清除开发环境路径后验证；干净虚拟机、其他 Windows 版本、Linux、macOS 和实际串口硬件需分别验证。MSVC 运行库安装程序 vc_redist.x64.exe 随分发目录提供；目标机器缺少运行库时需先安装。

GitHub Actions 的执行状态以远程仓库的实际运行结果为准。该记录不等同于远程 CI 已通过。
