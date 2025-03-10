#pragma once

#include <QObject>
#include <QTimer>
#include <QString>
#include <QQuickImageProvider>
#include <QColor>
#include <QMessageBox>

/*
Hinweis
Die Konfiguration der Wechselrichter erfolgt in der Datei mbmd.yaml auf dem RasPi

change log
Version  1.0 - erster Wurf, Funktion soweit OK
Version  1.1 - Werte im Programm an Werte aus Datenquellen angepasst (nur double wenn Kommazahlen übergeben werden, sonst int)
Version  1.2 - keine Mathe in QML, alle Berechnungen in C++, Ausgaben als Text
Version  1.3 - Sonne ändert die Farbe von Weiß nach Gelb kontinuierlich mit der Sonneneinstrahlung
Version  1.4 - consumptionPower hängt an EDLD und MBMD -> bei EDLD Probs. consumptionPower Rot färben
Version  1.5 - Fehlermeldung wenn einer der Wechselrichter keine Daten liefert (Modbus Fehler)
Version  1.6 - Zusätzlicher Wechselrichter Dach Nord vorbereitet, Anzeige des ChargeMode der Wallbox
Version  1.7 - Die ChargeModes sind per Mausclick umstellbar.
                Per MouseHover auf der Anzeige des aktuellen ChargeMode in der Wallbox den gewuenschten Modus selektieren
                und per MausClick aktivieren.
                Achtung: Die Wallbox braucht einige Sekunden, bis der neue Modus akzeptiert und zur Anzeige zurück geliefert wird.
Version  1.8 - Der maximale Strom bei manuellem Laden kann über die GUI eingestellt werden.
                Per MouseHover auf der Anzeige der Evaluation Points in der Wallbox den gewuenschten Wert selektieren.
                Der neue Wert wird unterhalb der "Eval. Points" angezeigt.
                Per MausClick wird der gerade angezeigte neue Wert aktiviert.
                Achtung: Die Wallbox braucht einige Sekunden, bis der neue Wert umgesetzt wird.
Version  1.9 - Verarbeitung der Daten von Wechselrichter 4 (DachN, V1.6) eingebaut aber per Define in WechselrichterJSON.h
                deaktiviert (sonst meldet PV-Anzeige einen Fehler "Mindestens einer der Wechselrichter liefert keine Daten").
Version 1.10 - Verarbeitung der Daten von Wechselrichter 4 (DachN, V1.6) aktiviert - läuft :)
Version 1.11 - Einblenden der (Gesamt-)Erträge aller Wechselrichter in einem Drawer (von rechts reinziehen)
Version 1.12 - Bei Klick auf die Sonne: Messagebox mit kopierbaren Ertragswerten der Wechselrichter - erspart abtippen :)
             - Ertragswerte für den Drawer in QString umgestellt -> Anzeige ausgerichtet und zentriert
Version 1.13 - Alle Einstellungen für den SmartCharger Raspberry Pi (V1.7 und V1.8) werden jetzt ebenfalls über
                Drawer von links und rechts im unteren Drittel (Wallbox) eingestellt. Das Hover mit der Maus ist hinfällig.
Version 1.14 - Pfeile am Rand für Drawer einblenden durch kleine graue Kreise ersetzt
             - dragMargin von 0,5 auf 0,25 windowWidth geändert (-> Button "Car/Bike Bilderwechsel" wieder erreichbar)
             - Farbe der Drawer auf das Grün der Boxen geändert
             - allgemein etwas aufgeräumt (auskommentierte Bereiche gelöscht, Kommentare angebracht, ...)
             - Drawer ManualCurrent: Initialwert für ManualCurrent beim Öffnen des Drawer sofort anzeigen
Version 1.15 - Texte und Werte in den drei Drawer weiß statt schwarz
             - automatisches Build Datum für die Caption für "April" korrigiert (war im April falsch "August")
Version 1.16 - Manual Current "6A" im Drawer auf "=" ausgerichtet
             - Die voreilig eingebauten Änderungen für Remote Zugriff auf den Prozentsatz der "EV ChargeRate" sind nicht 
                zielführend, der Prozentsatz kann nicht über das Remote Interface geändert werden. -> Auskommentiert
                (Wer weiß, vielleicht baut Nico das ja irgendwann mal in seinen EDL/SmartCharger ein ;)
             - Change log in Blockkommentar geändert
             - Version auf V1.16 geändert
*/

// program version for window title
#define VERSIONMAJOR    "1"
#define VERSIONMINOR    "16"

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
#define SUNWHITE        0               // "WHITE"         // weiß
#define SUNLTYELLOW     1               // "#FFFFA8"       // helles Gelb
#define SUNYELLOW       2               // "YELLOW"        // Gelb


class PowerNodeModel : public QObject {
    Q_OBJECT

public:
    PowerNodeModel();
    ~PowerNodeModel();

    // generator properties - all generator values are updated in one call to "generatorDataChanged"
    Q_PROPERTY(QString generatorPowerTotal   MEMBER m_genPowerTotal         NOTIFY generatorDataChanged)
    Q_PROPERTY(QString generatorPowerDachS   MEMBER m_genPowerDachS         NOTIFY generatorDataChanged)
    Q_PROPERTY(QString generatorPowerDachN   MEMBER m_genPowerDachN         NOTIFY generatorDataChanged)
    Q_PROPERTY(QString generatorPowerGaube   MEMBER m_genPowerGaube         NOTIFY generatorDataChanged)
    Q_PROPERTY(QString generatorPowerGarage  MEMBER m_genPowerGarage        NOTIFY generatorDataChanged)
    Q_PROPERTY(double generatorTotalEnergy   MEMBER m_generatorTotalEnergy  NOTIFY generatorDataChanged)
    Q_PROPERTY(QString genTotalEnergy        MEMBER m_genEnergyTotal        NOTIFY generatorDataChanged)
    Q_PROPERTY(QString generatorDachNEnergy  MEMBER m_genEnergyDachN        NOTIFY generatorDataChanged)
    Q_PROPERTY(QString generatorDachSEnergy  MEMBER m_genEnergyDachS        NOTIFY generatorDataChanged)
    Q_PROPERTY(QString generatorGaubeEnergy  MEMBER m_genEnergyGaube        NOTIFY generatorDataChanged)
    Q_PROPERTY(QString generatorGarageEnergy MEMBER m_genEnergyGarage       NOTIFY generatorDataChanged)
    Q_PROPERTY(QString generatorColor        MEMBER m_generatorColor        NOTIFY generatorDataChanged)
    Q_PROPERTY(QColor sunBGColor             MEMBER m_SunBGColor            NOTIFY generatorDataChanged)
    Q_PROPERTY(double sunAngle               MEMBER m_sunAngle              NOTIFY rotateSun)
    Q_PROPERTY(QString sunColor              MEMBER m_sunColor              NOTIFY sunColor)

    // battery properties - all battery values are updated in one call to "batteryDataChanged"
    Q_PROPERTY(QString batteryPower         MEMBER m_battPowerAnzeige      NOTIFY batteryDataChanged)
    Q_PROPERTY(double batteryPercentage     MEMBER m_batteryPercentage     NOTIFY batteryDataChanged)
    Q_PROPERTY(QString batteryText          MEMBER m_batteryText           NOTIFY batteryDataChanged)
    Q_PROPERTY(QString batteryColor         MEMBER m_batteryColor          NOTIFY batteryDataChanged)
    Q_PROPERTY(QString batteryImage         MEMBER m_batteryImage          NOTIFY batteryDataChanged)
    Q_PROPERTY(int batteryFill              MEMBER m_batteryFill           NOTIFY batteryDataChanged)
    Q_PROPERTY(double battTemp              MEMBER m_battTemp              NOTIFY batteryDataChanged)

    // home consumption properties - all home consumption values are updated in one call to "consumptionDataChanged"
    Q_PROPERTY(QString consumptionPower     MEMBER m_totPowerConsumption   NOTIFY consumptionDataChanged)
    Q_PROPERTY(QString homeColor            MEMBER m_homeColor             NOTIFY consumptionDataChanged)

    // shade properties
    Q_PROPERTY(int homeTopGreenH            MEMBER m_homeTopGreenH         NOTIFY shadeDataChanged)
    Q_PROPERTY(int homeBotRedH              MEMBER m_homeBotRedH           NOTIFY shadeDataChanged)

    // grid properties - all grid values are updated in one call to "gridDataChanged"
    Q_PROPERTY(QString gridText             MEMBER m_gridText         NOTIFY gridDataChanged)
    Q_PROPERTY(QString gridColor            MEMBER m_gridColor        NOTIFY gridDataChanged)
    Q_PROPERTY(QString gridPower            MEMBER m_gridPowerAnzeige NOTIFY gridDataChanged)                   // current power from/to grid
    Q_PROPERTY(double gridEnergyImport      MEMBER m_gridEnergyImport NOTIFY gridDataChanged)                   // Verbrauchszähler [kWh]
    Q_PROPERTY(double gridEnergyExport      MEMBER m_gridEnergyExport NOTIFY gridDataChanged)                   // Einspeisezähler [kWh]

    // wallbox properties - all wallbox values are updated in one call to "chargingDataChanged"
    Q_PROPERTY(QString chargingPower        MEMBER m_charPower        NOTIFY chargingDataChanged)                // current power [kW]
    Q_PROPERTY(QString chargedEnergy        MEMBER m_charEnergy       NOTIFY chargingDataChanged)                // total energy [kWh]
    Q_PROPERTY(QString sessionEnergy        MEMBER m_sessEnergy       NOTIFY chargingDataChanged)                // last session energy [kWh]
    Q_PROPERTY(int evalPoints               MEMBER m_evalPoints       NOTIFY chargingDataChanged)
    Q_PROPERTY(QString wallboxColor         MEMBER m_wallboxColor     NOTIFY chargingDataChanged)
    Q_PROPERTY(QString wallboxCar           MEMBER m_wallboxCar       NOTIFY chargingDataChanged)
    Q_PROPERTY(QString wallboxScoot         MEMBER m_wallboxScoot     NOTIFY chargingDataChanged)
    Q_PROPERTY(QString chargeMode           MEMBER m_EVChargingMode   NOTIFY chargingDataChanged)                // current charge mode (String)
    Q_PROPERTY(double manualCurrent         MEMBER m_EVManualCurrent  NOTIFY chargingDataChanged)                // charging current for manual
    Q_PROPERTY(QString manualCurrentS       MEMBER m_EVManualCurrentS NOTIFY chargingDataChanged)                // current charge mode (String)
    Q_PROPERTY(bool visibleComm             MEMBER m_visibleComm      NOTIFY showComm)
    Q_PROPERTY(char evalCountDown           MEMBER m_evalCountDown    NOTIFY showComm)
    //Q_PROPERTY(QString EVChargePercentS     MEMBER m_EVChargePercentS NOTIFY chargingDataChanged)                // current EV percentage (String)

    // color of power values (red/white if no/connection to SmartCharger on RasPi)
    Q_PROPERTY(QString EDLDfigures  MEMBER  m_EDLDfigures  NOTIFY setBackgroundColor)

    // color of power values (red/white if no/connection to MBMD on RasPi)
    Q_PROPERTY(QString MBMDfigures  MEMBER  m_MBMDfigures  NOTIFY setBackgroundColor)

    // color of consumptionPower (red/white if no/connection to SmartCharger on RasPi)
    Q_PROPERTY(QString consumptionPowerfigures  MEMBER  m_consumptionPowerfigures  NOTIFY setBackgroundColor) // consumptionPower hängt an EDLD _und_ MBMD, 2022-05-26

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
    Q_PROPERTY(QString WRProblemText   MEMBER m_WRProblemText   NOTIFY setWRWarning)
    Q_PROPERTY(QString backgroundColor MEMBER m_backgroundColor NOTIFY setBackgroundColor)

    Q_PROPERTY(QString windowTitle  MEMBER m_windowTitle NOTIFY displayWindowTitle)

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
    void setWRWarning();
    void setBackgroundColor();

    void rotateSun();
    void sunColor();
    void showComm();
    void displayWindowTitle();

public slots:
    void switchEVIcons();                   // change visualisation of car/scooter (icon or real picture)
    void switchChargeMode();                // send (new) chargeMode setting to SmartCharger
    void showChargeModeOFF();               // display (potentially new) chargeMode in GUI on hover
    void showChargeModeQUICK();             // display (potentially new) chargeMode in GUI on hover
    void showChargeModeSURPLUS();           // display (potentially new) chargeMode in GUI on hover
    void showChargeModeMANUAL();            // display (potentially new) chargeMode in GUI on hover
    void showChargeMode();                  // display currently selected ChargeMode
    void switchManualCurrent();             // send (new) manual current to SmartCharger
    void setManualCurrent6000();           // display ManualChargeCurrent 6 A in GUI on hover
    void setManualCurrent12000();          // display ManualChargeCurrent 12 A in GUI on hover
    void setManualCurrent18000();          // display ManualChargeCurrent 18 A in GUI on hover
    void showManualCurrent();              // display currently selected ManualChargeCurrent
/*
    void showEVPercent();                  // display currently selected ratio car battery/house battery (0..50.100%)
    void switchEVChargePercent();           // send (new) manual current to SmartCharger
    void setEVPercent10();                   // display EVPercent 0 GUI on hover
    void setEVPercent50();                   // display EVPercent 0 GUI on hover
    void setEVPercent100();                  // display EVPercent 0 GUI on hover
*/
    void openPopUpMsg();                    // Anzeige der Erträge aller WR und Gesamt

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
    void getIconType(void);
    void loadSmChXML(void);                 //
    void setMBMDText(void);
    void setEDLDText(void);
    void setWRText(void);
    void setBGColor(void);
    void setSunAngle(void);
    void setSunColor(int8_t);
    void setComm(void);
    void resetComm(void);
    void setWindowTitle(void);
    void countDown(void);

public:
    QString getChargeModeString(void);

// window title with version & build date
#define WINDOWTITLE "PV-Anzeige - V" VERSIONMAJOR "." VERSIONMINOR " - "
    QString m_windowTitle = WINDOWTITLE;

// generators, PV-Paneele
    QString m_genPowerTotal = 0;            // Momentanleistung String gesamt [kW]
    int m_generatorPowerTotal = 0;          // Momentanleistung int gesamt [kW]
    QString m_genPowerDachS = 0;            // Momentanleistung String Dach Süd
    QString m_genPowerDachN = 0;            // Momentanleistung String Dach Nord
    int m_generatorPowerDachS = 0;          // Momentanleistung int Dach Süd
    int m_generatorPowerDachN = 0;          // Momentanleistung int Dach Nord
    QString m_genPowerGaube = 0;            // Momentanleistung String Gaube
    int m_generatorPowerGaube = 0;          // Momentanleistung int Gaube
    QString m_genPowerGarage = 0;           // Momentanleistung String Garage
    int m_generatorPowerGarage = 0;         // Momentanleistung int Garage
    double m_generatorTotalEnergy = 0.0;    // Gesamtertrag PV-Anlage
    QString m_genEnergyTotal = 0;           // Gesamtertrag String PV-Anlage
    double m_generatorDachNEnergy = 0.0;    // Gesamtertrag WR DachN
    QString m_genEnergyDachN = 0;           // Gesamtertrag String WR DachN
    double m_generatorDachSEnergy = 0.0;    // Gesamtertrag WR DachS
    QString m_genEnergyDachS = 0;           // Gesamtertrag String WR DachS
    double m_generatorGaubeEnergy = 0.0;    // Gesamtertrag WR Gaube
    QString m_genEnergyGaube = 0;           // Gesamtertrag String WR Gaube
    double m_generatorGarageEnergy = 0.0;   // Gesamtertrag WR Garage
    QString m_genEnergyGarage = 0;          // Gesamtertrag String WR Garage
    QString m_generatorColor = VLIGHTGRAY;  // Farbe der PV Generator Box
    double m_sunAngle = 22.5;               // Sonne langsam rotieren ;)
    QString m_sunColor = "/Icons/Sonne_invers_hellgrau.png";          // 2022-05-26
    QColor m_SunBGColor = "#ffffff";

// battery, Akku
    int m_batteryPower = 0;                 // Batterieladung/-Entladung [kW]
    QString m_battPowerAnzeige = 0;         // Batterieladung/-Entladung zur Anzeige [kW]
    int m_batteryPercentage = 0;            // Batterie Ladezustand [%]
    QString m_batteryColor = VLIGHTGRAY;    // Farbe der Akku Box
    QString m_batteryText = "";             // Text in der Batterie Box, wechselt Ladung/Entladung
    QString m_batteryImage = "/Icons/Akku_weiss_transparent00.png";
    int m_batteryFill = 0;                  // analoge Balkenanzeige des Betteriefüllstands
    double m_battTemp = 0.0;                // Temperatur des Akkus
// consumption, home, Hausverbrauch
    int m_totalPowerConsumption = 0;        // Gesamtverbrauch [kW]
    QString m_totPowerConsumption = 0;      // Gesamtverbrauch [kW]
    QString m_homeColor = VLIGHTGRAY;       // Farbe der Haus Box
    int m_homeBotRedH = 0;                  // anteilige Energie aus Netzbezug (roter Balken von oben wachsend)
    int m_homeTopGreenH = 0;                // anteilige Energie aus Akku (dunkelgrüner Balken von unten wachsend)
// grid, Netz
    int m_gridPower = 0;                    // Netzbezug/Einspeisung [kW]
    QString m_gridPowerAnzeige = 0;         // Netzbezug für die Anzeige
    double m_gridEnergyImport = 0.0;        // Zähler [kWh]
    double m_gridEnergyExport = 0.0;        // Einspeisezähler [kWh]
    QString m_gridColor = VLIGHTGRAY;       // Farbe der Grid Box
    QString m_gridText = "";                // Text in der grid Box, wechselt Einspeisung/Bezug
// Wallbox
    int m_chargingPower = 0;                // current power [kW]
    QString m_charPower = 0;                // current power [kW]
    int m_chargedEnergy = 0;                // total energy [kWh]
    QString m_charEnergy = 0;               // total energy [kWh]
    int m_sessionEnergy = 0;                // last session energy [kWh]
    QString m_sessEnergy = 0;               // last session energy [kWh]
    int m_evalPoints = 0;                   // Evaluation Points (when to start charging)
    QString m_wallboxColor = VLIGHTGRAY;    // Farbe der Wallbox Box
    QString m_wallboxCar =      "Icons/electric-car-icon_weiss_transparent.png";            // default mit Stecker in der Luft
    QString m_wallboxScoot =    "Icons/electric-scooter_icon_weiss_transparent_rad.png";    // default mit Stecker in der Luft
    bool m_visibleComm = false;             // WLAN marker shown if RPi is interrogated
    bool m_realPics = false;                // no real pictures of EVs
    char m_evalCountDown = 60;              // count down 1 minute
    QString m_EVChargingMode;               // ChargeMode (OFF, SURPLUS, QUICK, MANUAL)
    char m_CurrChargeMode;
    char m_nextChargeMode = 0;              // set ChargeMode der Wallbox via SmartCharger
    double m_EVManualCurrent = 18000.0;     // set ManualCurrent der Wallbox via SmartCharger (default 18 A = 4140 W)
    QString m_EVManualCurrentS;             // String für Anzeige in der GUI
    //char m_EVChargePercent = 100;           // charge percentage into EV battery: 100..50..0 [%]
    //QString m_EVChargePercentS;             // String für Anzeige in der GUI

// Error Messages
    QString m_MBMDProblemText = "";
    QString m_EDLDProblemText = "";
    QString m_WRProblemText = "";
    QString m_backgroundColor = "whitesmoke";

// Figures color of values, RED if error on reading values, WHITE if readings are ok
    QString m_EDLDfigures = "red";
    QString m_MBMDfigures = "red";
    QString m_consumptionPowerfigures = "red";  // consumptionPower hängt an EDLD _und_ MBMD, 2022-05-26

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

