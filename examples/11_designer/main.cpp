#include <QtWidgets>
#include "ui_form.h"
int main(int argc,char **argv) {
    QApplication app(argc,argv); QWidget w; Ui::Form ui; ui.setupUi(&w);
    QObject::connect(ui.pushButton,&QPushButton::clicked,&w,[&ui] {
        ui.label->setText(QStringLiteral("你好，%1").arg(ui.lineEdit->text().trimmed()));
    });
    w.show();
    if(app.arguments().contains("--smoke")) QTimer::singleShot(150,&app,&QCoreApplication::quit);
    return app.exec();
}
