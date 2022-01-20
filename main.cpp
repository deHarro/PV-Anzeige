#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QApplication>

#include "PowerNodeModel.h"

// global array for saving the XML content of SmartCharger app running on SmartCharger RasPi
QByteArray m_XMLfiledata;
// global array for saving the JSON content of mbmd app running on SmartCharger RasPi
QString m_JSONfiledata;


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
//    QApplication app(argc, argv);

    PowerNodeModel powerNodeModel;
    QQmlApplicationEngine engine;

    qmlRegisterSingletonInstance<PowerNodeModel>("Smarf.PowerNodeModel", 1, 0, "PowerNodeModel", &powerNodeModel);

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl)
    {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
