#include <QtWidgets>
#include <QSaveFile>
class Editor : public QMainWindow {
public:
    Editor() {
        setCentralWidget(edit_); resize(760,500);
        auto *menu = menuBar()->addMenu(QStringLiteral("文件"));
        auto *open = menu->addAction(QStringLiteral("打开")); open->setShortcut(QKeySequence::Open);
        auto *save = menu->addAction(QStringLiteral("保存")); save->setShortcut(QKeySequence::Save);
        connect(open,&QAction::triggered,this,[this] {
            if(!confirm()) return;
            const QString path=QFileDialog::getOpenFileName(this,QStringLiteral("打开 UTF-8 文本"));
            if(path.isEmpty()) return;
            QFile f(path);
            if(!f.open(QIODevice::ReadOnly)) { error(f.errorString()); return; }
            const QByteArray bytes=f.readAll();
            if(f.error()!=QFileDevice::NoError) { error(f.errorString()); return; }
            edit_->setPlainText(QString::fromUtf8(bytes)); path_=path;
            edit_->document()->setModified(false); setWindowTitle(path_);
        });
        connect(save,&QAction::triggered,this,[this] { saveFile(); });
    }
protected:
    void closeEvent(QCloseEvent *event) override { if(confirm()) event->accept(); else event->ignore(); }
private:
    QTextEdit *edit_=new QTextEdit(this);
    QString path_;
    void error(const QString &message) { QMessageBox::warning(this,QStringLiteral("文件错误"),message); }
    bool saveFile() {
        QString target=path_;
        if(target.isEmpty()) target=QFileDialog::getSaveFileName(this,QStringLiteral("保存 UTF-8 文本"));
        if(target.isEmpty()) return false;
        QSaveFile f(target); const auto bytes=edit_->toPlainText().toUtf8();
        if(!f.open(QIODevice::WriteOnly) || f.write(bytes)!=bytes.size() || !f.commit()) {
            error(f.errorString()); return false;
        }
        path_=target; edit_->document()->setModified(false); setWindowTitle(path_); return true;
    }
    bool confirm() {
        if(!edit_->document()->isModified()) return true;
        const auto choice=QMessageBox::question(this,QStringLiteral("未保存"),QStringLiteral("是否保存修改？"),
            QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel,QMessageBox::Save);
        return choice==QMessageBox::Discard || (choice==QMessageBox::Save && saveFile());
    }
};
int main(int argc,char **argv) {
    QApplication app(argc,argv); Editor w; w.show();
    if(app.arguments().contains("--smoke")) QTimer::singleShot(150,&app,&QCoreApplication::quit);
    return app.exec();
}
