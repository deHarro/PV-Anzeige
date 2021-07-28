#include "PowerNodeModel.h"

#include <chrono>

using namespace std::chrono_literals;

PowerNodeModel::PowerNodeModel() {
    connect(&m_dataTimer, &QTimer::timeout, this, &PowerNodeModel::onDataTimer);
    m_dataTimer.start(1000);
}

PowerNodeModel::~PowerNodeModel() {
}

// demo - change / increment values
void PowerNodeModel::onDataTimer() {

    // PV generator handling -----------------------------------------------------
    generatorHandling();
    emit generatorDataChanged();

    // battery handling ----------------------------------------------------------
    batteryHandling();
    emit batteryDataChanged();

    // grid handling -------------------------------------------------------------
    gridHandling();
    emit gridDataChanged();

    // wallbox handling ----------------------------------------------------------
    wallboxHandling();
    emit chargingDataChanged();

    // consumption handling ----------------------------------------------------------
    consumptionHandling();
    emit consumptionDataChanged();

    // arrows handling
    arrowsHandling();
    emit arrowsDataChanged();
}

// handling routines, may be called from MQTT routines (comment rand() calls then ;)
// PV generator handling -----------------------------------------------------
void PowerNodeModel::generatorHandling(void)
{
    m_generatorPowerDach = rand() % 9000;
    m_generatorPowerGaube = rand() % 3600;
    m_generatorPowerGarage = rand() % 3000;

    m_generatorPowerTotal = m_generatorPowerDach
                          + m_generatorPowerGaube
                          + m_generatorPowerGarage;

//    m_generatorPowerTotal = 0;                // test

    if(m_generatorPowerTotal == 0) {
        m_generatorColor = VLIGHTGRAY;               // helles Hellgrau, keine QML Basic/SVG color
        m_pv2batt = false;
    }
    else {
        m_generatorColor = LIMEGREEN;                // Hellgrün
        if(m_batteryPower > 0){
            m_pv2batt = true;
        }
    }
}

// battery handling ----------------------------------------------------------
void PowerNodeModel::batteryHandling(void)
{
    m_batteryPower = (rand() % 10000) - 5000;

//    m_batteryPower = 0;               // test

    // change text and color depending on power value
    if(m_batteryPower == 0) {
        m_batteryText = "";                     // kein Strom  -> kein Text
        m_batteryColor = VLIGHTGRAY;            // helles Hellgrau, keine QML Basic/SVG color
        m_batt2house = false;
    }
    else if(m_batteryPower > 0) {
        m_batteryText = "Batterie-ladung";      // Batterie wird geladen
        m_batteryColor = LIMEGREEN;             // Hellgrün
        m_batt2house = false;
    }
    else {                                      // Batterie wird entladen
        m_batteryPower = abs(m_batteryPower);   // auch negative Werte (bei Entladung) werden positiv dargestellt...
        m_batteryText = "Batterie-entladung";   // ... nur der Text ändert sich
        m_batteryColor = FORESTGREEN;           // Dunkelgrün
        m_batt2house = true;
    }

    m_batteryPercentage = rand() % 100;

    // change battery icon depending on battery percentage
    if( m_batteryPercentage <= 5)
    {
        m_batteryImage = "//Icons//Akku_weiss_transparent00.png";
    }
    else if (m_batteryPercentage > 5 && m_batteryPercentage < 39)
    {
       m_batteryImage = "/Icons/Akku_weiss_transparent20.png";
    }
    else if (m_batteryPercentage > 40 && m_batteryPercentage < 59)
    {
        m_batteryImage = "/Icons/Akku_weiss_transparent40.png";
    }
    else if (m_batteryPercentage > 60 && m_batteryPercentage < 79)
    {
        m_batteryImage = "/Icons/Akku_weiss_transparent60.png";
    }
    else if (m_batteryPercentage > 80 && m_batteryPercentage < 96)
    {
        m_batteryImage = "/Icons/Akku_weiss_transparent80.png";
    }
    else
    {
        m_batteryImage = "/Icons/Akku_weiss_transparent100.png";
    }
}

// grid handling -------------------------------------------------------------
void PowerNodeModel::gridHandling(void)
{
    m_gridEnergyImport += (10 + (rand() % 100));    // das ist der Verbrauchszähler Richtung Netz
    m_gridEnergyExport += (100 + (rand() % 100));   // das ist der Einspeisezähler Richtung Netz

    m_gridPower = (rand() % 10000) - 5000;
    if(m_gridPower == 0) {
        m_gridText = "";                        // kein Strom  -> kein Text
        m_gridColor = VLIGHTGRAY;               // helles Hellgrau, keine QML Basic/SVG color
    }
    else if (m_gridPower > 0) {
        m_gridText = "Netz-einspeisung";
        m_gridColor = LIMEGREEN;                // Hellgrün
    }
    else {
        m_gridPower = abs(m_gridPower);
        m_gridText = "Netzbezug";
        m_gridColor = FIREBRICK;                // Dunkelrot
    }
}

// wallbox handling ----------------------------------------------------------
void PowerNodeModel::wallboxHandling()
{
    m_chargingPower = rand() % 4000;
    m_chargedEnergy += 10;
    m_sessionEnergy += 10;

//    m_chargingPower = 0;                   // test
//    m_evAttached = true;                   // test

    if(m_evAttached == true) {                  // cable attached to ev (car/bike)

        if(m_chargingPower > 0){                // keine Ladung aktiv ->
            m_wallboxColor = DODGERBLUE;        // schickes Blau
        }
        else
        {
            m_wallboxColor = DARKBLUE;              // dunkles Blau
        }
    }
    else {
        m_wallboxColor =VLIGHTGRAY ;            // helles Hellgrau, keine QML Basic/SVG color
    }
}

// consumption handling ----------------------------------------------------------
void PowerNodeModel::consumptionHandling(void)
{
    m_totalPowerConsumption = rand() % 10000;
    if(m_totalPowerConsumption == 0) {
        m_homeColor = VLIGHTGRAY;               // helles Hellgrau, keine QML Basic/SVG color
    }
    else {
        m_homeColor = LIMEGREEN;                // Hellgrün
        //rectangle5.color = LIMEGREEN;         // Hellgrün
    }
}

// arrows handling ----------------------------------------------------------
void PowerNodeModel::arrowsHandling(void)
{
    // PV to battery
    if((m_batteryPower > 0) && (m_generatorPowerTotal > 0))
    {
        m_pv2batt = true;
    }
    else
    {
        m_pv2batt = false;
    }

    // PV to house
    if((m_totalPowerConsumption > 0) && (m_generatorPowerTotal > 0))
    {
        m_pv2house = true;
    }
    else
    {
        m_pv2house = false;
    }

    // PV to grid
    if((m_gridPower > 0) && (m_generatorPowerTotal > 0))
    {
        m_pv2grid = true;
        //image10.visible = true;
    }
    else
    {
        m_pv2grid = false;
    }

    // battery to house
    if((m_batteryPower < 0) && (m_generatorPowerTotal = 0))
    {
        m_batt2house = true;
    }

    // house to battery (battery conditioning when PV is off for long time)
    if((m_batteryPower > 0) && (m_generatorPowerTotal = 0))
    {
        m_house2batt = true;
    }

    // grid to house
    if((m_gridPower < 0) && (m_totalPowerConsumption > 0))
    {
        m_grid2house = true;
    }
    else
    {
        m_grid2house = false;
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
