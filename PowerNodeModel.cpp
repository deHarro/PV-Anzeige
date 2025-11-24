#include <iostream>

#include "PowerNodeModel.h"
#include "Downloader.h"
#include "SmartChargerXML.h"
#include "WechselrichterJSON.h"
#include "EvccJSON.h"

// BUILDDATE	wird automatisch erzeugt:
// Build Datum der Form "Mar 9 2021" aus __DATE__ ummodeln in ISO Darstellung "2021-03-09"
constexpr unsigned int compileYear = (__DATE__[7] - '0') * 1000 + (__DATE__[8] - '0') * 100 + (__DATE__[9] - '0') * 10 + (__DATE__[10] - '0');
constexpr unsigned int compileMonth = (__DATE__[0] == 'J') ? ((__DATE__[1] == 'a') ? 1 : ((__DATE__[2] == 'n') ? 6 : 7))    // Jan, Jun or Jul
    : (__DATE__[0] == 'F') ? 2                                                              // Feb
    : (__DATE__[0] == 'M') ? ((__DATE__[2] == 'r') ? 3 : 5)                                 // Mar or May
    : (__DATE__[0] == 'A') ? ((__DATE__[1] == 'p') ? 4 : 8)                                 // Apr or Aug
    : (__DATE__[0] == 'S') ? 9                                                              // Sep
    : (__DATE__[0] == 'O') ? 10                                                             // Oct
    : (__DATE__[0] == 'N') ? 11                                                             // Nov
    : (__DATE__[0] == 'D') ? 12                                                             // Dec
    : 0;
constexpr unsigned int compileDay = (__DATE__[4] == ' ') ? (__DATE__[5] - '0') : (__DATE__[4] - '0') * 10 + (__DATE__[5] - '0');

#if defined (MITSTRICH)					// ergibt ISO "2021-03-09"
constexpr char IsoDate[] =
{ compileYear / 1000 + '0', (compileYear % 1000) / 100 + '0', (compileYear % 100) / 10 + '0', compileYear % 10 + '0',
    '-',  compileMonth / 10 + '0', compileMonth % 10 + '0',
    '-',  compileDay / 10 + '0', compileDay % 10 + '0',
    0
};
#else									// ergibt deutsche Darstellung "09.03.2021"
constexpr char IsoDate[] =
{ compileDay / 10 + '0', compileDay % 10 + '0',
    '.',  compileMonth / 10 + '0', compileMonth % 10 + '0',
    '.',  compileYear / 1000 + '0', (compileYear % 1000) / 100 + '0', (compileYear % 100) / 10 + '0', compileYear % 10 + '0',
    0
};
#endif
// \BUILDDATE


Downloader downler;
SmartChargerXML smchaXML;
WechselrichterJSON wrJSON;
EvccJSON evccJSON;

extern SmartChargerXML smchaXML;
extern Downloader downler;
extern WechselrichterJSON wrJSON;
//extern EvccJSON evccJSON;

extern QString m_setChargeModeString;
extern int m_ManualSetCurrent;
extern QString m_EVChargingModeS;
extern int m_ChargerPhases;

QString activeDataProviderString = downler.getDataProvider();
QString iniVersion = downler.getiniVersion();

using namespace std::chrono_literals;


PowerNodeModel::PowerNodeModel() {

    connect(&m_dataTimer, &QTimer::timeout, this, &PowerNodeModel::onDataTimer);
    m_dataTimer.start(500);     // 1/2 s timer for smooth display of rotating sun

    getIconType();              // get type of EV icons

    setWindowTitle();           // set window title with name and build date
    emit displayWindowTitle();
}

PowerNodeModel::~PowerNodeModel() {
}

// cyclically retrieve data from converters and wallbox
void PowerNodeModel::onDataTimer() {

    static int timerCounter = 0;

    if ((timerCounter++ % 10) == 0)  // alle 5 Sekunden den RPi abfragen (10 * 500 ms)
    {
    // Update the different values in C++
        setComm();              // switch on communication visu
        setSunAngle();          // slowly rotate sun icon
        setSunColor(SUNWHITE);  // change sun icon color
        //getXMLdata();           // extract values from XML string, read from RPi EDL Daemon
        //getJSONdata();          // extract values from JSON string, read from RPi MBMD Daemon
        getEvccJSONdata();      // extract values from JSON string, read from RPi evcc
        generatorHandling();    // PV generator handling
        batteryHandling();      // battery handling
        gridHandling();         // grid handling
        wallboxHandling();      // wallbox handling
        arrowsHandling();       // arrows handling
        consumptionHandling();  // consumption handling
        shadeHandling();        // handle shades for home with fractional grid power and fractional battery power
        setEDLDText();          // emit warning message if connection to EDL Daemon on RPi ceases
        setWRText();            // emit warning message if one or more of the WR fail to send data (modbus error)
        setMBMDText();          // emit warning message if connection to MBMD Daemon on RPi ceases
        setBGColor();           // on comm error make background light red
        showManualCurrent();    // make Manual ChargeCurrent visible in Drawer
        showManualPhases();     // make Manual Phases visible in Drawer
        showUsedPhases();       // show used phases in GUI

    // Update the different values in QML -> show on GUI
        emit showComm();
        emit rotateSun();
        emit sunColor();
        emit arrowsChanged();
        emit generatorDataChanged();
        emit gridDataChanged();
        emit batteryDataChanged();
        emit chargingDataChanged();
        emit consumptionDataChanged();
        emit shadeDataChanged();
        emit gridDataChanged();
        emit arrowsChanged();

        emit setEDLDWarning();
        emit setMBMDWarning();
        emit setWRWarning();
        emit setBackgroundColor();
    }
    else    // zyklisch die Sonne etwas drehen (wird jede 500 ms aufgerufen)
    {
        resetComm();             // switch off communication visu
        setSunAngle();
        emit rotateSun();
        emit showComm();
    }

    if ((timerCounter % 2) == 0)     // jede Sekunde aufrufen (2 * 500 ms)
    {
        countDown();
        emit showComm();
    }

}


// handling routines
void PowerNodeModel::getXMLdata(void)
{
    // download XML data from SmartCharger XML page into **global**  m_XMLfiledata
    downler.doDownloadXML();

    // decode XML data from m_XMLfiledata into member variables of SmartChargerXML
    smchaXML.ReadSmartChargerXML();
}

void PowerNodeModel::getJSONdata(void)
{
    // download JSON data from mbmd JSON page into **global**  m_JSONfiledata
    downler.doDownloadJSON();

    // decode JSON data from m_JSONfiledata into member variables of SmartChargerJSON
    wrJSON.ReadWechselrichterJSON();
}

// übler Trick um die Verdoppelung aller Zuweisungsroutinen zu vermeiden ---------------------------------
#define smchaXML evccJSON           // Umleitung der smchaXML-Zugriffe auf evccJSON-Zugriffe
#define wrJSON evccJSON             // Umleitung der wrJSON-Zugriffe auf evccJSON-Zugriffe
// \übler Trick um die Verdoppelung aller Zuweisungsroutinen zu vermeiden --------------------------------

void PowerNodeModel::getEvccJSONdata(void)
{
    // download JSON data from mbmd JSON page into **global**  m_JSONfiledata
    downler.doDownloadEVCCJSON();

    // decode JSON data from m_JSONfiledata into member variables of SmartChargerJSON
    evccJSON.ReadEvccJSON();
}

void PowerNodeModel::setMBMDText(void)              // Fehlermeldung wenn MBMD Daemon Probleme hat
{
    // check downloaded XML data for correct version
    // Ich verwende den MBMD-Text mit, damit die Meldung auf der linken Seite erscheint (wird nur gesetzt, nie gelöscht)
    if (m_messageFlag & VERSIONFlag)
    {
        m_MBMDProblemText = "EDLD - XML Version überprüfen!";
    }
    else if (m_messageFlag & MBMDFlag)
    {
        m_MBMDProblemText = "MBMD hat Probleme!";
    }
    else
    {
        m_MBMDProblemText = "";
    }
}

void PowerNodeModel::setEDLDText(void)              // Fehlermeldung wenn EDLD Daemon Probleme hat
{
    if (m_messageFlag & EDLDFlag)
    {
        m_EDLDProblemText = "EDLD hat Probleme!";
    }
    else
    {
        m_EDLDProblemText = "";
    }
}

void PowerNodeModel::setWRText(void)                // Fehlermeldung wenn einer der Wechslrichter Probleme hat
{
    if (m_messageFlag & WRFlag)
    {
        m_WRProblemText = "Mindestens einer der Wechselrichter liefert keine Daten!";
    }
    else
    {
        m_WRProblemText = "";
    }
}

void PowerNodeModel::setBGColor(void)               // Hintergrundfarbe ändern wenn auf dem RPi Probleme auftreten
{
    if (m_messageFlag & (EDLDFlag | MBMDFlag | VERSIONFlag | WRFlag))      // EDLD oder MBMD oder falsche Version oder WR Fehler
    {
        m_backgroundColor = LIGHTRED;              // sehr helles Rot
    }
    else
    {
        m_backgroundColor = "whitesmoke";           // sehr helles Grau
    }

    // zusätzlich Zahlen in Rot darstellen, wenn sie als ungültig zu betrachten sind
    if (m_messageFlag & EDLDFlag)                   // EDLD gesteuerte Werte
    {
          m_EDLDfigures = "red";
    }
    else
    {
          m_EDLDfigures = "white";
    }

    if (m_messageFlag & MBMDFlag)                   // MBMD gesteuerte Werte
    {
        m_MBMDfigures  = "red";
    }
    else
    {
        m_MBMDfigures  = "white";
    }

    // consumptionPower hängt an EDLD _und_ MBMD, 2022-05-26
    if (m_messageFlag & (EDLDFlag | MBMDFlag))      // EDLD oder MBMD gesteuerte Werte
    {
        m_consumptionPowerfigures = "red";
    }
    else
    {
        m_consumptionPowerfigures = "white";
    }
}

// rotate sun icon
void PowerNodeModel::setSunAngle(void)
{
    m_sunAngle += .5;
    if (m_sunAngle > 360.0) m_sunAngle = 0.0;
}

// change sun icon
void PowerNodeModel::setSunColor(int8_t newColor)
{
    // Setzt den Hintergrund der Sonne abhängig von der Einstrahlung. Wird zyklisch aufgerufen
    switch( newColor)
    {
        case 0: m_sunColor =  "/Icons/Sonne_invers_hellgrau.png"; break;    // ohne Sonne -> hellgrauer Hintergrund
        case 1: m_sunColor =  "/Icons/Sonne_invers_gruen.png"; break;       // mit Sonne  -> grüner Hintergrund
        default: m_sunColor = "/Icons/Sonne_invers_hellgrau.png";
    }
}

void PowerNodeModel::openPopUpMsg() {
    // Messagebox mit Ertragswerten der WR aufpoppen.
    // Schließen mit OK

    QMessageBox msgBox;
    msgBox.setTextInteractionFlags(Qt::TextInteractionFlags (true));

    msgBox.setTextFormat(Qt::RichText);
    // <pre> bewirkt die Umstellung auf Courier New (mono spaced Font), kann nicht unkompliziert geändert werden
    msgBox.setText("<pre>"
        "<b>Ertrag der Wechselrichter</b>"
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
        "<br><br>"
        "Dach Nord:&#9;" + m_genEnergyDachN + " kWh<br>" +
        "Dach Süd:&#9;" + m_genEnergyDachS + " kWh<br>" +
        "Gaube:&#9;" + m_genEnergyGaube + " kWh<br>" +
        "Garage:&#9;" + m_genEnergyGarage + " kWh<br><br>" +
        "Gesamt:&#9;" + m_genEnergyTotal + " MWh"
        "</pre>");

    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Information);

    msgBox.exec();
}

void PowerNodeModel::openVersionInfoMsg() {
    // Messagebox mit VersionInfo aufpoppen.
    // Schließen mit OK

    QString qtCompilerversionString = QString::number(QT_VERSION, 16);      // get compiler version
    QString qtRuntimeversionString = qVersion();                            // get runtime version
    activeDataProviderString = downler.getDataProvider();                   // get Dataprovider (EVCC oder EDLD/MBMD)

    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText("<b>Version Info</b><br><br>"
        "Version : V" VERSIONMAJOR "." VERSIONMINOR
        // die nachfolgenden zwei Zeilen legen die Breite der MSG-Box fest. Versuche, das alternativ zu erreichen, gingen schief :(
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
        "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"
        "<br>Builddate: " + QString(IsoDate) +
        "<br>Compiletime: " + QString(__TIME__) +
        "<br>Compilerversion: " + qtCompilerversionString +
        "<br>Runtimeversion: " + qtRuntimeversionString +
        "<br>Daten von: " + activeDataProviderString);

    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Information);

    msgBox.exec();
}

// visualize interrogation of RPi for new values
void PowerNodeModel::setComm(void)
{
        m_visibleComm = true;
}
void PowerNodeModel::resetComm(void)
{
        m_visibleComm = false;
}

// set window title
void PowerNodeModel::setWindowTitle(void)
{
    //m_windowTitle =  m_windowTitle;
}

// countDown handling
void PowerNodeModel::countDown(void)
{
    static uint8_t cdState = 0;
    switch (cdState)
    {
        case 0:                     // default state, waiting for change from evalstate 0 to 1
            if (m_evalCountDown > 0) {
                m_evalCountDown--;
            }
            cdState = 1;
        break;

        case 1:
//            if ((m_evalPoints > 0) && (m_evalPoints < 10))  // only do something from 1..9
            {
                if (m_evalCountDown > 0) {
                    m_evalCountDown--;
                }
                else {
                    m_evalCountDown = 59;   // start over from 59, counting down to 0,
                                            // 59 because of 1 s loss from statemachine handling
                    cdState = 0;
                }
            }
            break;

        default:
            cdState = 0;
    }
}

// PV generator handling -----------------------------------------------------
void PowerNodeModel::generatorHandling(void)
{
#if defined DEMOMODE
    m_generatorPowerDachS = rand() % 9000;
    m_generatorPowerDachN = rand() % 9000;
    m_generatorPowerGaube = rand() % 3600;
    m_generatorPowerGarage = rand() % 3000;
    m_generatorTotalEnergy = (rand() % 66000) + 30000;  // im Bereich ab 66 MWh
#endif

    m_generatorPowerDachS = (wrJSON.getPVDachSActualPower());        // [W] integer, no fraction
    m_generatorPowerDachN = (wrJSON.getPVDachNActualPower());        // [W] integer, no fraction
    m_generatorPowerGarage = (wrJSON.getPVGarageActualPower());      // [W] integer, no fraction
    m_generatorPowerGaube = (wrJSON.getPVGaubeActualPower());        // [W] integer, no fraction

    m_generatorPowerTotal   =   m_generatorPowerDachS                // [W] integer, no fraction
                            +   m_generatorPowerDachN
                            +   m_generatorPowerGaube
                            +   m_generatorPowerGarage;

    // Werte für Anzeige berechnen und als QString ausgeben
    m_genPowerTotal = QString().asprintf("%0.2f", (double)((double)(abs(m_generatorPowerTotal)/(double)1000))); // get rid of math in QML
    m_genPowerDachS = QString().asprintf("%0.2f", (double)((double)(abs(m_generatorPowerDachS)/(double)1000))); // get rid of math in QML
    m_genPowerDachN = QString().asprintf("%0.2f", (double)((double)(abs(m_generatorPowerDachN)/(double)1000))); // get rid of math in QML
    m_genPowerGaube = QString().asprintf("%0.2f", (double)((double)(abs(m_generatorPowerGaube)/(double)1000))); // get rid of math in QML
    m_genPowerGarage = QString().asprintf("%0.2f", (double)((double)(abs(m_generatorPowerGarage)/(double)1000))); // get rid of math in QML

    if(m_generatorPowerTotal <= 5) setSunColor(0);          // Grauer SonnenHintergrund
    else setSunColor(1);                                    // Grüner SonnenHintergrund

    // Gelbsättigung der Sonne berechnen
    m_SunBGColor = QRgb(0xFFFF00);                          // vorbelegen mit GELB

    unsigned int blueVal = m_generatorPowerTotal/40;
    blueVal = std::min((unsigned int)254, blueVal);
//    std::cout << "blueVal  = " << blueVal << std::endl;    // Display blueVal

    m_SunBGColor.setBlue(255 - (blueVal));                  // Weißanteil verringern -> Sättigung erhöhen
//    std::cout << "m_generatorPowerTotal  = " << m_generatorPowerTotal << std::endl;    // Display total Power

    m_generatorTotalEnergy = (wrJSON.getPVGesamtErtrag());       // [W] integer, no fraction

    // Werte für Anzeige berechnen und als QString ausgeben
    m_generatorTotalEnergy = m_generatorTotalEnergy / 1000;

    if(m_generatorPowerTotal == 0) {
        m_generatorColor = VLIGHTGRAY;                  // helles Hellgrau, keine QML Basic/SVG color
    }
    else {
        m_generatorColor = LIMEGREEN;                   // Hellgrün
    }

    // Ertragswerte der einzelnen Wechselrichter in Membervariablen speichern
    m_generatorDachSEnergy  = (wrJSON.getPVDachSErtrag());                          // [W] integer, no fraction
    m_genEnergyDachS = QString().asprintf("%6.0f", abs(m_generatorDachSEnergy));    // get rid of math in QML
    m_generatorDachNEnergy  = (wrJSON.getPVDachNErtrag());                          // [W] integer, no fraction
    m_genEnergyDachN = QString().asprintf("%6.0f", abs(m_generatorDachNEnergy));    // get rid of math in QML
    m_generatorGaubeEnergy  = (wrJSON.getPVGaubeErtrag());                          // [W] integer, no fraction
    m_genEnergyGaube = QString().asprintf("%6.0f", abs(m_generatorGaubeEnergy));    // get rid of math in QML
    m_generatorGarageEnergy = (wrJSON.getPVGarageErtrag());                         // [W] integer, no fraction
    m_genEnergyGarage = QString().asprintf("%6.0f", abs(m_generatorGarageEnergy));  // get rid of math in QML
    m_genEnergyTotal = QString().asprintf("%3.2f", abs(m_generatorTotalEnergy));    // get rid of math in QML
}

// battery handling ----------------------------------------------------------
void PowerNodeModel::batteryHandling(void)
{
#if defined DEMOMODE
    m_batteryPower = (rand() % 10000) - 5000;
    //    m_batteryPower = 0;               // test
    m_batteryPercentage = rand() % 100;
    m_battTemp = 30.0 + (rand() % 30) - 15;
#else
    m_batteryPower = -(smchaXML.getStorageSystemActualPower());  // [W] integer, no fraction
    m_batteryPercentage = smchaXML.getStorageSystemSOC();        // [%] integer, no fraction
    m_battTemp = smchaXML.getStorageSystemTemperature();         // [°] double, fraction (27.1)
#endif

    m_battPowerAnzeige = QString().asprintf("%0d", m_batteryPower);

    // Werte für Anzeige berechnen und als QString ausgeben
    m_battPowerAnzeige = QString().asprintf("%0.2f", (double)((double)(abs(m_batteryPower)/(double)1000))); // get rid of math in QML

    // change text and color depending on power value
    if(m_batteryPower == 0) {
        m_batteryText = "";                             // kein Strom  -> kein Text
        m_batteryColor = VLIGHTGRAY;                    // helles Hellgrau, keine QML Basic/SVG color
    }
    else if(m_batteryPower > 0) {
        m_batteryText = "Batterie-ladung";              // Batterie wird geladen
        m_batteryColor = LIMEGREEN;                     // Hellgrün
    }
    else {                                              // Batterie wird entladen
//        m_batteryPower = abs(m_batteryPower);           // auch negative Werte (bei Entladung) werden positiv dargestellt...
        m_batteryText = "Batterie-entladung";           // ... nur der Text ändert sich
        m_batteryColor = FORESTGREEN;                   // Dunkelgrün
    }

    // change battery icon depending on battery percentage (quasi Analog)
    m_batteryFill = m_batteryPercentage;
}

// grid handling -------------------------------------------------------------
void PowerNodeModel::gridHandling(void)
{
#if defined DEMOMODE
    m_gridEnergyImport += (10 + (rand() % 100));    // Verbrauchszähler Richtung Netz
    m_gridEnergyExport += (100 + (rand() % 100));   // Einspeisezähler Richtung Netz

    m_gridPower = (rand() % 10000) - 5000;
//    m_gridPower = 0;
#else
    m_gridEnergyImport = smchaXML.getSmartMeterConsumption();   // [kW] double, fraction (7322.8)
    m_gridEnergyExport = smchaXML.getSmartMeterSurplus();       // [kW] double, fraction (43852.6)
    m_gridPower = -(smchaXML.getSmartMeterActualPower());       // [W] integer, no fraction
#endif

    // Werte nur positiv anzeigen, Richtung kommt über die Farbe und die Pfeile
    m_gridPowerAnzeige = QString().asprintf("%0.2f", (double)((double)(abs(m_gridPower)/(double)1000))); // get rid of math in QML

//    qDebug() << "m_gridPower: " << m_gridPower;

    if (m_gridPower <= -5)                              // Netzbezug    (-5 weil gerundet wird)
    {
        m_gridColor = FIREBRICK;                        // Dunkelrot
        m_gridText = "Netzbezug";
    }
    else if (m_gridPower >= 5)                           // Einspeisung  (5 weil gerundet wird)
    {
        m_gridColor = LIMEGREEN;                        // Hellgrün
        m_gridText = "Netz-einspeisung";
    }
    else
    {
        m_gridColor = VLIGHTGRAY;                       // helles Hellgrau, keine QML Basic/SVG color
        m_gridText = "";                                // kein Strom  -> kein Text
    }
}

// EV icon handling
void PowerNodeModel::switchEVIcons()
{
    m_realPics = !m_realPics;                           // toggle icon versions
    wallboxHandling();                                  // update EV icons
    emit chargingDataChanged();                         // refresh GUI
}


QString PowerNodeModel::chargeModeButtonTxt()
{
    return (m_chargeModeManual);
}

// setting ChargeMode handling
void PowerNodeModel::switchChargeMode(QString value)
{
    if      (value == "OFF")                                                //  EVCC      EDLD
        m_setChargeModeString = (activeDataProviderString.toUpper() == "EVCC" ? "off"   : "off"     );
    else if (value == "QUICK")
        m_setChargeModeString = (activeDataProviderString.toUpper() == "EVCC" ? "now"   : "quick"   );
    else if (value == "SURPLUS")
        m_setChargeModeString = (activeDataProviderString.toUpper() == "EVCC" ? "pv"    : "surplus" );
    else if (value == "MANUAL")
        m_setChargeModeString = (activeDataProviderString.toUpper() == "EVCC" ? "minpv" : "manual"  );
    else
        m_setChargeModeString = (activeDataProviderString.toUpper() == "EVCC" ? "pv"    : "surplus" );  // default mode

    downler.doSetChargeMode();
    emit chargingDataChanged();                         // refresh GUI
}

void PowerNodeModel::changeChargeMode(QString value)
{
    m_EVChargingMode = value;
    switchChargeMode(value);
    showChargeMode();
    emit chargingDataChanged();                         // refresh GUI
}
void PowerNodeModel::showChargeMode()
{
    m_chargeModeManual = (activeDataProviderString.toUpper() == "EVCC" ? "Min+PV" : "MANUAL");
    emit chargingDataChanged();                         // refresh GUI
}
// \setting ChargeMode handling

// setting Manual Current handling
void PowerNodeModel::switchManualCurrent()
{
    m_EVManualCurrentS = QString::number(m_EVManualCurrent / 1000 * 230 / 1000 * (m_Output == 0 ? 1 : 3)) + " kW max.";   // Leistung berechnen (Strom[mA] * Spannung[V] / 1000) ergibt [kW])
    emit chargingDataChanged();                         // refresh GUI

    m_ManualSetCurrent = m_EVManualCurrent;
    downler.doSetManualCurrent();
}
void PowerNodeModel::setManualCurrent(int number)
{
    m_EVManualCurrent = number;
    switchManualCurrent();
}
// \setting Manual Current handling

// show manual current handling
void PowerNodeModel::showManualCurrent()
{
    m_EVManualCurrentS = QString::number(m_EVManualCurrent / 1000 * 230 / 1000 * (m_Output == 0 ? 1 : 3)) + " kW max.";   // Leistung berechnen (Strom[mA] * Spannung[V] / 1000) ergibt [kW])
    emit chargingDataChanged();                         // refresh GUI
}
// \show manual current handling

// show used phases handling
void PowerNodeModel::showUsedPhases()                   // zeigt die Rückmeldung von der Wallbox (Ausgang X1 aktiv/nicht aktiv)
{
    //m_EVusedPhasesS =  "P" + QString::number(m_Output == 0 ? 1 : 3);   // (m_Output = 0..1 -> 0: 1 Phase, 1: 3 Phasen)
    m_EVusedPhasesS =  QString::number(m_Output == 0 ? 1 : 3) + "P";   // (m_Output = 0..1 -> 0: 1 Phase, 1: 3 Phasen)
    emit chargingDataChanged();                         // refresh GUI
}
// \show used phases handling

// setting Charging Phases handling
void PowerNodeModel::setChargerPhases(int number)
{
    m_ChargerPhases = number;
//    setChargerPhases(m_ChargerPhases);
    downler.doSetChargerPhases();
    m_EVconfiguredPhases = number;                      // damit die Checkmark direkt beim Click wechselt (kann temporär wieder zurück springen)
    showManualPhases();
    emit chargingDataChanged();                         // refresh GUI
}
// \setting Charging Phases handling

// show Charging Phases handling
void PowerNodeModel::showManualPhases()
{
    emit chargingDataChanged();                         // refresh GUI
    //downler.doSetChargerPhases();
}
// \show Charging Phases handling

// wallbox handling ----------------------------------------------------------
void PowerNodeModel::wallboxHandling()
{
#if defined DEMOMODE
    m_chargingPower = rand() % 4000;
    m_chargedEnergy += rand() % 100000;
    m_sessionEnergy += 10;
    m_evalPoints += 1;
    m_evalPoints = m_evalPoints % 11;

    bool test = rand() % 2;                      // test
    test == 1 ? m_chargingPower = rand() % 4000 : m_chargingPower = 0;    // test
    m_evAttached = rand() % 2;                  // test
    m_evAttached == 1 ? m_evAttached = true : m_evAttached = false;    // test

    if(m_evAttached == true)                        // cable attached to ev (car/bike)
    {
        m_wallboxCar = "Icons/electric-car-icon_steckt_weiss_transparent.png";
//        m_wallboxScoot  = "Icons/electric-scooter_icon_steckt_weiss_transparent_rad.png";
        m_wallboxScoot  = "Icons/electric-scooter_real_steckt_transparent.png";

        if(m_chargingPower > 1){                    // keine Ladung aktiv ->
            m_wallboxColor = DODGERBLUE;            // schickes Blau
        }
        else
        {                                           // Ladung aktiv ->
            m_wallboxColor = DARKBLUE;              // dunkles Blau
        }
    }
    else
    {
        m_wallboxCar = "Icons/electric-car-icon_weiss_transparent.png";
        m_wallboxScoot  = "Icons/electric-scooter_icon_weiss_transparent_rad.png";

        m_wallboxColor =VLIGHTGRAY ;                // helles Hellgrau, keine QML Basic/SVG color
    }
#else
    m_evalPoints = smchaXML.getEVEvaluationPoints();            // [] integer, no fraction
    m_chargingPower = smchaXML.getEVActualPower();              // [W] integer, no fraction
    m_chargedEnergy = smchaXML.getEVTotalEnergy();              // [W] integer, no fraction
    m_sessionEnergy = smchaXML.getEVSessionEnergy();            // [W] integer, no fraction
    m_EVChargingMode = smchaXML.getEVChargeMode();              // QString
    m_Output = smchaXML.getEVOutput();                          // [] integer, no fraction
    m_EVconfiguredPhases = smchaXML.getEVconfiguredPhases();    // [] integer, no fraction

    // Werte für Anzeige berechnen und als QString ausgeben
    m_charPower = QString().asprintf("%0.3f", (double)((double)(abs(m_chargingPower)/(double)1000))); // get rid of math in QML
    m_charEnergy = QString().asprintf("%0.2f", (double)((double)(abs(m_chargedEnergy)/(double)1000))); // get rid of math in QML
    m_sessEnergy = QString().asprintf("%0.3f", (double)((double)(abs(m_sessionEnergy)/(double)1000))); // get rid of math in QML

    // derive attach state and charging state from wallbox states
    /*    // color selection
       "State" = Current state of the charging station
        0 : starting
        1 : not ready for charging; e.g. unplugged, X1 or "ena" not enabled, RFID not enabled, ...
        2 : ready for charging; waiting for EV charging request (S2)
        3 : charging
        4 : error
        5 : authorization rejected
    */

    if (((smchaXML.getEVState() == 2) || (smchaXML.getEVState() == 3))      // ready for or actually charging
      && (m_chargingPower == 0))                                            // state ready for charging (alles vorbereitet)
    {
        m_wallboxColor = DODGERBLUE;                                        // Ladung vorbereitet -> schickes mittleres Blau
    }
    else if ((smchaXML.getEVState() == 3) && (m_chargingPower > 0))         // state=charging && power>0
    {
        m_wallboxColor = DARKBLUE;                                          // Ladung startet oder läuft -> dunkles Blau
    }
    else if (smchaXML.getEVState() == 4)                                    // Error oder rejected
    {
        m_wallboxColor = FIREBRICK ;                                        // -> dunkles Rot
    }
    else if (((smchaXML.getEVState() <= 2) || (smchaXML.getEVState() == 5))
         && ((smchaXML.getEVPlug() == 5) || (smchaXML.getEVPlug() == 7)))   // Stecker steckt aber Wallbox not ready
    {
        m_wallboxColor = LIGHTBLUE ;                                        // helles Blau
    }
    else
        m_wallboxColor = VLIGHTGRAY ;                                       // helles Hellgrau, keine QML Basic/SVG color

    /*  // picture selection
       "Plug" = Current condition of the loading connection
        0 unplugged
        1 plugged on charging station
        3 plugged on charging station plug locked
        5 plugged on charging station plugged on EV
        7 plugged on charging station plug locked plugged on EV

        "Enable sys" = Enable state for charging (contains Enable input, RFID, UDP,..)
    */

    if (smchaXML.getEVPlug() >= 5)              // Stecker an EV und Wallbox sind eingesteckt
    {
        if(m_realPics == false)
        {
            m_wallboxCar = "Icons/electric-car-icon_steckt_weiss_transparent.png";
            m_wallboxScoot = "Icons/electric-scooter_icon_steckt_weiss_transparent_rad.png";
        }
        else
        {
            m_wallboxCar = "Icons/electric-car_real_steckt_transparent.png";
            m_wallboxScoot = "Icons/electric-scooter_real_steckt_transparent.png";
        }
    }
    else
    {
        if(m_realPics == false)
        {
            m_wallboxCar = "Icons/electric-car-icon_weiss_transparent.png";
            m_wallboxScoot = "Icons/electric-scooter_icon_weiss_transparent_rad.png";
        }
        else
        {
            m_wallboxCar = "Icons/electric-car_real_transparent.png";
            m_wallboxScoot = "Icons/electric-scooter_real_transparent.png";
        }
    }

    // all conditions true for charging but no current flowing -> EV state is "fully charged"
    if ((smchaXML.getEVPlug() >= 5) && (smchaXML.getEVState() == 2) && (m_chargingPower == 0))
    {
        if(m_realPics == false)
        {
            m_wallboxCar = "Icons/electric-car-icon_steckt_gruen_transparent.png";
            m_wallboxScoot  = "Icons/electric-scooter_icon_steckt_gruen_transparent_rad.png";
        }
        else
        {
            m_wallboxCar = "Icons/electric-car_real_steckt_gruen_transparent.png";
            m_wallboxScoot  = "Icons/electric-scooter_real_steckt_gruen_transparent.png";
        }
    }

#endif
}

// consumption handling ----------------------------------------------------------
void PowerNodeModel::consumptionHandling(void)
{
#if defined DEMOMODE
    m_totalPowerConsumption = rand() % 10000;
    //m_totalPowerConsumption = 0;
#else
    m_totalPowerConsumption = m_generatorPowerTotal     - (  (m_gridPower)
                                                        + (m_chargingPower)
                                                        + (m_batteryPower) );

    m_totalPowerConsumption = abs(m_totalPowerConsumption);  // Werte nur positiv anzeigen, Richtung kommt über die Farbe und die Pfeile

    // Werte für Anzeige berechnen und als QString ausgeben
    m_totPowerConsumption = QString().asprintf("%0.2f", (double)((double)(abs(m_totalPowerConsumption)/(double)1000))); // get rid of math in QML

#endif

    if(m_totalPowerConsumption == 0) {
        m_homeColor = VLIGHTGRAY;                   // helles Hellgrau, keine QML Basic/SVG color
    }
    else {
        m_homeColor = LIMEGREEN;                    // Hellgrün
    }
}

// arrows handling ----------------------------------------------------------
void PowerNodeModel::arrowsHandling(void)
{
    // PV to battery
    if((m_batteryPower > 9) && (m_generatorPowerTotal > 0))
    {
        m_pv2batt = true;
    }
    else
    {
        m_pv2batt = false;
    }

    // PV to house
    if((m_totalPowerConsumption > 9) && (m_generatorPowerTotal > 0))
    {
        m_pv2house = true;
    }
    else
    {
        m_pv2house = false;
    }

    // PV to grid
    if((m_gridPower >= 5) && (m_generatorPowerTotal > 9))    // (5 weil gerundet wird)
    {
        m_pv2grid = true;
    }
    else
    {
        m_pv2grid = false;
    }

    // battery to house
    if(((m_batteryPower < -9) && (m_generatorPowerTotal == 0)) ||     // PV keine Leistung
            ((m_batteryPower < -9) && (m_totalPowerConsumption > 0))) // Haus braucht Energie -> Batterieentladung
    {
        m_batt2house = true;
    }
    else
    {
        m_batt2house = false;
    }

    // house to battery (battery conditioning when PV is off for long time)
//    if((m_batteryPower > 9) && (m_generatorPowerTotal == 0) &&
    if((m_batteryPower > 1) && (m_generatorPowerTotal < m_batteryPower) &&  // Batterieladung > PV-Leistung -> kommt aus Netz
            (m_totalPowerConsumption > 0) && (m_gridPower < 0))             //  und Hausverbrauch u. Netzbezug
    {
        m_house2batt = true;
    }
    else
    {
        m_house2batt = false;
    }

    // grid to house
    if((m_gridPower <= -5) /*&& (m_totalPowerConsumption > 9)*/)    // (-5 weil gerundet wird)
    {
        m_grid2house = true;
    }
    else
    {
        m_grid2house = false;
    }

    // house to grid        // generator is off (night) and battery feeds house and temporarily the grid
    if((m_gridPower >= 5) && (m_generatorPowerTotal == 0))    // m_gridPowerAnzeige >= 10    // (5 weil gerundet wird)
    {
        m_house2grid = true;
    }
    else
    {
        m_house2grid = false;
    }

    // house to wallbox (charging of electric vehicle)
    if(m_chargingPower > 0)
    {
        m_house2charger = true;
    }
    else
    {
        m_house2charger = false;
    }

}

// shade handling  ----------------------------------------------------------
void PowerNodeModel::shadeHandling(void)
{
    // Anteil Netzbezug in ROT von unten kommend einblenden
    if(m_gridPower <= -5){                                // Netzbezug   (-5 weil gerundet wird)
        m_homeBotRedH = std::min((double)(abs(m_gridPower) / (double)m_totalPowerConsumption), (double)1) * 270;    // Höhe Home rectangle = 270 (war (double).5)
    }
    else
    {
        m_homeBotRedH = 0;
    }

    // Anteil Akkubezug in GRÜN von unten kommend einblenden
    if(m_batteryPower < 0){                            // Batteriebezug
        m_homeTopGreenH = std::min((double)(abs(m_batteryPower) / (double)m_totalPowerConsumption), (double)1) * 270;    // Höhe Home rectangle = 270 (war (double).5)
    }
    else
    {
        m_homeTopGreenH = 0;
    }
}

// get type of icon for car/scooter
void PowerNodeModel::getIconType()
{
    // open PVconfig.ini
    QDir dir("./");
    // to use/start from the current directory, we have to apply "./" as absoluteFilePath...
    QString filepath = dir.absoluteFilePath("./");              // path of PV-Anzeige.exe _at runtime_ (_not_ in Qt Creator!!)
    QFile file;

    // ... but to remove the dot in the middle of the resulting path, this "./" has to be removed again, before adding "debug/release" below
    // this is not absolutely neccessary but it's cleaner ;)
    filepath.chop(2);                                           // get rid of "./" in the middle of the path

    // catch case "running in QT Creator"
    if (filepath.contains("-Debug"))
        file.setFileName(filepath + "PVconfig.ini");      // add filename to path
    else if (filepath.contains("-Release"))
        file.setFileName(filepath + "PVconfig.ini");    // add filename to path
    else
        file.setFileName(filepath + "PVconfig.ini");            // add filename to path

    if(file.exists())                                           // check file access
    {
        if (!file.open(QIODevice::ReadOnly))
        {
            // Error while loading file
            std::cerr << "Error while loading PVconfig.ini" << std::endl;
        }
        else
        {
            while (!file.atEnd())
            {
                QString line = file.readLine();
                if (line.contains("[INIVERSION]"))
                {
                    QString iniVersion = (QString(file.readLine()).remove(QChar('\r'))).remove(QChar('\n'));     // Ini-Version 1.xx

                    if((iniVersion.left(1) >= VERSIONMAJOR) && (iniVersion.right(2) >= VERSIONMINOR))
                    {
                        QString line = file.readLine();
                        if (line.contains("[REALPICS]"))
                        {
                            m_realPics = (QString(QString(file.readLine()).remove(QChar('\r'))).remove(QChar('\n')).toInt() == 0) ? false : true;     // Icon Flag
                        }
                    }
                    else
                    {
                        // Error wrong INI-File version
                        std::cerr << "Error wrong INI-File version" << std::endl;
                    }
                }
            }
        }
        file.close();
    }
    else
    {
        // Error while loading file
        std::cerr << "Error: PVconfig.ini doesn't exist (at the given path)" << std::endl;
    }
}
