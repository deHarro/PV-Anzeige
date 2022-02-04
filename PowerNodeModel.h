#pragma once

#include <QObject>
#include <QTimer>
#include <QString>
#include <QQuickImageProvider>

//#define DEMOMODE              // generate random power values for checking coloring and arrows

class StringData;

// define colors according https://doc.qt.io/qt-5/qml-color.html
// use #RRGGBB notation for QML instead of 0xRRGGBB
#define VLIGHTGRAY      "#b3b3b3"       // "LIGHTGREY"    //  no defined color, hand tuned ;-)
#define LIMEGREEN       "#00ac00"       // "0x32cd32"     // -> "#32cd32" replace 0x by # for QColor in QML file
#define FORESTGREEN     "FORESTGREEN"   // "0x228b22"
#define FIREBRICK       "FIREBRICK"     // "0xb22222"
#define DODGERBLUE      "#02a4f5"       // "DODGERBLUE"   // EV charging // 0x0A7CEB = "um -20 dunkleres dogerblue" (Orig. 0x1e90ff)
#define DARKBLUE        "#2828B3"       // "#0371da" "0x00008b"     // EV attached to wallbox
//#define LIGHTBLUE       "#49C6E5"       // sehr helles Blau
//#define LIGHTBLUE       "#00C3FF"       // sehr helles Blau
//#define LIGHTBLUE       "#A3D1FF"       // sehr helles Blau
#define LIGHTBLUE       "#72BBFF"       // sehr helles Blau
#define LIGHTHRED       "#FFE5E5"       // sehr helles Rot


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
    Q_PROPERTY(double generatorTotalEnergy MEMBER m_generatorTotalEnergy NOTIFY generatorDataChanged)
    Q_PROPERTY(QString generatorColor MEMBER m_generatorColor NOTIFY generatorDataChanged)
    Q_PROPERTY(double sunAngle MEMBER m_sunAngle NOTIFY rotateSun)

    // battery properties - all battery values are updated in one call to "batteryDataChanged"
    Q_PROPERTY(double batteryPower MEMBER m_battPowerAnzeige NOTIFY batteryDataChanged)
    Q_PROPERTY(double batteryPercentage MEMBER m_batteryPercentage NOTIFY batteryDataChanged)
    Q_PROPERTY(QString batteryText MEMBER m_batteryText NOTIFY batteryDataChanged)
    Q_PROPERTY(QString batteryColor MEMBER m_batteryColor NOTIFY batteryDataChanged)
    Q_PROPERTY(QString batteryImage MEMBER m_batteryImage NOTIFY batteryDataChanged)
    Q_PROPERTY(int batteryFill MEMBER m_batteryFill NOTIFY batteryDataChanged)
    Q_PROPERTY(double battTemp MEMBER m_battTemp NOTIFY batteryDataChanged)

    // home consumption properties - all home consumption values are updated in one call to "consumptionDataChanged"
    Q_PROPERTY(double consumptionPower MEMBER m_totalPowerConsumption NOTIFY consumptionDataChanged)
    Q_PROPERTY(double consumptionEnergy MEMBER m_totalEnergyConsumption NOTIFY consumptionDataChanged)
    Q_PROPERTY(QString homeColor MEMBER m_homeColor NOTIFY consumptionDataChanged)

    // shade properties
    Q_PROPERTY(int homeTopGreenH MEMBER m_homeTopGreenH NOTIFY shadeDataChanged)
    Q_PROPERTY(int homeBotRedH MEMBER m_homeBotRedH NOTIFY shadeDataChanged)

    // grid properties - all grid values are updated in one call to "gridDataChanged"
    Q_PROPERTY(QString gridText         MEMBER m_gridText         NOTIFY gridDataChanged)
    Q_PROPERTY(QString gridColor        MEMBER m_gridColor        NOTIFY gridDataChanged)
    Q_PROPERTY(double gridPower         MEMBER m_gridPowerAnzeige NOTIFY gridDataChanged)
    Q_PROPERTY(double gridEnergyImport  MEMBER m_gridEnergyImport NOTIFY gridDataChanged)                    // Verbrauchszähler [kWh]
    Q_PROPERTY(double gridEnergyExport  MEMBER m_gridEnergyExport NOTIFY gridDataChanged)                    // Einspeisezähler [kWh]

    // wallbox properties - all wallbox values are updated in one call to "chargingDataChanged"
    Q_PROPERTY(double chargingPower MEMBER m_chargingPower NOTIFY chargingDataChanged)                      // current power [kW]
    Q_PROPERTY(double chargedEnergy MEMBER m_chargedEnergy NOTIFY chargingDataChanged)                      // total energy [kWh]
    Q_PROPERTY(double sessionEnergy MEMBER m_sessionEnergy NOTIFY chargingDataChanged)                      // last session energy [kWh]
    Q_PROPERTY(double evalPoints    MEMBER m_evalPoints    NOTIFY chargingDataChanged)
    Q_PROPERTY(QString wallboxColor MEMBER m_wallboxColor  NOTIFY chargingDataChanged)
    Q_PROPERTY(QString wallboxCar   MEMBER m_wallboxCar    NOTIFY chargingDataChanged)
    Q_PROPERTY(QString wallboxScoot MEMBER m_wallboxScoot  NOTIFY chargingDataChanged)
    Q_PROPERTY(bool visibleComm     MEMBER m_visibleComm   NOTIFY showComm)

    // color of power values (red/white if no/connection to SmartCharger on RasPi)
    Q_PROPERTY(QString EDLDfigures  MEMBER  m_EDLDfigures  NOTIFY setBackgroundColor)

    // color of power values (red/white if no/connection to MBMD on RasPi)
    Q_PROPERTY(QString MBMDfigures  MEMBER  m_MBMDfigures  NOTIFY setBackgroundColor)

    // arrow properties - all arrows are updated in one call to "arrowsDataChanged"
    Q_PROPERTY(bool batt2house      MEMBER m_batt2house    NOTIFY arrowsChanged)
    Q_PROPERTY(bool house2batt      MEMBER m_house2batt    NOTIFY arrowsChanged)
    Q_PROPERTY(bool grid2house      MEMBER m_grid2house    NOTIFY arrowsChanged)
    Q_PROPERTY(bool house2grid      MEMBER m_house2grid    NOTIFY arrowsChanged)
    Q_PROPERTY(bool pv2house        MEMBER m_pv2house      NOTIFY arrowsChanged)
    Q_PROPERTY(bool pv2batt         MEMBER m_pv2batt       NOTIFY arrowsChanged)
    Q_PROPERTY(bool pv2grid         MEMBER m_pv2grid       NOTIFY arrowsChanged)
    Q_PROPERTY(bool house2charger   MEMBER m_house2charger NOTIFY arrowsChanged)

    Q_PROPERTY(QString EDLDProblemText MEMBER m_EDLDProblemText NOTIFY setEDLDWarning)
    Q_PROPERTY(QString MBMDProblemText MEMBER m_MBMDProblemText NOTIFY setMBMDWarning)
    Q_PROPERTY(QString backgroundColor MEMBER m_backgroundColor NOTIFY setBackgroundColor)

    Q_PROPERTY(QString windowTitle MEMBER m_windowTitle NOTIFY displayWindowTitle)

Q_SIGNALS:
    void generatorDataChanged();
    void batteryDataChanged();
    void consumptionDataChanged();
    void gridDataChanged();
    void chargingDataChanged();
    void arrowsChanged();
    void shadeDataChanged();

    void setEDLDWarning();
    void setMBMDWarning();
    void setBackgroundColor();

    void rotateSun();
    void showComm();
    void displayWindowTitle();

private:
    void getXMLdata(void);
    void getJSONdata(void);
    void generatorHandling(void);
    void batteryHandling(void);
    void gridHandling(void);
    void wallboxHandling(void);
    void consumptionHandling(void);
    void arrowsHandling(void);
    void shadeHandling(void);
    void loadSmChXML(void);               //
    void setMBMDText(void);
    void setEDLDText(void);
    void setBGColor(void);
    void setSunAngle(void);
    void setComm(void);
    void resetComm(void);
    void setWindowTitle(void);

// window title with version & build date
    QString m_windowTitle = "PV-Anzeige - V1.0 - ";

// generators, PV-Paneele
    double m_generatorPowerTotal = 0.0;     // Momentanleistung gesamt [kW]
    double m_generatorPowerDach = 0.0;      // Momentanleistung String Dach
    double m_generatorPowerGaube = 0.0;     // Momentanleistung String Gaube
    double m_generatorPowerGarage = 0.0;    // Momentanleistung String Garage
    double m_generatorTotalEnergy = 0.0;    // Gesamtertrag der PV-Anlage
    QString m_generatorColor = VLIGHTGRAY;
    double m_sunAngle = 22.5;                  // Sonne langsam rotieren ;)

// battery, Akku
    double m_batteryPower = 0.0;            // Batterieladung/-Entladung [kW]
    double m_battPowerAnzeige = 0.0;        // Batterieladung/-Entladung zur Anzeige [kW]
    int m_batteryPercentage = 0;            // Batterie Ladezustand [%]
    QString m_batteryColor = VLIGHTGRAY;
    QString m_batteryText = "";             // Text in der Batterie Box, wechselt Ladung/Entladung
    QString m_batteryImage = "/Icons/Akku_weiss_transparent00.png";
    int m_batteryFill = 0;                  // analoge Balkenanzeige des Betteriefüllstands
    double m_battTemp = 0.0;                // Temperatur des Akkus
// consumption, home, Hausverbrauch
    double m_totalPowerConsumption = 0.0;   // Gesamtverbrauch [kW]
    double m_totalEnergyConsumption = 0.0;  // Gesamtverbrauch aus Netz und Akku und PV - woher kommt dieser Wert?
    QString m_homeColor = VLIGHTGRAY;
    int m_homeBotRedH = 0;                  // anteilige Energie aus Netzbezug (roter Balken von oben wachsend)
    int m_homeTopGreenH = 0;                // anteilige Energie aus Akku (dunkelgrüner Balken von unten wachsend)
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
    int m_evalPoints = 0;                   // Evaluation Points (when to start charging)
    QString m_wallboxColor = VLIGHTGRAY;
    QString m_wallboxCar =      "Icons/electric-car-icon_weiss_transparent.png";            // default mit Stecker in der Luft
    QString m_wallboxScoot =    "Icons/electric-scooter_icon_weiss_transparent_rad.png";    // default mit Stecker in der Luft
    bool m_visibleComm = false;             // WLAN marker shown if RPi is interrogated

// Error Messages
    QString m_MBMDProblemText = "";
    QString m_EDLDProblemText = "";
    QString m_backgroundColor = "whitesmoke";

// Figures color of values, RED if error on reading values, WHITE if readings are ok
    QString m_EDLDfigures = "red";
    QString m_MBMDfigures = "red";

// arrow handling
    bool m_pv2batt = false;
    bool m_pv2house = false;
    bool m_pv2grid = false;
    bool m_batt2house = false;
    bool m_house2batt = false;
    bool m_grid2house = false;
    bool m_house2grid = false;
    bool m_house2charger = false;

    // Members for demo purposes
    QTimer m_dataTimer;
    void onDataTimer();

};

