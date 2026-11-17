#include <QApplication>
#include <QLabel>
#include <QTimer>
int main(int argc, char *argv[]) {
    QApplication app(argc, argv); // QApplication 必须先于任何 QWidget 创建
    QLabel label(QStringLiteral("你好，Qt！\n缩放窗口试试看。"));
    label.setAlignment(Qt::AlignCenter);
    label.resize(480, 260);
    label.setWindowTitle(QStringLiteral("01 · 第一个窗口"));
    label.show();
    if (app.arguments().contains("--smoke")) QTimer::singleShot(150, &app, &QCoreApplication::quit);
    return app.exec(); // 进入事件循环，关闭最后一个窗口后退出
}
