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

}

// PV generator handling -----------------------------------------------------
void PowerNodeModel::generatorHandling(void)
{
    m_generatorPowerTotal = rand() % 10000;
    m_generatorPowerDach += 10;
    m_generatorPowerGaube += 10;
    m_generatorPowerGarage += 10;

    if(m_generatorPowerTotal == 0) {
        m_generatorColor = VLIGHTGRAY;               // helles Hellgrau, keine QML Basic/SVG color
    }
    else {
        m_generatorColor = LIMEGREEN;                // Hellgrün
        //rectangle5.color = LIMEGREEN;         // Hellgrün
    }

}

// battery handling ----------------------------------------------------------
void PowerNodeModel::batteryHandling(void)
{
    m_batteryPower = (rand() % 10000) - 5000;

    // change text and color depending on power value
    if(m_batteryPower == 0) {
        m_batteryText = "";                     // kein Strom  -> kein Text
        m_batteryColor = VLIGHTGRAY;            // helles Hellgrau, keine QML Basic/SVG color
    }
    else if(m_batteryPower > 0) {
        m_batteryText = "Batterie-ladung";      // Batterie wird geladen
        m_batteryColor = LIMEGREEN;             // Hellgrün
        //rectangle2.color = LIMEGREEN;         // Hellgrün
    }
    else {                                      // Batterie wird entladen
        m_batteryPower = abs(m_batteryPower);   // auch negative Werte (bei Entladung) werden positiv dargestellt...
        m_batteryText = "Batterie-entladung";   // ... nur der Text ändert sich
        m_batteryColor = FORESTGREEN;           // Dunkelgrün
    }

    m_batteryPercentage = rand() % 100;
/*    switch (m_batteryPercentage) {            // schade, das geht nicht, klappt nur in GCC
        case 0 ... 19:
            //rectangle2.image = Akku_weiss_transparent00.png;
            break;
        case 20 ... 39:
            //rectangle2.image = Akku_weiss_transparent20.png;
            break;
        case 40 ... 59:
            //rectangle2.image = Akku_weiss_transparent40.png;
            break;
        case 60 ... 79:
            //rectangle2.image = Akku_weiss_transparent60.png;
            break;
        case 80 ... 96:
            //rectangle2.image = Akku_weiss_transparent80.png;
            break;
        case 97 ... 100:
            //rectangle2.image = Akku_weiss_transparent100.png;
            break;
        default:
            //rectangle2.image = Akku_weiss_transparent100.png;
            break;
    }
*/

    // change battery icon depending on battery percentage
    if(m_batteryPercentage > 0 && m_batteryPercentage < 19)
    {
        //rectangle2.image = Akku_weiss_transparent00.png;
    }
    else if (m_batteryPercentage > 20 && m_batteryPercentage < 39)
    {
        //rectangle2.image = Akku_weiss_transparent20.png;
    }
    else if (m_batteryPercentage > 40 && m_batteryPercentage < 59)
    {
        //rectangle2.image = Akku_weiss_transparent40.png;
    }
    else if (m_batteryPercentage > 60 && m_batteryPercentage < 79)
    {
        //rectangle2.image = Akku_weiss_transparent60.png;
    }
    else if (m_batteryPercentage > 80 && m_batteryPercentage < 96)
    {
        //rectangle2.image = Akku_weiss_transparent80.png;
    }
    else
    {
        //rectangle2.image = Akku_weiss_transparent100.png;
    }
}

// grid handling -------------------------------------------------------------
void PowerNodeModel::gridHandling(void)
{
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
    m_chargingPower = rand() % 10000;
    m_chargedEnergy += 10;
    m_sessionEnergy += 10;

    if(m_evAttached == true) {                  // cable attached to ev (car/bike)
        m_wallboxColor = DARKBLUE;              // dunkles Blau

        if(m_chargingPower > 0){                // keine Ladung aktiv ->
            m_wallboxColor = DODGERBLUE;        // schickes Blau
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
