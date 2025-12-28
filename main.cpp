#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#ifdef Q_OS_WASM                                // wird für WebAssembly (WASM) übersetzt oder für Windows?
    #include <emscripten/html5.h>
#endif

#include "PowerNodeModel.h"

// globals ----------------------------------------
// global array for saving the XML content of SmartCharger app running on SmartCharger RasPi
QByteArray m_XMLfiledata;

// global array for saving the JSON content of mbmd app running on SmartCharger RasPi
QString m_JSONfiledata;

// global array for saving the JSON content of evcc app running on RasPi
QString m_JSONevccData;

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
// to make the App adapt to various screen sizes
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
   QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

#ifdef Q_OS_WASM                                // wird für WebAssembly (WASM) übersetzt oder für Windows?
    // ... in main() ...
    // Ersetzt das alte Flag-Verhalten:
    // 1. Argument: Die ID des Canvas im HTML (Standard ist "canvas")
    // 2. Argument: Breite
    // 3. Argument: Höhe
    // 4. Argument: Soll die CSS-Größe ebenfalls angepasst werden? (1 = Ja)
    emscripten_set_canvas_element_size("#canvas", 800, 600);    // to make the App adapt to various screen sizes
#endif
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
