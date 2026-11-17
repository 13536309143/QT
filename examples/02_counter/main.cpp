#include <QtWidgets>
class Counter final : public QWidget {
    Q_OBJECT
public:
    Counter() {
        auto *layout = new QVBoxLayout(this);
        auto *value = new QLabel("0", this);
        auto *button = new QPushButton(QStringLiteral("加一"), this);
        layout->addWidget(value); layout->addWidget(button);
        // this 是连接上下文：窗口销毁后连接自动失效。
        connect(button, &QPushButton::clicked, this, [this] { emit countChanged(++count_); });
        connect(this, &Counter::countChanged, value, [value](int n) { value->setText(QString::number(n)); });
        resize(320,180);
    }
signals:
    void countChanged(int count);
private:
    int count_ = 0;
};
int main(int argc,char **argv) {
    QApplication app(argc,argv); Counter w; w.show();
    if(app.arguments().contains("--smoke")) QTimer::singleShot(150,&app,&QCoreApplication::quit);
    return app.exec();
}
// Q_OBJECT 在 cpp 中定义时，AUTOMOC 需要这一行。
#include "main.moc"
