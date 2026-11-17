#include <QtCore>
int main(int argc,char **argv) {
    QCoreApplication app(argc,argv);
    QTemporaryDir dir; // 本例退出自动清理，不污染用户目录
    if(!dir.isValid()) return 1;
    QJsonObject object{{"title",QStringLiteral("学习 Qt")},{"done",false},{"version",1}};
    const auto bytes=QJsonDocument(object).toJson();
    QSaveFile out(dir.filePath("task.json"));
    if(!out.open(QIODevice::WriteOnly) || out.write(bytes)!=bytes.size() || !out.commit()) {
        qCritical()<<out.errorString(); return 1;
    }
    QFile in(dir.filePath("task.json"));
    if(!in.open(QIODevice::ReadOnly)) return 2;
    QJsonParseError error;
    auto doc=QJsonDocument::fromJson(in.readAll(),&error);
    if(error.error!=QJsonParseError::NoError || !doc.isObject()) return 3;
    qInfo().noquote()<<doc.object().value("title").toString(); return 0;
}
