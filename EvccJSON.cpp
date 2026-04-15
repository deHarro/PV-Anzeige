#include "EvccJSON.h"
#include "Downloader.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Downloader downler; // Diese EINE Zeile muss bleiben, da sie global gebraucht wird

extern QString m_JSONevccData;    // globally defined in main.cpp, loaded with data by Downloader.cpp

EvccJSON::EvccJSON() {}

// Hilfsfunktion: Wandelt verschachteltes JSON in eine flache Map um
// Beispiel: "pv" -> Array -> [0] -> "power" wird zu Key: "pv[0].power"
void flattenJson(const QJsonValue &value, const QString &prefix, QMap<QString, QJsonValue> &map) {
    if (value.isObject()) {
        QJsonObject obj = value.toObject();
        for (auto it = obj.begin(); it != obj.end(); ++it) {
            flattenJson(it.value(), prefix.isEmpty() ? it.key() : prefix + "." + it.key(), map);
        }
    } else if (value.isArray()) {
        QJsonArray arr = value.toArray();
        for (int i = 0; i < arr.size(); ++i) {
            flattenJson(arr[i], prefix + "[" + QString::number(i) + "]", map);
        }
    } else {
        map.insert(prefix, value);
    }
}

void EvccJSON::ReadEvccJSON() {
    if (m_JSONevccData.isEmpty()) return;

    QJsonDocument doc = QJsonDocument::fromJson(m_JSONevccData.toUtf8());
    if (doc.isNull() || !doc.isObject()) return;

    // 1. Alles flachklopfen
    QMap<QString, QJsonValue> data;
    flattenJson(doc.object(), "", data);

    // 2. Werte einfach "picken" (Wahlfreier Zugriff)

    // Einfache Werte (Wurzel)
    m_SmartMeterSurplus       = data.value("aux[0].energy").toDouble();
    m_totalPowerConsumption   = data.value("homePower").toInt();
    m_generatorPowerTotal     = data.value("pvPower").toDouble();
    m_PVGesamtErtrag          = data.value("pvEnergy").toDouble();
    m_prioritySOC             = data.value("prioritySoc").toInt();
    m_bufferSOC               = data.value("bufferSoc").toInt();
    m_batteryDisChargeControl = data.value("batteryDischargeControl").toBool();

    // Grid (Objekt-Pfad)
    m_SmartMeterActualPower   = data.value("grid.power").toDouble();
    m_SmartMeterConsumption   = data.value("grid.energy").toDouble();

    // Akku (Array-Pfad)
    m_StorageSystemSOC         = data.value("battery.soc").toDouble();
    m_StorageSystemActualPower = data.value("battery.power").toDouble();

    // Loadpoints (Wallbox)
    QString loadpoints = "loadpoints[0].";
    m_EVActualPower      = data.value(loadpoints + "chargePower").toDouble();
    m_EVSessionEnergy    = data.value(loadpoints + "sessionEnergy").toDouble();
    m_EVTotalEnergy      = data.value(loadpoints + "chargeTotalImport").toDouble();
    m_EVChargerPhases    = data.value(loadpoints + "phasesActive").toInt();
    m_EVconfiguredPhases = data.value(loadpoints + "phasesConfigured").toInt();
    m_EVdisableDelay     = data.value(loadpoints + "disableDelay").toInt();
    m_EVenableDelay      = data.value(loadpoints + "enableDelay").toInt();

    // EVCC hat 3 Booleans die den Zustand der Kombination EV-Wallbox beschreiben:
    // connected, enabled, charging
    bool connected      = data.value(loadpoints + "connected").toBool();        //
    bool charging       = data.value(loadpoints + "charging").toBool();              //
    bool enabled       = data.value(loadpoints + "enabled").toBool();              //

    // Daraus muss ein Schema abgeleitet werden, das die Abfragen zum Wallbox-Coloring ergibt
    m_EVPlug = connected == true ? 5 : 0;           // Kabel ist verbunden (an Wallbox und EV)
    if(charging == true)
        if(m_EVActualPower > 0) m_EVState = 3;      // lädt
        else m_EVState = 2;                         // lädt nicht (mehr)
    else if(enabled == true)
        m_EVState = 2;                              // könnte laden, lädt aber nicht mehr -> Akku ist voll oder Ladestand ereicht -> Stecker GRÜN
        else m_EVState = 0;                         // lädt nicht

    // Lademodus: OFF, PV, MINPV, NOW
    // Hier den Text für die GUI zusammenstellen
    QString mode         = data.value(loadpoints + "mode").toString();
    m_EVChargingMode     = (mode == "off" ? "OFF" : mode == "pv" ? "SURPLUS" : mode == "now" ? "QUICK" : "Min+PV");

    // Dynamische Suche für PV-Wechselrichter (nach Titel)
    // Da wir nicht wissen, an welchem Index "Garage" steht, loopen wir kurz durch die Map
    for (int i = 0; i < 10; ++i) { // Annahme: Max 10 PV-Einträge
        QString base = QString("pv[%1].").arg(i);
        if (!data.contains(base + "title")) break;

        QString title = data.value(base + "title").toString();
        double power = data.value(base + "power").toDouble();
        double energy = data.value(base + "energy").toDouble();

        if (title == "Garage") { m_PVGarageActualPower = power; m_PVGarageErtrag = energy; }
        if (title == "Dach")   { m_PVDachSActualPower  = power; m_PVDachSErtrag  = energy; }
        if (title == "Gaube")  { m_PVGaubeActualPower  = power; m_PVGaubeErtrag  = energy; }
        if (title == "DachN")  { m_PVDachNActualPower  = power; m_PVDachNErtrag  = energy; }
    }

    qDebug() << "Parsing erfolgreich abgeschlossen.";
}

EvccJSON::~EvccJSON() {}

// getter Funktionen
double          EvccJSON::getPVDachSActualPower(void)       {return m_PVDachSActualPower; }
double          EvccJSON::getPVDachNActualPower(void)       {return m_PVDachNActualPower; }
double          EvccJSON::getPVGaubeActualPower(void)       {return m_PVGaubeActualPower; }
double          EvccJSON::getPVGarageActualPower(void)      {return m_PVGarageActualPower; }
double          EvccJSON::getPVGesamtErtrag(void)           {return m_PVGesamtErtrag;}
double          EvccJSON::getPVDachNErtrag(void)            {return m_PVDachNErtrag; }
double          EvccJSON::getPVDachSErtrag(void)            {return m_PVDachSErtrag; }
double          EvccJSON::getPVGaubeErtrag(void)            {return m_PVGaubeErtrag; }
double          EvccJSON::getPVGarageErtrag(void)           {return m_PVGarageErtrag; }

double          EvccJSON::getSmartMeterActualPower(void)    {return m_SmartMeterActualPower; }
double          EvccJSON::getSmartMeterConsumption(void)    {return m_SmartMeterConsumption; }
double          EvccJSON::getSmartMeterSurplus(void)        {return m_SmartMeterSurplus; }

int             EvccJSON::getStorageSystemSOC(void)         {return m_StorageSystemSOC; }
double          EvccJSON::getStorageSystemTemperature(void) {return m_StorageSystemTemperature; }
double          EvccJSON::getStorageSystemActualPower(void) {return m_StorageSystemActualPower; }

double          EvccJSON::getEVEvaluationPoints(void)       {return m_EVEvaluationPoints;}
int             EvccJSON::getEVState(void)                  {return m_EVState;}
double          EvccJSON::getEVMaxPhases(void)              {return m_EVMaxPhases;}
int             EvccJSON::getEVPlug(void)                   {return m_EVPlug;}
double          EvccJSON::getEVSystemEnabled(void)          {return m_EVSystemEnabled;}
//int             EvccJSON::getEVOutput(void)                 {return m_EVOutput;}
int             EvccJSON::getEVOutput(void)                 {return m_EVconfiguredPhases;}
double          EvccJSON::getEVActualPower(void)            {return m_EVActualPower;}
double          EvccJSON::getEVSessionEnergy(void)          {return m_EVSessionEnergy;}
QString         EvccJSON::getEVChargeMode(void)             {return m_EVChargingMode;}
double          EvccJSON::getEVTotalEnergy(void)            {return m_EVTotalEnergy * 1000.0;}
int             EvccJSON::getEVconfiguredPhases(void)       {return m_EVconfiguredPhases;}
// int             EvccJSON::getEVactivePhases(void)           {return m_EVChargerPhases;}
int             EvccJSON::getEVactivePhases(void)           {return m_EVconfiguredPhases;}
int             EvccJSON::getEVprioritySOC(void)            {return m_prioritySOC;}
int             EvccJSON::getEVbufferSOC(void)              {return m_bufferSOC;}
bool            EvccJSON::getEVbattDcControl(void)          {return m_batteryDisChargeControl;}
unsigned char   EvccJSON::getEVdisableDelay (void)          {return m_EVdisableDelay;}
unsigned char   EvccJSON::getEVenableDelay (void)           {return m_EVenableDelay;}



