#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "tablewithphilosophers.h"
#include "philosophers.h"
extern  QString philosopherName;
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    qmlRegisterType<PhilosophersModel>("Philosophers",1,0,"PhilosophersModel");
    qmlRegisterUncreatableType<TableWithPhilosophers>("Philosophers",1,0,"TableWithPhilosophers",
                                                      QStringLiteral("TableWithPhilosophers should not be created in QML") );
    TableWithPhilosophers table;
    QQmlApplicationEngine engine;
    QThread* workThread = new QThread();
    table.moveToThread(workThread);
    workThread->start();
    engine.rootContext()->setContextProperty(QStringLiteral("PhilosopherList"), &table);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
