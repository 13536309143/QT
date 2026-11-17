#include <QtWidgets>
#include <QtNetwork>
#include <memory>
#include "lineprotocol.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle(QStringLiteral("16 · 本机 TCP 按行通信"));
    auto *layout = new QVBoxLayout(&window);
    auto *input = new QLineEdit(QStringLiteral("你好，TCP"), &window);
    auto *send = new QPushButton(QStringLiteral("发送一行"), &window);
    auto *log = new QPlainTextEdit(&window);
    log->setReadOnly(true);
    log->document()->setMaximumBlockCount(200);
    layout->addWidget(input); layout->addWidget(send); layout->addWidget(log);
    QTcpServer server;
    QTcpSocket client;
    LineProtocol clientProtocol;
    bool pending = false;
    QTimer deadline;
    deadline.setSingleShot(true);
    deadline.setInterval(3000);
    send->setEnabled(false);
    QObject::connect(&server, &QTcpServer::newConnection, &window, [&] {
        while (server.hasPendingConnections()) {
            auto *peer = server.nextPendingConnection();
            peer->setReadBufferSize(4096);
            auto protocol = std::make_shared<LineProtocol>();
            QObject::connect(peer, &QTcpSocket::readyRead, peer, [peer, protocol] {
                QList<QByteArray> lines;
                while (peer->bytesAvailable() > 0) {
                    if (!protocol->feed(peer->read(4096), lines)) { peer->abort(); return; }
                    for (const auto &line : lines) {
                        if (peer->bytesToWrite() > 65536) { peer->abort(); return; }
                        peer->write(line + '\n');
                    }
                    lines.clear();
                }
            });
            QObject::connect(peer, &QTcpSocket::disconnected, peer, &QObject::deleteLater);
        }
    });
    QObject::connect(&client, &QTcpSocket::connected, &window, [&] {
        log->appendPlainText(QStringLiteral("已连接本机回显服务")); send->setEnabled(true);
    });
    QObject::connect(&client, &QTcpSocket::readyRead, &window, [&] {
        QList<QByteArray> lines;
        while (client.bytesAvailable() > 0) {
            if (!clientProtocol.feed(client.read(4096), lines)) { client.abort(); return; }
            for (const auto &line : lines) log->appendPlainText(QStringLiteral("收到：%1").arg(QString::fromUtf8(line)));
            if (!lines.isEmpty()) { pending = false; deadline.stop(); send->setEnabled(true); }
            lines.clear();
        }
    });
    QObject::connect(&client, &QTcpSocket::errorOccurred, &window, [&](QAbstractSocket::SocketError) {
        deadline.stop(); pending = false; send->setEnabled(false); log->appendPlainText(client.errorString());
    });
    QObject::connect(&client, &QTcpSocket::disconnected, &window, [&] { send->setEnabled(false); });
    QObject::connect(&deadline, &QTimer::timeout, &window, [&] {
        log->appendPlainText(QStringLiteral("响应超时，本次连接终止")); pending = false; client.abort(); send->setEnabled(false);
    });
    QObject::connect(send, &QPushButton::clicked, &window, [&] {
        const QByteArray bytes = input->text().toUtf8();
        if (pending || bytes.isEmpty() || bytes.size() > LineProtocol::MaxPayload || bytes.contains('\n')) {
            log->appendPlainText(QStringLiteral("请输入 1~1024 字节、不含换行的 UTF-8 文本")); return;
        }
        if (client.write(bytes + '\n') == -1) { log->appendPlainText(client.errorString()); return; }
        pending = true; send->setEnabled(false); deadline.start();
    });
    // 只监听回环地址，端口 0 由操作系统分配，不占用固定课程端口。
    if (!server.listen(QHostAddress::LocalHost, 0)) return 1;
    client.setReadBufferSize(4096);
    client.connectToHost(QHostAddress::LocalHost, server.serverPort());
    window.resize(620, 360); window.show();
    if (app.arguments().contains("--smoke")) QTimer::singleShot(300, &app, &QCoreApplication::quit);
    const int result = app.exec();
    // 在局部状态析构前断开窗口回调，再主动终止连接。
    QObject::disconnect(&client, nullptr, &window, nullptr);
    QObject::disconnect(&server, nullptr, &window, nullptr);
    QObject::disconnect(&deadline, nullptr, &window, nullptr);
    client.abort();
    server.close();
    return result;
}
