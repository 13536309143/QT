#include <QtWidgets>
#include <QStandardPaths>
#include "taskstore.h"
class Window : public QMainWindow {
public:
    explicit Window(TaskStore &store):store_(store) {
        auto *body=new QWidget(this); setCentralWidget(body); auto *layout=new QVBoxLayout(body);
        auto *line=new QHBoxLayout;
        input_=new QLineEdit(body); input_->setPlaceholderText(QStringLiteral("输入任务，回车添加")); input_->setMaxLength(200);
        auto *add=new QPushButton(QStringLiteral("添加"),body); line->addWidget(input_); line->addWidget(add); layout->addLayout(line);
        auto *search=new QLineEdit(body); search->setPlaceholderText(QStringLiteral("搜索任务")); layout->addWidget(search);
        model_=new QSqlQueryModel(this); proxy_=new QSortFilterProxyModel(this);
        proxy_->setSourceModel(model_); proxy_->setFilterKeyColumn(1); proxy_->setFilterCaseSensitivity(Qt::CaseInsensitive);
        table_=new QTableView(body); table_->setModel(proxy_); table_->setSelectionBehavior(QAbstractItemView::SelectRows);
        table_->setSelectionMode(QAbstractItemView::SingleSelection); table_->setSortingEnabled(true); layout->addWidget(table_);
        auto *buttons=new QHBoxLayout; auto *toggle=new QPushButton(QStringLiteral("切换完成状态"),body);
        auto *remove=new QPushButton(QStringLiteral("删除选中任务"),body); buttons->addWidget(toggle); buttons->addWidget(remove); layout->addLayout(buttons);
        connect(add,&QPushButton::clicked,this,[this] { addTask(); });
        connect(input_,&QLineEdit::returnPressed,this,[this] { addTask(); });
        connect(search,&QLineEdit::textChanged,proxy_,&QSortFilterProxyModel::setFilterFixedString);
        connect(toggle,&QPushButton::clicked,this,[this] {
            const auto index=sourceIndex(); if(!index.isValid()) return;
            if(!store_.complete(model_->data(model_->index(index.row(),0)).toInt(),!model_->data(model_->index(index.row(),2)).toBool())) showError();
            reload();
        });
        connect(remove,&QPushButton::clicked,this,[this] {
            const auto index=sourceIndex(); if(!index.isValid()) return;
            const int id=model_->data(model_->index(index.row(),0)).toInt();
            if(QMessageBox::question(this,QStringLiteral("确认删除"),QStringLiteral("删除此任务？"))!=QMessageBox::Yes) return;
            if(!store_.remove(id)) showError(); reload();
        });
        setWindowTitle(QStringLiteral("TaskDesk · Qt 学习任务管理器")); resize(800,520);
        QSettings settings; restoreGeometry(settings.value("geometry").toByteArray()); reload();
    }
protected:
    void closeEvent(QCloseEvent *event) override { QSettings().setValue("geometry",saveGeometry()); QMainWindow::closeEvent(event); }
private:
    TaskStore &store_; QLineEdit *input_; QTableView *table_; QSqlQueryModel *model_; QSortFilterProxyModel *proxy_;
    void showError() { QMessageBox::warning(this,QStringLiteral("操作失败"),store_.error()); }
    QModelIndex sourceIndex() { return proxy_->mapToSource(table_->currentIndex()); }
    void addTask() { if(store_.add(input_->text())) { input_->clear(); reload(); } else showError(); }
    void reload() {
        model_->setQuery("SELECT id,title,done FROM tasks ORDER BY id DESC",store_.database());
        if(model_->lastError().isValid()) { QMessageBox::warning(this,QStringLiteral("查询失败"),model_->lastError().text()); return; }
        model_->setHeaderData(0,Qt::Horizontal,"ID"); model_->setHeaderData(1,Qt::Horizontal,QStringLiteral("任务"));
        model_->setHeaderData(2,Qt::Horizontal,QStringLiteral("完成(1/0)")); table_->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
        statusBar()->showMessage(QStringLiteral("已载入 %1 条；大数据请扩展分页").arg(model_->rowCount()));
    }
};
int main(int argc,char **argv) {
    QApplication app(argc,argv); app.setOrganizationName("QtLearning"); app.setApplicationName("TaskDesk");
    const bool smoke=app.arguments().contains("--smoke");
    if(smoke) QStandardPaths::setTestModeEnabled(true);
    const auto dir=QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if(!QDir().mkpath(dir)) { qCritical()<<"Cannot create data directory"; return 1; }
    TaskStore store; if(!store.open(smoke?QString(":memory:"):dir+"/tasks.sqlite")) { qCritical()<<store.error(); return 2; }
    Window w(store); w.show(); if(smoke) QTimer::singleShot(150,&app,&QCoreApplication::quit);
    return app.exec(); // w 和模型先析构，然后才销毁数据库连接
}
