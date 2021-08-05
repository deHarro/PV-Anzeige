#include <algorithm>    // std::max
#include "PowerNodeModel.h"

#include <chrono>

#if defined USEMQTT
    #include "Types.h"
    #include <msgpack.h>
#endif

using namespace std::chrono_literals;

#if defined USEMQTT
PowerNodeModel::PowerNodeModel(QMQTT::Client& mqttClient)
    : m_client(mqttClient) {
    connect(&m_dataTimer, &QTimer::timeout, this, &PowerNodeModel::onDataTimer);
    m_dataTimer.start(2000);

    connect(&m_client, &QMQTT::Client::connected, this, &PowerNodeModel::onConnected);
    connect(&m_client, &QMQTT::Client::disconnected, this, &PowerNodeModel::onDisconnected);
    connect(&m_client, &QMQTT::Client::error, this, &PowerNodeModel::onError);
    connect(&m_client, &QMQTT::Client::subscribed, this, &PowerNodeModel::onSubscribed);
    connect(&m_client, &QMQTT::Client::received, this, &PowerNodeModel::onReceived);

    m_client.connectToHost();
}
#else
PowerNodeModel::PowerNodeModel() {
    connect(&m_dataTimer, &QTimer::timeout, this, &PowerNodeModel::onDataTimer);
    m_dataTimer.start(2000);
}
#endif

PowerNodeModel::~PowerNodeModel() {
}

// demo - change / increment values
void PowerNodeModel::onDataTimer() {

// Update the different values in C++
    generatorHandling();    // PV generator handling
    batteryHandling();      // battery handling
    gridHandling();         // grid handling
    wallboxHandling();      // wallbox handling
    consumptionHandling();  // consumption handling
    arrowsHandling();       // arrows handling
    shadeHandling();        // handle shades for home with fractional grid power and fractional battery power

// Update the different values in QML
    emit generatorDataChanged();
    emit batteryDataChanged();
    emit gridDataChanged();
    emit chargingDataChanged();
    emit consumptionDataChanged();
    emit arrowsChanged();
    emit shadeDataChanged();
}

// handling routines, may be called from MQTT routines (comment rand() calls then ;)
// PV generator handling -----------------------------------------------------
void PowerNodeModel::generatorHandling(void)
{
#if defined DEMOMODE
    m_generatorPowerDach = rand() % 9000;
    m_generatorPowerGaube = rand() % 3600;
    m_generatorPowerGarage = rand() % 3000;
#endif

    m_generatorPowerTotal   =   m_generatorPowerDach
                            +   m_generatorPowerGaube
                            +   m_generatorPowerGarage;

//        m_generatorPowerTotal = 1000;                    // test

    if(m_generatorPowerTotal == 0) {
        m_generatorColor = VLIGHTGRAY;                  // helles Hellgrau, keine QML Basic/SVG color
//        m_pv2batt = false;
    }
    else {
        m_generatorColor = LIMEGREEN;                   // Hellgrün
//        if(m_batteryPower > 0){
//            m_pv2batt = true;
//        }
    }
}

// battery handling ----------------------------------------------------------
void PowerNodeModel::batteryHandling(void)
{
#if defined DEMOMODE
    m_batteryPower = (rand() % 10000) - 5000;
    //    m_batteryPower = 0;               // test
    m_batteryPercentage = rand() % 100;
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


    // change battery icon depending on battery percentage
    if( m_batteryPercentage <= 5)
    {
        m_batteryImage = "/Icons/Akku_weiss_transparent00.png";
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
#if defined DEMOMODE
    m_gridEnergyImport += (10 + (rand() % 100));    // Verbrauchszähler Richtung Netz
    m_gridEnergyExport += (100 + (rand() % 100));   // Einspeisezähler Richtung Netz

    m_gridPower = (rand() % 10000) - 5000;
//    m_gridPower = 0;
#endif

    m_gridPowerAnzeige = m_gridPower;               // für die Anzeige eine extra Var benutzen wegen abs()
    if(m_gridPowerAnzeige == 0) {
        m_gridText = "";                            // kein Strom  -> kein Text
        m_gridColor = VLIGHTGRAY;                   // helles Hellgrau, keine QML Basic/SVG color
    }
    else if (m_gridPowerAnzeige > 0) {
        m_gridColor = LIMEGREEN;                    // Hellgrün
        m_gridText = "Netz-einspeisung";
    }
    else {
        m_gridPowerAnzeige = abs(m_gridPowerAnzeige); // Werte nur positiv anzeigen, Richtung kommt über die Farbe und die Pfeile

        m_gridColor = FIREBRICK;                    // Dunkelrot
        m_gridText = "Netzbezug";
    }
}

// wallbox handling ----------------------------------------------------------
void PowerNodeModel::wallboxHandling()
{
#if defined DEMOMODE
//    m_chargingPower = rand() % 4000;
    m_chargedEnergy += 10;
    m_sessionEnergy += 10;

    bool test = rand() % 2;                      // test
    test == 1 ? m_chargingPower = rand() % 4000 : m_chargingPower = 0;    // test
    m_evAttached = rand() % 2;                  // test
    m_evAttached == 1 ? m_evAttached = true : m_evAttached = false;    // test
#endif

    if(m_evAttached == true) {                      // cable attached to ev (car/bike)

        if(m_chargingPower > 0){                    // keine Ladung aktiv ->
            m_wallboxColor = DODGERBLUE;            // schickes Blau
        }
        else
        {
            m_wallboxColor = DARKBLUE;              // dunkles Blau
        }
    }
    else {
        m_wallboxColor =VLIGHTGRAY ;                // helles Hellgrau, keine QML Basic/SVG color
    }
}

// consumption handling ----------------------------------------------------------
void PowerNodeModel::consumptionHandling(void)
{
#if defined DEMOMODE
    m_totalPowerConsumption = rand() % 10000;
    //m_totalPowerConsumption = 0;
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
    }
    else
    {
        m_pv2grid = false;
    }

    // battery to house
    if(((m_batteryPower < 0) && (m_generatorPowerTotal = 0)) ||     // PV keine Leistung
            ((m_batteryPower < 0) && (m_totalPowerConsumption > 0)))     // Haus braucht Energie
    {
        m_batt2house = true;
    }
    else
    {
        m_batt2house = false;
    }

    // house to battery (battery conditioning when PV is off for long time)
    if((m_batteryPower > 0) && (m_generatorPowerTotal = 0) && (m_totalPowerConsumption > 0) && (m_gridPower < 0))
    {
        m_house2batt = true;
    }
    else
    {
        m_house2batt = false;
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

// shade handling  ----------------------------------------------------------
void PowerNodeModel::shadeHandling(void)
{
    // Anteil Netzbezug in ROT von oben kommend einblenden
    if(m_gridPower < 0){                            // Netzbezug
        m_homeTopRedH = fmin((abs(m_gridPower) / m_totalPowerConsumption), (double).5) * 270;    // Höhe Home rectangle = 270
    }
    else
    {
        m_homeTopRedH = 0;
    }

    // Anteil Akkubezug in GRÜN von unten kommend einblenden
    if(m_batteryPower < 0){                            // Netzbezug
        m_homeBotGreenH = fmin((abs(m_batteryPower) / m_totalPowerConsumption), (double).5) * 270;    // Höhe Home rectangle = 270
    }
    else
    {
        m_homeBotGreenH = 0;
    }
}

#if defined USEMQTT
void PowerNodeModel::onConnected() {
    m_client.subscribe("sbfspot_1234567890/live");
}

void PowerNodeModel::onDisconnected() {

}

void PowerNodeModel::onError(const QMQTT::ClientError error) {

}

void PowerNodeModel::onSubscribed(const QString& topic) {

}

void PowerNodeModel::onReceived(const QMQTT::Message& message) {
    auto topic = message.topic();
    qDebug() << topic;

    // Inverter live data
    auto variant = MsgPack::unpack(message.payload()).toMap();
    //m_lastUpdate = variant.value(toIntString(InverterProperty::Timestamp)).toDateTime();
    //m_yieldTotal = variant.value(toIntString(InverterProperty::YieldTotal)).toDouble();
    //m_yieldToday = variant.value(toIntString(InverterProperty::YieldToday)).toDouble();
    m_generatorPowerTotal = variant.value(toIntString(InverterProperty::Power)).toDouble();

    // String live data
    // auto strings = variant.value(toIntString(InverterProperty::Strings)).toList();
    // int i = 0;
    // int j = 0;
    // m_powerDcTotal = 0.0;
    // for (; (i < strings.size()) && (j < m_stringLiveData.size()); ++i, ++j) {
    //     m_stringLiveData[i]->power = strings.value(i).toMap().value(toIntString(InverterProperty::StringPower)).toReal();
    //     m_powerDcTotal += m_stringLiveData[i]->power;
    // }
}
#endif
