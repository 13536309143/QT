#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTimer>
int main(int argc,char **argv) {
    QGuiApplication app(argc,argv); QQmlApplicationEngine engine;
    QObject::connect(&engine,&QQmlApplicationEngine::objectCreationFailed,&app,[]{ QCoreApplication::exit(1); },Qt::QueuedConnection);
    engine.loadFromModule("Course","Main");
    if(app.arguments().contains("--smoke")) QTimer::singleShot(200,&app,&QCoreApplication::quit);
    return app.exec();
}
