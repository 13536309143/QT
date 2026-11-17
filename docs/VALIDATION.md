# 验证记录

验证日期：2026-11-17。环境：Windows 11 x64、Qt 6.8.3、MSVC 2022（19.44）、C++17、Release。

| 检查 | 结果 |
|---|---|
| 项目内 Qt SDK | 6.8.3，MSVC x64，包含 Widgets、Quick、SQL、Test、Designer 和翻译资源 |
| 构建入口 scripts/build.cmd | 配置、编译全部目标成功 |
| Qt Test | 10 passed，0 failed，0 skipped（含初始化与清理） |
| 数据规则 | 空白、正常标题、引号、200/201 长度边界通过 |
| 数据库 | CRUD 与关闭连接后重开文件通过 |
| 异步信号 | 单次定时器信号测试通过 |
| 应用启动 | 13 个示例 + TaskDesk 的 --smoke 检查通过 |
| Windows 分发 | hello、TaskDesk、Quick 三个目录生成成功 |
| 脱离 SDK 路径启动 | 清除 Qt/QML 环境变量并缩减 PATH 后，三个分发程序启动成功 |
| 文档检查 | 本地相对链接、JSON、PowerShell 脚本语法检查通过 |
| Git 忽略 | sdk、.tools、build、dist 均被忽略 |

Qt Test 的 10 项结果包含 initTestCase、cleanupTestCase；另外 8 项为参数化验证、CRUD、磁盘持久化与信号行为。详细结果可由测试命令重新生成，JUnit 报告位于 build/qt-test-results.xml。

## 适用范围

启动检查验证初始化、资源加载和退出，不代替按钮交互、长时间线程任务、网络异常、视觉缩放与文件对话框取消等手工验收。对应步骤见 [实操实验手册](WORKBOOK.md)。

分发目录在同一台 Windows 机器清除开发环境路径后验证；干净虚拟机、其他 Windows 版本、Linux、macOS 和实际串口硬件需分别验证。MSVC 运行库安装程序 vc_redist.x64.exe 随分发目录提供；目标机器缺少运行库时需先安装。

GitHub Actions 的执行状态以远程仓库的实际运行结果为准。该记录不等同于远程 CI 已通过。
