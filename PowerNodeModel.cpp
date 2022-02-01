#include "PowerNodeModel.h"

#include <algorithm>
#include <chrono>
#include <iostream>

#include "Downloader.h"
#include "SmartChargerXML.h"
#include "WechselrichterJSON.h"

Downloader downler;
SmartChargerXML smchaXML;
WechselrichterJSON smchaJSON;

// Ausgabe von Warnungen über die Services auf dem Raspberry Pi
// gesteuert über globales Flag Byte
// Flag: Bit 0 = 1 -> EDLD antwortet nicht korrekt
// Flag: Bit 1 = 1 -> MBMD antwortet nicht korrekt
extern quint8 m_messageFlag;


using namespace std::chrono_literals;


PowerNodeModel::PowerNodeModel() {

    connect(&m_dataTimer, &QTimer::timeout, this, &PowerNodeModel::onDataTimer);
    m_dataTimer.start(500);
}

PowerNodeModel::~PowerNodeModel() {
}

// cyclically retrieve data from converters and wallbox
void PowerNodeModel::onDataTimer() {

    static int timerCounter = 0;

    if ((timerCounter++ % 10) == 0)  // alle 3 Sekunden den RPi abfragen (6 * 500 ms)
    {
    // Update the different values in C++
        setComm();              // switch on communication visu
        getXMLdata();           // extract values from XML string, read from RPi EDL Daemon
        getJSONdata();          // extract values from JSON string, read from RPi MBMD Daemon
        generatorHandling();    // PV generator handling
        batteryHandling();      // battery handling
        gridHandling();         // grid handling
        wallboxHandling();      // wallbox handling
        arrowsHandling();       // arrows handling
        consumptionHandling();  // consumption handling
        shadeHandling();        // handle shades for home with fractional grid power and fractional battery power
        setEDLDText();          // emit warning message if connection to EDL Daemon on RPi ceases
        setMBMDText();          // emit warning message if connection to MBMD Daemon on RPi ceases
        setBGColor();           // on comm error make background light red
        setSunAngle();          // slowly rotate sun incon

    // Update the different values in QML
        emit arrowsChanged();
        emit gridDataChanged();
        emit generatorDataChanged();
        emit batteryDataChanged();
        emit chargingDataChanged();
        emit consumptionDataChanged();
        emit shadeDataChanged();
        emit gridDataChanged();
        emit arrowsChanged();
        emit setEDLDWarning();
        emit setMBMDWarning();
        emit setBackgroundColor();
        emit rotateSun();
        emit showComm();
    }
    else    // zyklisch die Sonne etwas drehen
    {
        resetComm();             // switch off communication visu
        setSunAngle();
        emit rotateSun();
        emit showComm();
    }
}

// handling routines, may be called from MQTT routines (comment rand() calls then ;)
void PowerNodeModel::getXMLdata(void)
{
    // download XML data from SmartCharger XML page into **global**  m_XMLfiledata
    extern Downloader downler;
    downler.doDownloadXML();

    // decode XML data from m_XMLfiledata into member variables of SmartChargerXML
    extern SmartChargerXML smchaXML;
    smchaXML.ReadSmartChargerXML();
}

void PowerNodeModel::getJSONdata(void)
{
    // download JSON data from mbmd JSON page into **global**  m_JSONfiledata
    extern Downloader downler;
    downler.doDownloadJSON();

    // decode JSON data from m_JSONfiledata into member variables of SmartChargerJSON
    extern WechselrichterJSON smchaJSON;
    smchaJSON.ReadWechselrichterJSON();
}

void PowerNodeModel::setMBMDText(void)              // Fehlermeldung wenn MBMD Daemon Probleme hat
{
    if (m_messageFlag & MBMDFlag)
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

void PowerNodeModel::setBGColor(void)               // Hintergrundfarbe ändern wenn auf dem RPi Probleme auftreten
{
    if (m_messageFlag & (EDLDFlag | MBMDFlag))      // EDLD oder MBMD
    {
        m_backgroundColor = "#FFC8C8";              // sehr helles Rot
    }
    else
    {
        m_backgroundColor = "whitesmoke";
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
}

// rotate sun icon
void PowerNodeModel::setSunAngle(void)
{
    m_sunAngle += .5;
    if (m_sunAngle > 360.0) m_sunAngle = 0.0;
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


// PV generator handling -----------------------------------------------------
void PowerNodeModel::generatorHandling(void)
{
#if defined DEMOMODE
    m_generatorPowerDach = rand() % 9000;
    m_generatorPowerGaube = rand() % 3600;
    m_generatorPowerGarage = rand() % 3000;
    m_generatorTotalEnergy = (rand() % 66000) + 30000;  // im Bereich ab 66 MWh
#endif


    m_generatorPowerDach = (smchaJSON.getPVDachActualPower());
    m_generatorPowerGarage = (smchaJSON.getPVGarageActualPower());
    m_generatorPowerGaube = (smchaJSON.getPVGaubeActualPower());

    m_generatorPowerTotal   =   m_generatorPowerDach
                            +   m_generatorPowerGaube
                            +   m_generatorPowerGarage;

    m_generatorTotalEnergy = (smchaJSON.getPVGesamtErtrag());

//        m_generatorPowerTotal = 230;                    // test

    if(m_generatorPowerTotal == 0) {
        m_generatorColor = VLIGHTGRAY;                  // helles Hellgrau, keine QML Basic/SVG color
    }
    else {
        m_generatorColor = LIMEGREEN;                   // Hellgrün
    }
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
    extern SmartChargerXML smchaXML;
    m_batteryPower = -(smchaXML.getStorageSystemActualPower());
    m_batteryPercentage = smchaXML.getStorageSystemSOC();
    m_battTemp = smchaXML.getStorageSystemTemperature();
#endif

    m_battPowerAnzeige = m_batteryPower;
    // change text and color depending on power value
    if(m_battPowerAnzeige == 0) {
        m_batteryText = "";                             // kein Strom  -> kein Text
        m_batteryColor = VLIGHTGRAY;                    // helles Hellgrau, keine QML Basic/SVG color
    }
    else if(m_battPowerAnzeige > 0) {
        m_batteryText = "Batterie-ladung";              // Batterie wird geladen
        m_batteryColor = LIMEGREEN;                     // Hellgrün
    }
    else {                                              // Batterie wird entladen
        m_battPowerAnzeige = abs(m_battPowerAnzeige);           // auch negative Werte (bei Entladung) werden positiv dargestellt...
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
    extern SmartChargerXML smchaXML;
    m_gridEnergyImport = smchaXML.getSmartMeterConsumption();
    m_gridEnergyExport = smchaXML.getSmartMeterSurplus();
    m_gridPower = -(smchaXML.getSmartMeterActualPower());
#endif

//    m_gridPowerAnzeige = m_gridPower;                   // für die Anzeige eine extra Var benutzen wegen abs()
    m_gridPowerAnzeige = abs(m_gridPower);              // Werte nur positiv anzeigen, Richtung kommt über die Farbe und die Pfeile

    if (m_gridPower < -9)                              // Netzbezug
    {
        m_gridColor = FIREBRICK;                        // Dunkelrot
        m_gridText = "Netzbezug";
    }
    else if (m_gridPower >= 9)                         // Einspeisung
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
        m_wallboxScoot  = "Icons/electric-scooter_icon_steckt_weiss_transparent_rad.png";

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
    extern SmartChargerXML smchaXML;
    m_evalPoints = smchaXML.getEVEvaluationPoints();
    m_chargingPower = smchaXML.getEVActualPower();
    m_chargedEnergy = smchaXML.getEVTotalEnergy();
    m_sessionEnergy = smchaXML.getEVSessionEnergy();

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

    if (smchaXML.getEVState() == 2)             // state ready for charging (alles vorbereitet)
    {
        m_wallboxColor = DODGERBLUE;            // Ladung vorbereitet -> schickes helles Blau
    }
    else if ((smchaXML.getEVState() == 3) && (m_chargingPower > 0))  // state=charging && power>0
    {
        m_wallboxColor = DARKBLUE;              // Ladung startet oder ist beendet -> dunkles Blau
    }
    else if (smchaXML.getEVState() == 4)        // Error oder rejected
    {
        m_wallboxColor = FIREBRICK ;            // dunkles Rot
    }
    else if (((smchaXML.getEVState() <= 2) || (smchaXML.getEVState() == 5))
         && ((smchaXML.getEVPlug() == 5) || (smchaXML.getEVPlug() == 7)))   // Stecker steckt aber Wallbox not ready
    {
        m_wallboxColor = LIGHTBLUE ;            // helles Blau
    }
    else
        m_wallboxColor = VLIGHTGRAY ;           // helles Hellgrau, keine QML Basic/SVG color

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
        m_wallboxCar = "Icons/electric-car-icon_steckt_weiss_transparent.png";
        m_wallboxScoot  = "Icons/electric-scooter_icon_steckt_weiss_transparent_rad.png";
    }
    else
    {
        m_wallboxCar = "Icons/electric-car-icon_weiss_transparent.png";
        m_wallboxScoot  = "Icons/electric-scooter_icon_weiss_transparent_rad.png";
    }

    // all conditions true for charging but no current flowing -> EV state is "fully charged"
    if ((smchaXML.getEVPlug() >= 5) && (smchaXML.getEVState() == 2) && (m_chargingPower == 0))
    {
        m_wallboxCar = "Icons/electric-car-icon_steckt_gruen_transparent.png";
        m_wallboxScoot  = "Icons/electric-scooter_icon_steckt_gruen_transparent_rad.png";
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
    extern SmartChargerXML smchaXML;
    // die Null steht für die PV-Leistung vom Dach
    m_totalPowerConsumption = m_generatorPowerTotal     - (  (m_gridPower)
                                                        + (m_chargingPower)
                                                        + (m_batteryPower) );

    m_totalPowerConsumption = abs(m_totalPowerConsumption);  // Werte nur positiv anzeigen, Richtung kommt über die Farbe und die Pfeile
#endif

    if(m_totalPowerConsumption == 0.0) {
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
    if((m_gridPower > 9) && (m_generatorPowerTotal > 0))
    {
        m_pv2grid = true;
    }
    else
    {
        m_pv2grid = false;
    }

    // battery to house
    if(((m_batteryPower < -9) && (m_generatorPowerTotal == 0)) ||     // PV keine Leistung
            ((m_batteryPower < -9) && (m_totalPowerConsumption > 0))) // Haus braucht Energie
    {
        m_batt2house = true;
    }
    else
    {
        m_batt2house = false;
    }

    // house to battery (battery conditioning when PV is off for long time)
    if((m_batteryPower > 9) && (m_generatorPowerTotal == 0) &&
            (m_totalPowerConsumption > 0) && (m_gridPower < 0))
    {
        m_house2batt = true;
    }
    else
    {
        m_house2batt = false;
    }

    // grid to house
    if((m_gridPower < -9) /*&& (m_totalPowerConsumption > 10)*/)
    {
        m_grid2house = true;
    }
    else
    {
        m_grid2house = false;
    }

    // house to grid        // generator is off (night) and battery feeds house and temporarily the grid
    if((m_gridPower > 9) && (m_generatorPowerTotal == 0))    // m_gridPowerAnzeige >= 10
    {
        m_house2grid = true;
    }
    else
    {
        m_house2grid = false;
    }

    // house to wallbox (charging of elctric vehicle)
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
    if(m_gridPower < 0){                            // Netzbezug
        m_homeBotRedH   = std::min((abs(m_gridPower) / m_totalPowerConsumption), (double)1) * 270;    // Höhe Home rectangle = 270 (war (double).5)
    }
    else
    {
        m_homeBotRedH = 0;
    }

    // Anteil Akkubezug in GRÜN von unten kommend einblenden
    if(m_batteryPower < 0){                            // Batteriebezug
        m_homeTopGreenH = std::min((abs(m_batteryPower) / m_totalPowerConsumption), (double)1) * 270;    // Höhe Home rectangle = 270 (war (double).5)
    }
    else
    {
        m_homeTopGreenH = 0;
    }
}
