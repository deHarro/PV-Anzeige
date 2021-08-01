#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QApplication>

#include <qmqtt.h>

#include "PowerNodeModel.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    //QGuiApplication app(argc, argv);
    QApplication app(argc, argv);

    QMQTT::Client mqttClient;
    PowerNodeModel powerNodeModel(mqttClient);

    QQmlApplicationEngine engine;

    qmlRegisterSingletonInstance<PowerNodeModel>("Smarf.PowerNodeModel", 1, 0, "PowerNodeModel", &powerNodeModel);

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl)
    {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

// test
//    QLabel label("<img src=':/Icons/FF.png' />");
//    label.show();

//    QQuickView view;
//    QQmlEngine *engine1 = view.engine();
//    engine1->addImageProvider(QLatin1String("colors"), new FFImageProvider);
//    view.setSource(QUrl::fromLocalFile(QStringLiteral("main.qml")));
//    view.show();

    engine.load(url);

    return app.exec();
}

/*
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);

    PowerNodeModel powerNodeModel;

    // Register our component type with QML.
    qmlRegisterSingletonInstance<PowerNodeModel>("Smarf.PowerNodeModel", 1, 0, "PowerNodeModel", &powerNodeModel);

    int rc = 0;

    QQmlEngine engine;
    //QQmlApplicationEngine engine;
    QQmlComponent *component = new QQmlComponent(&engine);

    QObject::connect(&engine, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));


    QObject *topLevel = component->create();
    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

    component->loadUrl(QUrl("main.qml"));

    if (!component->isReady() ) {
        qWarning("%s", qPrintable(component->errorString()));
        return -1;
    }
    QSurfaceFormat surfaceFormat = window->requestedFormat();
    window->setFormat(surfaceFormat);
    window->show();

    rc = app.exec();

    delete component;
    return rc;
}
*/
