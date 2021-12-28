#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QApplication>

#include "PowerNodeModel.h"

#ifdef USE_MQTT
#include <qmqtt.h>
#endif

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    //QGuiApplication app(argc, argv);
    QApplication app(argc, argv);

#ifndef USE_MQTT
     PowerNodeModel powerNodeModel;
#else
    QMQTT::Client mqttClient;
    PowerNodeModel powerNodeModel(mqttClient);
#endif
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
