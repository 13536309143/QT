#include <QtWidgets>

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("QtLearning");
    QCoreApplication::setApplicationName("SettingsLab");
    const bool smoke = app.arguments().contains("--smoke");
    QStandardPaths::setTestModeEnabled(smoke);
    QTemporaryDir temporary;
    const QString folder = smoke ? temporary.path() : QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    if (folder.isEmpty() || !QDir().mkpath(folder)) return 1;
    // 使用明确路径的 INI，便于观察；启动检查只写临时目录。
    QSettings settings(QDir(folder).filePath("preferences.ini"), QSettings::IniFormat);
    QWidget window;
    window.setWindowTitle(QStringLiteral("15 · 配置与恢复"));
    auto *layout = new QFormLayout(&window);
    auto *name = new QLineEdit(settings.value("profile/name", QString()).toString(), &window);
    auto *minutes = new QSpinBox(&window);
    minutes->setRange(5, 120);
    minutes->setValue(qBound(5, settings.value("study/minutes", 30).toInt(), 120));
    auto *save = new QPushButton(QStringLiteral("保存设置"), &window);
    auto *status = new QLabel(&window);
    status->setWordWrap(true);
    layout->addRow(QStringLiteral("姓名"), name);
    layout->addRow(QStringLiteral("每天分钟数"), minutes);
    layout->addRow(save);
    auto *pathLabel = new QLabel(QStringLiteral("配置文件：%1").arg(settings.fileName()), &window);
    pathLabel->setWordWrap(true);
    pathLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout->addRow(pathLabel);
    layout->addRow(status);
    window.resize(620, 260);
    window.restoreGeometry(settings.value("window/geometry").toByteArray());
    QObject::connect(save, &QPushButton::clicked, &window, [&] {
        settings.setValue("profile/name", name->text().trimmed());
        settings.setValue("study/minutes", minutes->value());
        settings.setValue("window/geometry", window.saveGeometry());
        settings.sync();
        status->setText(settings.status() == QSettings::NoError
            ? QStringLiteral("已保存，请关闭后重新打开验证") : QStringLiteral("保存失败，请检查路径与权限"));
    });
    window.show();
    if (smoke) QTimer::singleShot(150, &app, &QCoreApplication::quit);
    return app.exec();
}
