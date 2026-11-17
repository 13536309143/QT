#include <QtWidgets>
int main(int argc,char **argv) {
    QApplication app(argc,argv); QWidget w; auto *layout=new QVBoxLayout(&w);
    QFile text(":/lesson/readme.txt");
    if(!text.open(QIODevice::ReadOnly)) return 1;
    auto *label=new QLabel(QString::fromUtf8(text.readAll()),&w); layout->addWidget(label);
    QFile style(":/lesson/theme.qss");
    if(style.open(QIODevice::ReadOnly)) w.setStyleSheet(QString::fromUtf8(style.readAll()));
    layout->addWidget(new QPushButton(QStringLiteral("资源加载成功"),&w));
    w.resize(420,200); w.show();
    if(app.arguments().contains("--smoke")) QTimer::singleShot(150,&app,&QCoreApplication::quit);
    return app.exec();
}
