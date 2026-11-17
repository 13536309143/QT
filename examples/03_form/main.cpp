#include <QtWidgets>
int main(int argc,char **argv) {
    QApplication app(argc,argv); QWidget w;
    auto *layout = new QFormLayout(&w);
    auto *name = new QLineEdit(&w);
    auto *age = new QSpinBox(&w); age->setRange(1,120); age->setValue(18);
    auto *agree = new QCheckBox(QStringLiteral("确认信息正确"),&w);
    auto *submit = new QPushButton(QStringLiteral("提交"),&w);
    auto *result = new QLabel(&w); result->setWordWrap(true);
    layout->addRow(QStringLiteral("姓名"), name); layout->addRow(QStringLiteral("年龄"),age);
    layout->addRow(agree); layout->addRow(submit); layout->addRow(result);
    QObject::connect(submit,&QPushButton::clicked,&w,[=] {
        const auto text = name->text().trimmed();
        if(text.isEmpty() || !agree->isChecked()) {
            result->setText(QStringLiteral("请填写姓名并确认信息。")); return;
        }
        result->setText(QStringLiteral("你好，%1，你今年 %2 岁。").arg(text).arg(age->value()));
    });
    w.resize(420,250); w.show();
    if(app.arguments().contains("--smoke")) QTimer::singleShot(150,&app,&QCoreApplication::quit);
    return app.exec();
}
