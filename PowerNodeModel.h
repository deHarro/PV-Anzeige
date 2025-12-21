#pragma once

#include <QTimer>
#include <QColor>
#include <QMessageBox>
#include <EvccJSON.h>

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
Version 1.15 - Texte und Werte der drei Drawer weiß statt schwarz
             - automatisches Build Datum für die Caption für "April" korrigiert (war im April falsch "August")
Version 1.16 - Manual Current "6A" im Drawer auf "=" ausgerichtet
             - Die voreilig eingebauten Änderungen für Remote Zugriff auf den Prozentsatz der "EV ChargeRate" sind nicht 
                zielführend, der Prozentsatz kann nicht über das Remote Interface geändert werden. -> Auskommentiert
                (Wer weiß, vielleicht baut Nico das ja irgendwann mal in seinen EDL/SmartCharger ein ;)
             - Change log in Blockkommentar geändert
Version 1.17 - Real Car Icon aktualisiert
             - Auslesen des Parameters MaxPhases der Wallbox. Einstellung (Setting), wieviele Phasen die Wallbox an das EV
                weiterleiten darf. Muss in SmartCharger auf "3" stehen. Wird nicht weiter genutzt/ausgewertet/angezeigt.
             - Auslesen des Parameters Output der Wallbox. Liest den Status des Ausgangs X2 der Wallbox ein.
                X2 steuert das Phasen-Umschaltrelais für die Wallbox. Das Relais schaltet bei Bedarf die Phasen L2 und L3
                zur Wallbox durch, so dass auch 3-phasig geladen werden kann. Das ist der Normalfall.
                Interessant im Zusammenhang mit Überschussladen per PV, gesteuert durch SmartCharger.
                Liegt/fällt der Sonnen_überschuss_ unter 3 * 1370 W (4,11 kW), wird auf 1-phasiges Laden umgeschaltet.
             - Anzeige der Anzahl genutzter Phasen in der GUI.
             - Maxwert für Manual Current in der GUI (Drawer links unten) von 18 A auf 32 A / 7.36 kW erhöht (nur bei 3 Phasen).
             - Die vertikale Lokalisierung der Drag-Punkte ist bei Qt V6.x weg, es gilt die komplette seitliche Kante für den zuerst
                in main.qml deklarierten Drawer -> Rechts wird nur der Charge Mode Drawer ausgeklappt. Nicht gut.
                -> Die Dragpunkte ersetzt durch runde Buttons, die per Mausklick den jeweiligen Drawer aufklappen.
             - Die (unsichtbaren) Buttons für die Drawer sind deutlich größer als die Dragpunkte -> leichter zu treffen :)
             - Drawer schließen per Mausklick außerhalb der Drawerfläche.
             - Upscaling der App für Qt V6.x aktiviert (Darstellung auf Tablet auf Bildschirmhöhe gestreckt),
                in main.cpp: QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Ceil);
                Das war bei Qt V5.x als Default aktiviert.
             - Box mit WR-Werten aufgehübscht, Aufruf geändert (Click auf WR-Werte), Werte können selektiert und kopiert werden.
             - Box mit Angabe der Version, Build-Date und -Time, Compiler- und Runtimeversion (Click auf Sonne).
             - Alle unsichtbaren Buttons bleiben unsichtbar wenn geklickt ("opacity: 0" in main.qml).
Version 1.18 - Pfad auf PVConfig.ini schön gemacht (mitten im Pfad "./" entfernt).
Version 1.19 - Bei manueller Einstellung des Ladestroms wird automatisch auf 3 Phasen umgeschaltet. Das bewirkt, dass der
                in der GUI eingestellte Ladestrom verdreifacht wird (jede Phase übernimmt den eingestellten Wert), allerdings nur,
                wenn ein dreiphasiges Ladekabel verwendet wird und das Fahrzeug dreiphasiges Laden unterstützt.
                Das wurde erst mit dem Capri zum Problem, der lädt mit 3 Phasen und hat ein 3-phasiges Ladekabel.
                -> Behoben mit Radiobuttons für 1- bzw. 3-phasiges Laden im Manual Dialog.
             - Gesamtertrag der Anlage aus der PV-Box entfernt, der Wert steht schon im Drawer.
             - Darstellung der WR-Werte geschönt (Column, offset, spacing, Item).
             - Extern (in main.cpp) deklarierte Variablen aus den Routinen an den jeweiligen Dateianfang verschoben.
             - Liste der möglichen Remote-Befehle in "Downloader.cpp" als Kommentar.
Version 1.20 - Überflüssige (bereits auskommentierte) Includes entfernt.
             - Überflüssige Kommentare entfernt (auskommentierte Routinen und Variablendefinitionen).
             - Gesetzte Anzahl Phasen für Radio-Buttons im Manual Drawer von Wallbox zurücklesen und im Drawer anzeigen.
             - TransferTimeout für alle Aufrufe an EDLD und MBMD gesetzt (4 Sekunden statt DefaultTransferTimeout -> 30s).
                Theoretisch werden damit in PV-Anzeige nicht beantwortete Anfragen beim SmartCharger abgebrochen und laufen nicht immer weiter.
Version 1.21 - Update des EDLD auf V2.9.114 -> Neue XML SettingsVersion 1.8 freigegeben (ist identisch zu 1.6).
             - In der Datei edld.ini (auf dem SmartCharger RPi) kann die Zeit nach einem Umschalten des Phasenrelais eingestellt werden.
                Gültige Werte sind 10..45 s. (Momentan auf 45 s gesetzt). Größere Werte verursachen ein Rücksetzen der Wallbox (60s Watchdog).
                Diese längere Pause ist notwendig, um einen MEB basierten EV Zeit zu geben, sich auf die geänderten Phasenanzahl
                einzustellen (bekanntes Problem aller EV die auf dem VW MEB basieren -> beispielsweise VW ID3..7, Skoda, Ford Capri, ...).
                EDLD macht aus den eingestellten 45 s deren 35 s -> ist OK, Ford Capri schlägt >30 s vor (Duck.Ai-Antwort ;)
             - Alle Werte in der Wallbox-Anzeige auf Null setzen, wenn EDLD die Wallbox nicht anspricht (Haken aus in SmartCharger-Settings).
Version 2.00 - Versionssprung auf 2.0 wegen Umstellung auf neuen DataProvider EVCC.
             - Alternativer Zugang zu WR-Daten, Energy-Meter-Daten und Wallbox-Daten über EVCC (anstelle EDLD und MBMD).
                Die Unterstützung der Keba Wallbox in EVCC kostet einmalig 100$ -> man erhält einen "Provider-Token".
                Der Token wird in die evcc.yaml eingetragen oder über die GUI in der evcc.db Datenbank (in der DB nur kompliziert zu ändern!).
             - Erstellung von EcvvJSON.h und .cpp - Die Werte von EVCC werden in einem JSON Array zur Verfügung gestellt.
             - -->> Übler Trick <<-- 2 #define um die Verdoppelung aller Werte-Zuweisungen zu vermeiden (PowerNodeModel.cpp - ca. Zeile 160
                #define smchaXML evccJSON           // Umleitung der smchaXML-Zugriffe auf evccJSON-Zugriffe
                #define wrJSON evccJSON             // Umleitung der wrJSON-Zugriffe auf evccJSON-Zugriffe
             - INI-Datei erweitert um Version der INI-Datei [INIVERSION]. Der Wert INIVERSION muss mindestens der Programmversion entsprechen,
                sonst wird eine Fehlermeldung auf std-err ausgegeben. Das Programm startet trotzdem, zeigt aber ggf. keine Daten.
             - Caption der App um " - EVCC" erweitert, siehe #define WINDOWTITLE in Z. 287 (suche #define WINDOWTITLE).
             - Schreibfehler LIGHTRED behoben.
             - GUI wo notwendig an EVCC Notation angepasst (abhängig von DataProvider auch für EDLD/MBMD passend).
             - Charge mode kann über die GUI geändert werden (Drawer2, rechts unten).
             - Farbe der Wallbox muss anders hergeleitet werden, EVCC liefert keinen Status im Sinne von EDLD.
             - Aktualisierungsrate in EVCC auf 20 s eingestellt (keine Programmänderung).
                10 s sind deutlich zu knapp- Regelung schwingt :( - EVCC Default ist 30 s (Minimum).
             - Umstellung Drawer-Background von Rectangle auf Button -> Drawer schließen per Click im Window und in der Drawerfläche.
             - setManualCurrent() auf Parameter umgestellt.
             - setChargerPhases() auf Parameter umgestellt.
             - Phasenanzahl kann über die GUI geändert werden (Drawer3, links unten).
             - showChargeMode() auf Parameter umgestellt.
             - Temperatur für Batterie bei EVCC ausgeblendet (in main.qml: Temp == 0 -> ausgeblendet).
             - Vorgabe für maximalen Ladestrom über die GUI implementiert (setManualCurrent())
             - Einstellungen in PV-Anzeige werden direkt in EVCC (Web-GUI) angezeigt, umgekehrt ebenfalls, aber um die Aktualisierungszeit
                von EVCC verzögert.
Version 2.01 - "Aktive Phasen" Anzeige im Wallbox-Kasten bei EVCC nach links gerückt und ausgeschrieben (anstelle der Eval.Points)
                Die Magie erfolgt in "main.qml". Wenn EVCC als Datenprovider in der "PVconfig.ini" eingestellt ist, werden Koordinaten
                und Texte angepasst: siehe PowerNodeModel.activeDataProvider == "EVCC" ?
Version 2.02 - "Aktive Phasen" Anzeige im Wallbox-Kasten korrigiert (falsche Variable verwendet)
Version 2.03 - Hintergrund hellrot einfärben, wenn evcc keine Daten mehr liefert oder der Server nicht läuft
Version 2.04 - evcc-Variablen im Zusammenhang mit Ladeverteilung zwischen Hausakku und EV im Code eingeführt (bufferSoc, prioritySoc, batteryDischargeControl).
                (Diese Vorgaben werden _in der evcc-GUI_ über Menü-Hausbatterie eingestellt).
             - INI-File Überprüfung auf MajorVersion reduziert.
Version 2.05 - Visualisierung der drei neuen Parameter bufferSoc, prioritySoc und batteryDischargeControl in Drawer4.
             - bufferSoc, prioritySoc und batteryDischargeControl können über die GUI im Drawer4 gesetzt werden.
                Werden die Parameter in Pv_Anzeige geändert, werden meist ncohmal kurz die vorherigen Werte angezeigt, bevor der neue Wert
                angezeigt wird. Evcc übernimmt die neuen Einstellungen sofort (Kontrolle über die evcc GUI).


  ---> Hinweis: Code läuft _nicht_ stabil mit Qt V6.x. Nach zufälligen Zeiten crasht die App auf dem Tablet ohne Meldung weg (ab V1.17 - 2025-06-21) <---
*/

// program version for window title
#define VERSIONMAJOR    "2"
#define VERSIONMINOR    "05"

//#define DEMOMODE              // generate random power values for checking coloring and arrows

class StringData;

// define colors according https://doc.qt.io/qt-5/qml-color.html
// use #RRGGBB notation for QML instead of 0xRRGGBB
#define VLIGHTGRAY      "#b3b3b3"       // "LIGHTGREY"      //  no defined color, hand tuned ;-)
#define LIMEGREEN       "#00ac00"       // "0x32cd32"       // -> "#32cd32" replace 0x by # for QColor in QML file
#define FORESTGREEN     "FORESTGREEN"   // "0x228b22"
#define FIREBRICK       "FIREBRICK"     // "0xb22222"
#define DODGERBLUE      "#02a4f5"       // "DODGERBLUE"     // EV charging // 0x0A7CEB = "um -20 dunkleres dogerblue" (Orig. 0x1e90ff)
#define DARKBLUE        "#2828B3"       // "#0371da" "0x00008b"     // EV attached to wallbox
#define LIGHTBLUE       "#72BBFF"       // sehr helles Blau
#define LIGHTRED        "#FFE5E5"       // sehr helles Rot
#define SUNWHITE        0               // "WHITE"          // weiß
#define SUNLTYELLOW     1               // "#FFFFA8"        // helles Gelb
#define SUNYELLOW       2               // "YELLOW"         // Gelb

class PowerNodeModel : public QObject {
    Q_OBJECT

    EvccJSON evcc;

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
    Q_PROPERTY(int prioritySOC              MEMBER m_evPrioritySOC         NOTIFY batteryDataChanged)
    Q_PROPERTY(int bufferSOC                MEMBER m_evBufferSOC           NOTIFY batteryDataChanged)
    Q_PROPERTY(bool evBattDcControl         MEMBER m_evBattDcControl       NOTIFY batteryDataChanged)

    // home consumption properties - all home consumption values are updated in one call to "consumptionDataChanged"
    Q_PROPERTY(QString consumptionPower     MEMBER m_totPowerConsumption   NOTIFY consumptionDataChanged)
    Q_PROPERTY(QString homeColor            MEMBER m_homeColor             NOTIFY consumptionDataChanged)

    // shade properties
    Q_PROPERTY(int homeTopGreenH            MEMBER m_homeTopGreenH         NOTIFY shadeDataChanged)
    Q_PROPERTY(int homeBotRedH              MEMBER m_homeBotRedH           NOTIFY shadeDataChanged)

    // grid properties - all grid values are updated in one call to "gridDataChanged"
    Q_PROPERTY(QString gridText             MEMBER m_gridText           NOTIFY gridDataChanged)
    Q_PROPERTY(QString gridColor            MEMBER m_gridColor          NOTIFY gridDataChanged)
    Q_PROPERTY(QString gridPower            MEMBER m_gridPowerAnzeige   NOTIFY gridDataChanged)           // current power from/to grid
    Q_PROPERTY(double gridEnergyImport      MEMBER m_gridEnergyImport   NOTIFY gridDataChanged)           // Verbrauchszähler [kWh]
    Q_PROPERTY(double gridEnergyExport      MEMBER m_gridEnergyExport   NOTIFY gridDataChanged)           // Einspeisezähler [kWh]

    // wallbox properties - all wallbox values are updated in one call to "chargingDataChanged"
    Q_PROPERTY(QString chargingPower        MEMBER m_charPower          NOTIFY chargingDataChanged)       // current power [kW]
    Q_PROPERTY(QString chargedEnergy        MEMBER m_charEnergy         NOTIFY chargingDataChanged)       // total energy [kWh]
    Q_PROPERTY(QString sessionEnergy        MEMBER m_sessEnergy         NOTIFY chargingDataChanged)       // last session energy [kWh]
    Q_PROPERTY(int evalPoints               MEMBER m_evalPoints         NOTIFY chargingDataChanged)       // ab 6 startet PV Ladevorgang (nur EDLD)
    Q_PROPERTY(QString wallboxColor         MEMBER m_wallboxColor       NOTIFY chargingDataChanged)
    Q_PROPERTY(QString wallboxCar           MEMBER m_wallboxCar         NOTIFY chargingDataChanged)
    Q_PROPERTY(QString wallboxScoot         MEMBER m_wallboxScoot       NOTIFY chargingDataChanged)
    Q_PROPERTY(QString chargeMode           MEMBER m_EVChargingMode     NOTIFY chargingDataChanged)       // current charge mode (String)
    Q_PROPERTY(double manualCurrent         MEMBER m_EVManualCurrent    NOTIFY chargingDataChanged)       // manually set charge current (Number)
    Q_PROPERTY(QString manualCurrentS       MEMBER m_EVManualCurrentS   NOTIFY chargingDataChanged)       // manually set charge current (String)
    Q_PROPERTY(bool visibleComm             MEMBER m_visibleComm        NOTIFY showComm)                  // WLAN Icon rechts oben in Wallbox
    Q_PROPERTY(char evalCountDown           MEMBER m_evalCountDown      NOTIFY showComm)                  // (nicht genutzt)
    Q_PROPERTY(QString usedPhases           MEMBER m_EVusedPhasesS      NOTIFY chargingDataChanged)       // currently used phases (String)
    Q_PROPERTY(QString chargeModeManual     MEMBER m_chargeModeManual   NOTIFY chargingDataChanged)       // currently used charge mode (String)
    Q_PROPERTY(int configuredPhases         MEMBER m_EVconfiguredPhases NOTIFY chargingDataChanged)       // ab 6 startet PV Ladevorgang (nur EDLD)
    Q_PROPERTY(QString activeDataProvider   MEMBER m_actDataProviderStr NOTIFY chargingDataChanged)       // bestimmte Anzeigen in der GUI ein/ausblenden/ändern

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
    void showEvalPtsTxt(void);

public slots:
    void switchEVIcons();                   // change visualisation of car/scooter (icon or real picture)
    void switchChargeMode(QString);         // send (new) chargeMode setting to SmartCharger
    void changeChargeMode(QString);           // display chargeMode in GUI
    void showChargeMode();                 // display currently selected ChargeMode
    void switchManualCurrent();             // send (new) manual current to SmartCharger

    void setManualCurrent(int);             // display ManualChargeCurrent 6, 12, 18, 32 A in GUI
    void showManualCurrent();               // display currently selected ManualChargeCurrent
    void showUsedPhases();                  // display currently selected used phases (relais on X2 switched ON/OFF - ON: 3 phases - OFF: 1 phase)
    void showManualPhases();                // display manually set used phases (relais on X2 switched ON/OFF - ON: 3 phases - OFF: 1 phase)
    QString chargeModeButtonTxt();          // send button text to "MANUAL" button in drawer2 (main.qml)
    void setBatDcControl(qreal);
    void setBufferSoc(int);
    void setPrioritySoc(int);

    void setChargerPhases(int);             // set Charger Phases to 0(automatic), 1, 3

    void openVersionInfoMsg();              // Anzeige der Compiler- und Runtimeversion (V1.17)
    void openPopUpMsg();                    // Anzeige der Erträge aller WR und Gesamt (V1.17)

private:
    void getXMLdata(void);
    void getJSONdata(void);
    void getEvccJSONdata(void);
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
    void setEvccText(void);
    void setBGColor(void);
    void setSunAngle(void);
    void setSunColor(int8_t);
    void setComm(void);
    void resetComm(void);
    void setWindowTitle(void);
    void countDown(void);
    void setEvPrioritySOC(void);
    void setEvBufferSOC(void);
    void setEvBattDischargeControl(void);

public:
    QString getChargeModeString(void);

// window title with version & build date
#define WINDOWTITLE "PV-Anzeige - V" VERSIONMAJOR "." VERSIONMINOR " - EVCC"
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
//    QColor m_SunBGColor = "#ffffff";      // wird angemeckert (Warnung)
    QColor m_SunBGColor = QColor(255, 255, 255, 255);   // weiß

// battery, Akku
    int m_batteryPower = 0;                 // Batterieladung/-Entladung [kW]
    QString m_battPowerAnzeige = 0;         // Batterieladung/-Entladung zur Anzeige [kW]
    int m_batteryPercentage = 0;            // Batterie Ladezustand [%]
    QString m_batteryColor = VLIGHTGRAY;    // Farbe der Akku Box
    QString m_batteryText = "";             // Text in der Batterie Box, wechselt Ladung/Entladung
    QString m_batteryImage = "/Icons/Akku_weiss_transparent00.png";
    int m_batteryFill = 0;                  // analoge Balkenanzeige des Betteriefüllstands
    double m_battTemp = 0.0;                // Temperatur des Akkus
    int m_evPrioritySOC = 0;                // bis zu welchem %-Satz muss der Hausakku geladen sein, bevor das EV anfängt zu laden (bei knapper Sonne)
    int m_evBufferSOC = 0;                  // der Hausakku wird nur für EV-Ladung genutzt, wenn er über diesem %-Satz geladen ist
    bool m_evBattDcControl = true;          // wenn true wird der Hausakku nicht entladen, wenn das EV mit QUICK geladen wird

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
    int m_Output = 0;                       // Phasen-Relais-Ausgang geschaltet (0: AUS - 1: EIN)
    QString m_EVusedPhasesS;                // String für Anzeige in der GUI
    QString m_chargeModeManual;             // MANUAL oder Min+PV für den Button im Drawer, je nach DataProvider
 //   int m_EVChargerPhases;                  // Rückmeldung von Wallbox, Anzahl Phasen beim Laden: 1 oder 3
    int m_EVconfiguredPhases;               // Rückmeldung von EVCC
    QString m_actDataProviderStr;           // Dataprovider (EVCC oder EDLD)

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

