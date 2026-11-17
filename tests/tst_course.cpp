#include <QtTest>
#include <QSignalSpy>
#include "taskstore.h"
class CourseTests : public QObject {
    Q_OBJECT
private slots:
    void validation_data() {
        QTest::addColumn<QString>("text"); QTest::addColumn<bool>("accepted");
        QTest::newRow("blank")<<QString("  ")<<false;
        QTest::newRow("normal")<<QStringLiteral("学习信号槽")<<true;
        QTest::newRow("quote")<<QStringLiteral("O'Reilly")<<true;
        QTest::newRow("limit")<<QString(200,'a')<<true;
        QTest::newRow("too long")<<QString(201,'a')<<false;
    }
    void validation() {
        QFETCH(QString,text); QFETCH(bool,accepted);
        TaskStore store; QVERIFY2(store.open(":memory:"),qPrintable(store.error())); QCOMPARE(store.add(text),accepted);
    }
    void crud() {
        TaskStore store; QVERIFY(store.open(":memory:")); QVERIFY(store.add(QStringLiteral("任务 ' A")));
        QSqlQuery q(store.database()); QVERIFY(q.exec("SELECT id,title,done FROM tasks")); QVERIFY(q.next());
        const int id=q.value(0).toInt(); QCOMPARE(q.value(1).toString(),QStringLiteral("任务 ' A")); QCOMPARE(q.value(2).toInt(),0); q.finish();
        QVERIFY(store.complete(id,true)); QVERIFY(q.exec("SELECT done FROM tasks")); QVERIFY(q.next()); QCOMPARE(q.value(0).toInt(),1); q.finish();
        QVERIFY(store.remove(id)); QVERIFY(!store.remove(id));
        QVERIFY(q.exec("SELECT COUNT(*) FROM tasks")); QVERIFY(q.next()); QCOMPARE(q.value(0).toInt(),0);
    }
    void diskPersistence() {
        QTemporaryDir dir; QVERIFY(dir.isValid()); const auto path=dir.filePath("test.sqlite");
        { TaskStore store; QVERIFY(store.open(path)); QVERIFY(store.add("persistent")); }
        { TaskStore store; QVERIFY(store.open(path)); QSqlQuery q(store.database());
          QVERIFY(q.exec("SELECT title FROM tasks")); QVERIFY(q.next()); QCOMPARE(q.value(0).toString(),QString("persistent")); }
    }
    void timerSignal() {
        QTimer timer; timer.setSingleShot(true); QSignalSpy spy(&timer,&QTimer::timeout);
        timer.start(10); QVERIFY(spy.wait(1000)); QCOMPARE(spy.count(),1);
    }
};
QTEST_MAIN(CourseTests)
#include "tst_course.moc"
