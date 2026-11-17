#include <QtWidgets>

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle(QStringLiteral("14 · 控件与状态同步"));
    auto *layout = new QFormLayout(&window);
    auto *name = new QLineEdit(&window);
    name->setObjectName("nameInput");
    name->setMaxLength(40);
    auto *level = new QComboBox(&window);
    level->addItem(QStringLiteral("刚开始"), 0);
    level->addItem(QStringLiteral("学过 C++"), 1);
    auto *minutes = new QSpinBox(&window);
    minutes->setRange(5, 120);
    minutes->setValue(30);
    auto *slider = new QSlider(Qt::Horizontal, &window);
    slider->setRange(5, 120);
    slider->setValue(30);
    auto *preview = new QLabel(&window);
    preview->setObjectName("preview");
    preview->setWordWrap(true);
    auto *reset = new QPushButton(QStringLiteral("恢复默认"), &window);
    layout->addRow(QStringLiteral("姓名(&N)"), name);
    layout->addRow(QStringLiteral("基础(&L)"), level);
    layout->addRow(QStringLiteral("每天分钟数(&M)"), minutes);
    layout->addRow(QStringLiteral("拖动调整"), slider);
    layout->addRow(preview);
    layout->addRow(reset);
    QObject::connect(slider, &QSlider::valueChanged, minutes, &QSpinBox::setValue);
    QObject::connect(minutes, &QSpinBox::valueChanged, slider, &QSlider::setValue);
    auto refresh = [=] {
        const QString who = name->text().trimmed();
        preview->setText(QStringLiteral("%1：%2，每天 %3 分钟")
            .arg(who.isEmpty() ? QStringLiteral("未填写姓名") : who)
            .arg(level->currentText()).arg(minutes->value()));
    };
    QObject::connect(name, &QLineEdit::textChanged, &window, refresh);
    QObject::connect(level, &QComboBox::currentIndexChanged, &window, refresh);
    QObject::connect(minutes, &QSpinBox::valueChanged, &window, refresh);
    QObject::connect(reset, &QPushButton::clicked, &window, [=] {
        // 批量修改时暂停中间通知，最后统一刷新派生显示。
        const QSignalBlocker blockName(name), blockLevel(level), blockMinutes(minutes), blockSlider(slider);
        name->clear(); level->setCurrentIndex(0);
        minutes->setValue(30); slider->setValue(30);
        refresh();
    });
    refresh();
    window.resize(520, 280);
    window.show();
    if (app.arguments().contains("--smoke")) QTimer::singleShot(150, &app, &QCoreApplication::quit);
    return app.exec();
}
