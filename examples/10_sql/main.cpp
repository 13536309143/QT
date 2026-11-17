#include <QtCore>
#include <QtSql>
int main(int argc,char **argv) {
    QCoreApplication app(argc,argv);
    auto db=QSqlDatabase::addDatabase("QSQLITE"); db.setDatabaseName(":memory:");
    if(!db.open()) { qCritical()<<db.lastError().text(); return 1; }
    QSqlQuery query(db);
    if(!query.exec("CREATE TABLE tasks(id INTEGER PRIMARY KEY, title TEXT NOT NULL)")) return 2;
    query.prepare("INSERT INTO tasks(title) VALUES(:title)");
    query.bindValue(":title",QStringLiteral("带引号的任务 ' Qt"));
    if(!query.exec()) { qCritical()<<query.lastError().text(); return 3; }
    if(!query.exec("SELECT id,title FROM tasks ORDER BY id")) return 4;
    while(query.next()) qInfo()<<query.value(0).toInt()<<query.value(1).toString();
    return 0;
}
