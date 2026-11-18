# 官方参考与资料说明

本仓库为原创中文讲解和教学代码，不是 Qt 官方文档镜像。链接仅作进一步查阅；SDK 和工具保留其各自许可。教程核心主线为 Widgets/C++，另含 QML 入门和专门选修方向，未宣称覆盖每一个 Qt 模块。

- [Qt 6.8 文档](https://doc.qt.io/qt-6.8/)
- [Windows 支持与工具链](https://doc.qt.io/qt-6.8/windows.html)
- [CMake 入门](https://doc.qt.io/qt-6.8/cmake-get-started.html)
- [信号与槽](https://doc.qt.io/qt-6.8/signalsandslots.html)
- [QObject](https://doc.qt.io/qt-6.8/qobject.html)
- [线程](https://doc.qt.io/qt-6.8/threads-qobject.html)
- [Model/View](https://doc.qt.io/qt-6.8/model-view-programming.html)
- [网络](https://doc.qt.io/qt-6.8/qnetworkaccessmanager.html)
- [SQL](https://doc.qt.io/qt-6.8/qsqlquery.html)
- [测试](https://doc.qt.io/qt-6.8/qtest-overview.html)
- [Windows 部署](https://doc.qt.io/qt-6.8/windows-deployment.html)
- [Qt 许可](https://doc.qt.io/qt-6.8/licensing.html)
- [aqtinstall 使用](https://aqtinstall.readthedocs.io/en/latest/cli.html)
- [对象树与所有权](https://doc.qt.io/qt-6.8/objecttrees.html)
- [QDialog 与窗口寿命](https://doc.qt.io/qt-6.8/qdialog.html)
- [QSettings](https://doc.qt.io/qt-6.8/qsettings.html)
- [QStandardPaths](https://doc.qt.io/qt-6.8/qstandardpaths.html)
- [QSignalBlocker](https://doc.qt.io/qt-6.8/qsignalblocker.html)
- [事件与事件过滤](https://doc.qt.io/qt-6.8/eventsandfilters.html)
- [QTcpSocket](https://doc.qt.io/qt-6.8/qtcpsocket.html)
- [模型接口](https://doc.qt.io/qt-6.8/qabstractitemmodel.html)
- [QML 属性绑定](https://doc.qt.io/qt-6.8/qtqml-syntax-propertybinding.html)
- [模型自动检查](https://doc.qt.io/qt-6.8/qabstractitemmodeltester.html)
- [QTest 交互与异步等待](https://doc.qt.io/qt-6.8/qtest.html)

基线资料核对日期：2026-11-17。版本已固定，升级 SDK 时请重新构建、运行测试并复查相应版本的文档。

## 图形专题参考（2026-11-18 补充）

- [QOpenGLWidget：回调、默认 framebuffer 与资源清理](https://doc.qt.io/qt-6.8/qopenglwidget.html)
- [QOpenGLShaderProgram](https://doc.qt.io/qt-6.8/qopenglshaderprogram.html)
- [QOpenGLBuffer](https://doc.qt.io/qt-6.8/qopenglbuffer.html)
- [QOpenGLVertexArrayObject](https://doc.qt.io/qt-6.8/qopenglvertexarrayobject.html)
- [QOpenGLTexture](https://doc.qt.io/qt-6.8/qopengltexture.html)
- [QMatrix4x4](https://doc.qt.io/qt-6.8/qmatrix4x4.html)
- [QVulkanInstance](https://doc.qt.io/qt-6.8/qvulkaninstance.html)
- [QVulkanWindow](https://doc.qt.io/qt-6.8/qvulkanwindow.html)
- [QVulkanWindowRenderer](https://doc.qt.io/qt-6.8/qvulkanwindowrenderer.html)
- [Khronos Vulkan 同步指南](https://docs.vulkan.org/guide/latest/synchronization.html)
- [Khronos Vulkan 并发帧](https://docs.vulkan.org/tutorial/latest/03_Drawing_a_triangle/03_Drawing/03_Frames_in_flight.html)

图形代码为本课程配套教学实现。OpenGL 3.3 Core 与 Vulkan 清屏是完整可构建示例；矩阵、纹理、Vulkan 三角形和描述符等章节明确标注为逐步扩展，不冒充全部已经实现的渲染引擎。

## Qt Creator 图形化操作参考

- [Creator 工作区](https://doc.qt.io/qtcreator/creator-quick-tour.html)
- [Kit 配置](https://doc.qt.io/qtcreator/creator-preferences-kits.html)
- [打开项目](https://doc.qt.io/qtcreator/creator-project-opening.html)
- [Widgets 新建向导](https://doc.qt.io/qtcreator/creator-writing-program.html)
- [集成 Widgets Designer](https://doc.qt.io/qtcreator/creator-using-qt-designer.html)
- [Designer 布局](https://doc.qt.io/qt-6.8/designer-layouts.html)
- [Designer 信号槽模式](https://doc.qt.io/qt-6.8/designer-connection-mode.html)
- [CMake 设置](https://doc.qt.io/qtcreator/creator-build-settings-cmake.html)
- [运行设置](https://doc.qt.io/qtcreator/creator-run-settings.html)
- [调试器支持与 CDB](https://doc.qt.io/qtcreator/creator-debugger-engines.html)

本机截图来自 Qt Creator 20.0.2。项目 SDK 仍以 Qt 6.8.3 / MSVC 2022 为教学基线；Creator 设置页当前检测的其他 Qt 和工具链不能当作课程已配置完成。功能示意图为课程原创矢量图，明确标注非截图。
