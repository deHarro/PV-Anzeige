#pragma once

#include <QObject>
#include <QTimer>
#include <QString>
#include <QQuickImageProvider>

#define DEMOMODE                // generate random power values for coloring and arrows

// define colors according https://doc.qt.io/qt-5/qml-color.html
// use #RRGGBB notation for QML instead of 0xRRGGBB
#define VLIGHTGRAY      "#b3b3b3"     // "LIGHTGREY"    //  no defined color, hand tuned ;-)
//#define LIMEGREEN       "LIMEGREEN"   // "0x32cd32"     // -> "#32cd32" replace 0x by # for QColor in QML file
#define LIMEGREEN       "#00ac00"   // "0x32cd32"     // -> "#32cd32" replace 0x by # for QColor in QML file
#define FORESTGREEN     "FORESTGREEN" // "0x228b22"     // "0x228b22"
#define FIREBRICK       "FIREBRICK"   // "0xb22222"
//#define DODGERBLUE      "#0A7CEB"     // "DODGERBLUE"   // EV charging // 0x0A7CEB = "um -20 dunkleres dogerblue" (Orig. 0x1e90ff)
#define DODGERBLUE      "#02a4f5"     // "DODGERBLUE"   // EV charging // 0x0A7CEB = "um -20 dunkleres dogerblue" (Orig. 0x1e90ff)
//#define DARKBLUE        "DARKBLUE"    // "0x00008b"     // EV attached to wallbox
#define DARKBLUE        "#0371da"    // "0x00008b"     // EV attached to wallbox


class PowerNodeModel : public QObject {
    Q_OBJECT

public:
    PowerNodeModel();
    ~PowerNodeModel();

    // generator properties - all generator values are updated in one call to "generatorDataChanged"
    Q_PROPERTY(double generatorPowerTotal MEMBER m_generatorPowerTotal NOTIFY generatorDataChanged)
    Q_PROPERTY(double generatorPowerDach MEMBER m_generatorPowerDach NOTIFY generatorDataChanged)
    Q_PROPERTY(double generatorPowerGaube MEMBER m_generatorPowerGaube NOTIFY generatorDataChanged)
    Q_PROPERTY(double generatorPowerGarage MEMBER m_generatorPowerGarage NOTIFY generatorDataChanged)
    Q_PROPERTY(QString generatorColor MEMBER m_generatorColor NOTIFY generatorDataChanged)

    // battery properties - all battery values are updated in one call to "batteryDataChanged"
    Q_PROPERTY(double batteryPower MEMBER m_battPowerAnzeige NOTIFY batteryDataChanged)
    Q_PROPERTY(double batteryPercentage MEMBER m_batteryPercentage NOTIFY batteryDataChanged)
    Q_PROPERTY(QString batteryText MEMBER m_batteryText NOTIFY batteryDataChanged)    
    Q_PROPERTY(QString batteryColor MEMBER m_batteryColor NOTIFY batteryDataChanged)
    Q_PROPERTY(QString batteryImage MEMBER m_batteryImage NOTIFY batteryDataChanged)

    // home consumption properties - all home consumption values are updated in one call to "consumptionDataChanged"
    Q_PROPERTY(double consumptionPower MEMBER m_totalPowerConsumption NOTIFY consumptionDataChanged)
    Q_PROPERTY(double consumptionEnergy MEMBER m_totalEnergyConsumption NOTIFY consumptionDataChanged)
    Q_PROPERTY(QString homeColor MEMBER m_homeColor NOTIFY consumptionDataChanged)

    // grid properties - all grid values are updated in one call to "gridDataChanged"
    Q_PROPERTY(double gridPower MEMBER m_gridPowerAnzeige NOTIFY gridDataChanged)
    Q_PROPERTY(double gridEnergyImport MEMBER m_gridEnergyImport NOTIFY gridDataChanged)                    // Verbrauchszähler [kWh]
    Q_PROPERTY(double gridEnergyExport MEMBER m_gridEnergyExport NOTIFY gridDataChanged)                    // Einspeisezähler [kWh]
    Q_PROPERTY(QString gridText MEMBER m_gridText NOTIFY gridDataChanged)
    Q_PROPERTY(QString gridColor MEMBER m_gridColor NOTIFY gridDataChanged)

    // wallbox properties - all wallbox values are updated in one call to "chargingDataChanged"
    Q_PROPERTY(double chargingPower MEMBER m_chargingPower NOTIFY chargingDataChanged)                      // current power [kW]
    Q_PROPERTY(double chargedEnergy MEMBER m_chargedEnergy NOTIFY chargingDataChanged)                      // total energy [kWh]
    Q_PROPERTY(double sessionEnergy MEMBER m_sessionEnergy NOTIFY chargingDataChanged)                      // last session energy [kWh]
    Q_PROPERTY(QString wallboxColor MEMBER m_wallboxColor NOTIFY chargingDataChanged)

    // arrow properties - all arrows are updated in one call to "arrowsDataChanged"
    Q_PROPERTY(bool batt2house MEMBER m_batt2house  NOTIFY arrowsDataChanged)
    Q_PROPERTY(bool house2batt MEMBER m_house2batt  NOTIFY arrowsDataChanged)
    Q_PROPERTY(bool grid2house MEMBER m_grid2house  NOTIFY arrowsDataChanged)
    Q_PROPERTY(bool pv2house MEMBER m_pv2house  NOTIFY arrowsDataChanged)
    Q_PROPERTY(bool pv2batt MEMBER m_pv2batt  NOTIFY arrowsDataChanged)
    Q_PROPERTY(bool pv2grid MEMBER m_pv2grid  NOTIFY arrowsDataChanged)
    Q_PROPERTY(bool house2charger MEMBER m_house2charger NOTIFY arrowsDataChanged)

Q_SIGNALS:
    void generatorDataChanged();
    void batteryDataChanged();
    void consumptionDataChanged();
    void gridDataChanged();
    void chargingDataChanged();
    void arrowsDataChanged();

private:
    void onConnected();
    void onDisconnected();

    void generatorHandling(void);
    void batteryHandling(void);
    void gridHandling(void);
    void wallboxHandling(void);
    void consumptionHandling(void);
    void arrowsHandling(void);


// generators, PV-Paneele
    double m_generatorPowerTotal = 0.0;     // Momentanleistung gesamt [kW]
    double m_generatorPowerDach = 0.0;      // Momentanleistung String Dach
    double m_generatorPowerGaube = 0.0;     // Momentanleistung String Gaube
    double m_generatorPowerGarage = 0.0;    // Momentanleistung String Garage
    QString m_generatorColor = VLIGHTGRAY;
// battery, Akku
    double m_batteryPower = 0.0;            // Batterieladung/-Entladung [kW]
    double m_battPowerAnzeige = 0.0;        // Batterieladung/-Entladung zur Anzeige [kW]
    int m_batteryPercentage = 0;            // Batterie Ladezustand [%]
    QString m_batteryColor = VLIGHTGRAY;
    QString m_batteryText = "";             // Text in der Batterie Box, wechselt Ladung/Entladung
    QString m_batteryImage = "/Icons/Akku_weiss_transparent00.png";
// consumption, home, Hausverbrauch
    double m_totalPowerConsumption = 0.0;   // Gesamtverbrauch [kW]
    double m_totalEnergyConsumption = 0.0;  // Gesamtverbrauch aus Netz und Akku und PV - woher kommt dieser Wert?
    QString m_homeColor = VLIGHTGRAY;
// grid, Netz
    double m_gridPower = 0.0;               // Netzbezug/Einspeisung [kW]
    double m_gridPowerAnzeige = 0.0;        // Netzbezug für die Anzeige
    double m_gridEnergyImport = 0.0;        // Zähler [kWh]
    double m_gridEnergyExport = 0.0;        // Einspeisezähler [kWh]
    QString m_gridColor = VLIGHTGRAY;
    QString m_gridText = "";                // Text in der grid Box, wechselt Einspeisung/Bezug
// Wallbox
    bool m_evAttached = false;              // wallbox delivers the information whether any vehicle is attached to the outlet
    double m_chargingPower = 0.0;           // current power [kW]
    double m_chargedEnergy = 0.0;           // total energy [kWh]
    double m_sessionEnergy = 0.0;           // last session energy [kWh]
    QString m_wallboxColor = VLIGHTGRAY;

// arrow handling
    bool m_pv2batt = false;
    bool m_pv2house = false;
    bool m_pv2grid = false;
    bool m_batt2house /*= false*/;
    bool m_house2batt = false;
    bool m_grid2house = false;
    bool m_house2charger = false;

    // Members for demo purposes
    QTimer m_dataTimer;
    void onDataTimer();
};

// test --------------------------------------------------------------------

//class FFImageProvider : public QQuickImageProvider {
//public:
//    FFImageProvider()
//               : QQuickImageProvider(QQuickImageProvider::Pixmap)
//    {
//    }

//    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override
//    {
//       int width = 37;
//       int height = 36;

//       if (size)
//          *size = QSize(width, height);
//       QPixmap pixmap(requestedSize.width() > 0 ? requestedSize.width() : width,
//                      requestedSize.height() > 0 ? requestedSize.height() : height);
//       //pixmap.fill(QColor(id).rgba());
//       return pixmap;
//    }
//};

