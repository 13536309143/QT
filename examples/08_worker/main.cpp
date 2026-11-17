#include <QtWidgets>
class Worker : public QObject {
    Q_OBJECT
public slots:
    void run() {
        for(int i=0;i<=100;++i) {
            if(QThread::currentThread()->isInterruptionRequested()) break;
            QThread::msleep(20); // 仅模拟后台耗时工作，不在 GUI 线程 sleep
            emit progress(i);
        }
        emit finished();
    }
signals: void progress(int value); void finished();
};
class Window : public QWidget {
public:
    Window() {
        auto *layout=new QVBoxLayout(this); auto *bar=new QProgressBar(this);
        auto *start=new QPushButton(QStringLiteral("开始"),this);
        auto *cancel=new QPushButton(QStringLiteral("取消"),this);
        layout->addWidget(bar); layout->addWidget(start); layout->addWidget(cancel);
        connect(start,&QPushButton::clicked,this,[=] {
            if(thread_.isRunning()) return;
            bar->setValue(0); start->setEnabled(false);
            auto *worker=new Worker; worker->moveToThread(&thread_);
            connect(&thread_,&QThread::started,worker,&Worker::run);
            connect(worker,&Worker::progress,bar,&QProgressBar::setValue);
            connect(worker,&Worker::finished,&thread_,&QThread::quit,Qt::DirectConnection);
            connect(&thread_,&QThread::finished,worker,&QObject::deleteLater);
            thread_.start();
        });
        connect(&thread_,&QThread::finished,this,[=] { start->setEnabled(true); });
        connect(cancel,&QPushButton::clicked,this,[this] { thread_.requestInterruption(); });
    }
    ~Window() override { thread_.requestInterruption(); thread_.quit(); thread_.wait(); }
private: QThread thread_;
};
int main(int argc,char **argv) {
    QApplication app(argc,argv); Window w; w.show();
    if(app.arguments().contains("--smoke")) QTimer::singleShot(150,&app,&QCoreApplication::quit);
    return app.exec();
}
#include "main.moc"
