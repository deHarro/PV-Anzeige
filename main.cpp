#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "PowerNodeModel.h"

// globals ----------------------------------------
// global array for saving the XML content of SmartCharger app running on SmartCharger RasPi
QByteArray m_XMLfiledata;

// global array for saving the JSON content of mbmd app running on SmartCharger RasPi
QString m_JSONfiledata;

// global flag memory for error messages
// Flag: Bit 0 = 1 -> EDLD antwortet nicht korrekt
// Flag: Bit 1 = 1 -> MBMD antwortet nicht korrekt
quint8 m_messageFlag = 0;

// global value for changing the ChargeMode on SmartCharger RasPi
QString m_setChargeModeString;                      // Befehl für remote control des SmartCharger (off, quick, surplus, manual);
int m_ManualSetCurrent;                             // Befehl für remote control des SmartCharger (manual current 6, 12, 18, 32 A)
QString m_EVChargingModeS;                          // Anzeige der manuell einzustellenden maximalen Ladeleistung
int m_ChargerPhases;                                // Befehl für remote control des SmartCharger (number of phases 1, 3)
// \globals ----------------------------------------


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
   QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    // to make the App adapt to various screen sizes
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Ceil);

    //QGuiApplication app(argc, argv); // Für Messagebox muss QApplication verwendet werden
    QApplication app(argc, argv); // Für Messagebox muss QApplication verwendet werden

    PowerNodeModel powerNodeModel;
    QQmlApplicationEngine engine;

    qmlRegisterSingletonInstance<PowerNodeModel>("PowerNodeModel", 1, 0, "PowerNodeModel", &powerNodeModel);

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl)
    {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
