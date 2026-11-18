# 第三方依赖

本仓库原创教程与示例使用 MIT License。Qt、Python、CMake、MSVC、aqtinstall 及其依赖不因本仓库许可而改变原许可，也不随源码仓库提交。

- Qt SDK 从 Qt 分发源下载，本地 sdk 下保留原有许可文件。不同模块许可可能不同；发布应用前请阅读 [Qt 6.8 Licensing](https://doc.qt.io/qt-6.8/licensing.html) 和实际模块许可。
- aqtinstall 是第三方安装工具：[项目与许可](https://github.com/miurahr/aqtinstall)。它不是 Qt 官方在线安装器。
- GitHub Actions 工作流使用 GitHub 官方 actions/checkout、actions/setup-python 和 actions/upload-artifact。执行时下载依赖，不将它们作为本仓库源码重新许可。
- 图形扩展可使用 Khronos Vulkan-Headers（固定标签 vulkan-sdk-1.3.290.0）提供编译头文件，下载到被忽略的 .tools/vulkan-headers。保留该项目随附的许可文件，参见 [上游项目](https://github.com/KhronosGroup/Vulkan-Headers)。本仓库不复制其头文件到提交内容。完整 Vulkan SDK、验证层与 GPU 驱动分别遵循其自身许可。

项目未复制 Qt 官方教程全文，未包含第三方图片、字体、付费课程或授权密钥。官方参考链接见 docs/REFERENCES.md。
