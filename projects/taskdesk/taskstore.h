#pragma once
#include <QtSql>
#include <QUuid>
// 仓储层：不依赖窗口，测试可使用内存数据库。
class TaskStore {
public:
    TaskStore():name_(QUuid::createUuid().toString()),db_(QSqlDatabase::addDatabase("QSQLITE",name_)) {}
    TaskStore(const TaskStore &)=delete;
    TaskStore &operator=(const TaskStore &)=delete;
    ~TaskStore() { db_.close(); db_=QSqlDatabase(); QSqlDatabase::removeDatabase(name_); }
    bool open(const QString &path) {
        db_.setDatabaseName(path);
        if(!db_.open()) { error_=db_.lastError().text(); return false; }
        QSqlQuery q(db_);
        if(!q.exec("CREATE TABLE IF NOT EXISTS tasks(id INTEGER PRIMARY KEY AUTOINCREMENT,title TEXT NOT NULL,done INTEGER NOT NULL DEFAULT 0 CHECK(done IN (0,1)))")) {
            error_=q.lastError().text(); return false;
        }
        return true;
    }
    bool add(const QString &title) {
        const auto t=title.trimmed();
        if(t.isEmpty() || t.size()>200) { error_=QStringLiteral("任务应为 1~200 个 UTF-16 代码单元"); return false; }
        QSqlQuery q(db_); q.prepare("INSERT INTO tasks(title) VALUES(?)"); q.addBindValue(t); return execute(q);
    }
    bool complete(int id,bool done) {
        QSqlQuery q(db_); q.prepare("UPDATE tasks SET done=? WHERE id=?");
        q.addBindValue(done?1:0); q.addBindValue(id); return execute(q) && q.numRowsAffected()==1;
    }
    bool remove(int id) {
        QSqlQuery q(db_); q.prepare("DELETE FROM tasks WHERE id=?"); q.addBindValue(id);
        return execute(q) && q.numRowsAffected()==1;
    }
    QSqlDatabase database() const { return db_; }
    QString error() const { return error_; }
private:
    bool execute(QSqlQuery &q) { if(q.exec()) return true; error_=q.lastError().text(); return false; }
    QString name_,error_;
    QSqlDatabase db_;
};
