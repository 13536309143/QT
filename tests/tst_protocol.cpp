#include <QtTest>
#include <QTcpServer>
#include <QTcpSocket>
#include "../examples/16_tcp/lineprotocol.h"

class ProtocolTests : public QObject {
    Q_OBJECT
private slots:
    void fragmentedUtf8() {
        LineProtocol parser;
        QList<QByteArray> lines;
        const auto text = QStringLiteral("你好，Qt").toUtf8();
        QVERIFY(parser.feed(text.left(1), lines));
        QVERIFY(lines.isEmpty());
        QVERIFY(parser.feed(text.mid(1), lines));
        QVERIFY(lines.isEmpty());
        QVERIFY(parser.feed("\n", lines));
        QCOMPARE(lines, QList<QByteArray>{text});
    }
    void coalescedAndPartial() {
        LineProtocol parser;
        QList<QByteArray> lines;
        QVERIFY(parser.feed("one\ntwo\npar", lines));
        QCOMPARE(lines, (QList<QByteArray>{"one", "two"}));
        lines.clear();
        QVERIFY(parser.feed("tial\n", lines));
        QCOMPARE(lines, QList<QByteArray>{"partial"});
    }
    void boundaries_data() {
        QTest::addColumn<QByteArray>("input");
        QTest::addColumn<bool>("accepted");
        QTest::addColumn<int>("frames");
        QTest::newRow("empty frame") << QByteArray("\n") << true << 1;
        QTest::newRow("exact limit") << (QByteArray(1024, 'x') + '\n') << true << 1;
        QTest::newRow("over limit") << (QByteArray(1025, 'x') + '\n') << false << 0;
        QTest::newRow("pending exact limit") << QByteArray(1024, 'x') << true << 0;
    }
    void boundaries() {
        QFETCH(QByteArray, input); QFETCH(bool, accepted); QFETCH(int, frames);
        LineProtocol parser;
        QList<QByteArray> lines;
        QCOMPARE(parser.feed(input, lines), accepted);
        QCOMPARE(lines.size(), frames);
    }
    void oversizedAcrossChunks() {
        LineProtocol parser;
        QList<QByteArray> lines;
        QVERIFY(parser.feed(QByteArray(1024, 'x'), lines));
        QVERIFY(!parser.feed("x", lines));
        QVERIFY(lines.isEmpty());
    }
    void loopbackRoundTrip() {
        QTcpServer server;
        QVERIFY(server.listen(QHostAddress::LocalHost, 0));
        QTcpSocket client;
        client.connectToHost(QHostAddress::LocalHost, server.serverPort());
        QTRY_VERIFY(client.state() == QAbstractSocket::ConnectedState);
        QTRY_VERIFY(server.hasPendingConnections());
        auto *peer = server.nextPendingConnection();
        QVERIFY(peer);
        LineProtocol serverParser;
        QObject::connect(peer, &QTcpSocket::readyRead, peer, [peer, &serverParser] {
            QList<QByteArray> lines;
            if (!serverParser.feed(peer->readAll(), lines)) { peer->abort(); return; }
            for (const auto &line : lines) peer->write(line + '\n');
        });
        const auto text = QStringLiteral("实际网络往返").toUtf8();
        QCOMPARE(client.write(text + '\n'), qint64(text.size() + 1));
        QTRY_VERIFY(client.bytesAvailable() >= text.size() + 1);
        QCOMPARE(client.readAll(), text + '\n');
        // 在解析器销毁前显式销毁连接，回调不会引用失效的栈对象。
        delete peer;
    }
};
QTEST_MAIN(ProtocolTests)
#include "tst_protocol.moc"
