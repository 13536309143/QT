#include <QtWidgets>
#include <QtNetwork>
int main(int argc,char **argv) {
    QApplication app(argc,argv); QWidget w; auto *layout=new QVBoxLayout(&w);
    auto *url=new QLineEdit("http://127.0.0.1:8765/sample.json",&w);
    auto *button=new QPushButton(QStringLiteral("GET 请求"),&w);
    auto *output=new QPlainTextEdit(&w); output->setReadOnly(true);
    auto *manager=new QNetworkAccessManager(&w);
    layout->addWidget(url); layout->addWidget(button); layout->addWidget(output);
    QObject::connect(button,&QPushButton::clicked,&w,[url,button,output,manager,&w] {
        QUrl target(url->text());
        if(!target.isValid() || (target.scheme()!="https" && target.scheme()!="http") || target.host().isEmpty()) {
            output->setPlainText(QStringLiteral("请输入有效 HTTP/HTTPS 地址")); return;
        }
        button->setEnabled(false); output->clear();
        QNetworkRequest request(target); request.setTransferTimeout(10000);
        auto *reply=manager->get(request);
        // 总时限独立于“连续无数据”超时；reply 销毁时定时器自动失效。
        QTimer::singleShot(15000,reply,[reply] { if(reply->isRunning()) reply->abort(); });
        QObject::connect(reply,&QNetworkReply::downloadProgress,reply,[reply](qint64 received,qint64) {
            if(received>1024*1024) reply->abort(); // 教学客户端限制为 1 MiB
        });
        QObject::connect(reply,&QNetworkReply::finished,&w,[reply,button,output] {
            const auto status=reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            output->setPlainText(QStringLiteral("HTTP %1\n%2\n%3").arg(status)
                .arg(reply->error()==QNetworkReply::NoError?QStringLiteral("成功"):reply->errorString())
                .arg(QString::fromUtf8(reply->readAll())));
            reply->deleteLater(); button->setEnabled(true);
        });
    });
    w.resize(700,450); w.show();
    if(app.arguments().contains("--smoke")) QTimer::singleShot(150,&app,&QCoreApplication::quit);
    return app.exec();
}
