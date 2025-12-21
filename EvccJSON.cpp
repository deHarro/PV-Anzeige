#include "EvccJSON.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

extern QString m_JSONevccData;    // globally defined in main.cpp, loaded with data by Downloader.cpp

EvccJSON::EvccJSON() {}

void EvccJSON::ReadEvccJSON() {
    QJsonDocument evccJSONdocument;
    QJsonArray jsonArray;
    QJsonValue value;

    if(!m_JSONevccData.isEmpty())                                               // QString enthält Daten?
    {
        // QString in QJsonObject umwandeln
        evccJSONdocument = QJsonDocument::fromJson(m_JSONevccData.toUtf8());    // Ja -> QJsonDocument erzeugen

        if (!evccJSONdocument.isNull())                                         // hat das Erzeugen des JSON Doc geklappt?
        {
            if (evccJSONdocument.isObject())
            {
                QJsonObject jsonObject = evccJSONdocument.object();

                for (auto &&value : jsonObject["aux"].toArray())                // suche Werte des Netzes (SMA Energy-Meter)
                {
                    if (value.isObject())
                    {
                        QJsonObject auxObj = value.toObject();
                        QString title   = auxObj["title"].toString();
                        double power    = auxObj["power"].toDouble();
                        double energy   = auxObj["energy"].toDouble();

                        qDebug() << "Name:" << title << ", Energy:" << energy << ", Power:" << power;

                        m_SmartMeterSurplus = energy;
                    }
                }
                qDebug() << "Aux fertig";

                for (auto &&value : jsonObject["battery"].toArray())              // suche Werte des Akku
                {
                    if (value.isObject())
                    {
                        QJsonObject auxObj = value.toObject();
                        QString title   = auxObj["title"].toString();
                        double power    = auxObj["power"].toDouble();
                        double capacity   = auxObj["capacity"].toDouble();
                        double soc   = auxObj["soc"].toDouble();

                        qDebug() << "Name:" << title <<
                            ", Capacity:" << capacity <<
                            ", Power:" << power <<
                            "Soc:" << soc << "%";

                        m_StorageSystemSOC          = soc;              // aktuelle prozentuale Ladung des Hausakku
                        m_StorageSystemTemperature  = 0.0;              // aktuelle Temperatur des Hausakku/des Wechselrichters (?)
                        m_StorageSystemActualPower  = power;            // aktuell vom Akku abgegebene/aufgenommene Leistung
                    }
                }
                qDebug() << "Battery fertig";

                if (jsonObject["grid"].isObject()) {                // suche Werte des Netzes
                    QJsonObject gridObj = jsonObject["grid"].toObject();
                    double power = gridObj["power"].toDouble();
                    double energy = gridObj["energy"].toDouble();

                    m_SmartMeterActualPower = power;
                    m_SmartMeterConsumption = energy;

                    qDebug() << "Grid Energy:" << energy << ", Grid Power:" << power;
                }
                qDebug() << "Grid fertig";

                if (jsonObject.contains("homePower")) {
                    QJsonValue homePower = jsonObject.value("homePower");                   // suche Werte des Netzes

                    qDebug() << "Wert von" << "homePower" << ":" << homePower.toVariant();  // oder value.toVariant() für andere Typen

                    m_totalPowerConsumption = homePower.toInt();
                }
                qDebug() << "homePower fertig";

                for (auto &&value : jsonObject["loadpoints"].toArray())                     // suche Werte der Wallbox
                {
                    if (value.isObject())
                    {
                        QJsonObject auxObj  = value.toObject();

                        QString title       = auxObj["title"].toString();                   // Wallbox
                        double power        = auxObj["chargePower"].toDouble();             // aktuelle Leistung ins EV
                        double totalImport  = auxObj["chargeTotalImport"].toDouble();       // Gesamt Energie aus der Wallbox
                        double sessionEnergy = auxObj["sessionEnergy"].toDouble();          // Energie in dieser Sitzung
                        QString mode        = auxObj["mode"].toString();                    // off/pv/minpv/now
                        int phasesActive    = auxObj["phasesActive"].toInt();               // 1 oder 3
                        int phasesConfigured    = auxObj["phasesConfigured"].toInt();       // 0 oder 1 oder 3
                        bool connected      = auxObj["connected"].toBool();                 //
                        bool enabled        = auxObj["enabled"].toBool();                   //
                        bool charging       = auxObj["charging"].toBool();                  //
                        QString chargerStatusReason = auxObj["chargerStatusReason"].toString();

                        qDebug() << "Power:" << power
                                 << ", TotalImport:" << totalImport
                                 << ", ChargedEnergy:" << sessionEnergy
                                 << ", Mode:" << mode
                                 << ", Phases configured:" << phasesConfigured
                                 << ", Phases active:" << phasesActive
                                 << ", Enabled:" << enabled
                                 << ", ChStatus:" << chargerStatusReason;

                        // Lademodus: OFF, PV, MINPV, NOW (EDLD: Surplus, Quick, Manual, Off)
                        // Hier den Text für die GUI zusammenstellen
                        m_EVChargingMode = (mode == "off" ? "OFF" :
                                            mode == "pv" ? "SURPLUS" :
                                            mode == "minpv" ? "Min+PV" :
                                            mode == "now" ? "QUICK" : "");
                        m_EVOutput = (phasesConfigured == 1 ? 0 : 1);   // Ausgang für Phasenumschaltrelais (0 -> 1 Phase, >1 -> 3 Phasen)
                        m_EVActualPower = power;                        // aktuell ans EV abgegebene Leistung
                        m_EVSessionEnergy = sessionEnergy;              // zuletzt ins EV geladene Energie
                        m_EVTotalEnergy = totalImport;                  // gesamte jemals in ein EV eingeladene Energie
                        m_EVChargerPhases = phasesActive;               // Rückmeldung Anzahl Phasen von EVCC
                        m_EVconfiguredPhases = phasesConfigured;        // konfigurierte Anzahl Phasen beim Laden: 1 oder 3

                        // EVCC hat 3 Booleans die den Zustand der Kombination EV-Wallbox beschreiben:
                        // connected, enabled, charging
                        // Daraus muss ein Schema abgeleitet werden, das die Abfragen zum Wallbox-Coloring ergibt
                        m_EVPlug = connected == true ? 5 : 0;           // Kabel ist verbunden (an Wallbox und EV)
                        if(charging == true)
                            if(power > 0) m_EVState = 3;
                            else m_EVState = 2;
                        else m_EVState = 0;
                    }
                }
                qDebug() << "Wallbox fertig";

                for (auto &&value : jsonObject["pv"].toArray())                   // suche PV-Ertrag der WR
                {
                    if (value.isObject())
                    {
                        QJsonObject auxObj = value.toObject();
                        QString title   = auxObj["title"].toString();
                        double power    = auxObj["power"].toDouble();
                        double energy   = auxObj["energy"].toDouble();

                        qDebug() << "Name:" << title << ", Energy:" << energy << ", Power:" << power;

                        // Werte an die bekannten Membervariablen übergeben
                        if(title == "Garage")   {m_PVGarageActualPower = power; m_PVGarageErtrag = energy;}
                        if(title == "Dach")     {m_PVDachSActualPower  = power; m_PVDachSErtrag  = energy;}
                        if(title == "Gaube")    {m_PVGaubeActualPower  = power; m_PVGaubeErtrag  = energy;}
                        if(title == "DachN")    {m_PVDachNActualPower  = power; m_PVDachNErtrag  = energy;}

// der Gesamtertrag wird von EVCC als Wert "pvEnergy" zur Verfügung gestellt
//                        m_PVGesamtErtrag = m_PVGarageErtrag + m_PVDachSErtrag + m_PVGaubeErtrag + m_PVDachNErtrag;
//                        qDebug() << "Wert von" << "m_PVGesamtErtrag" << ":" << m_PVGesamtErtrag;
                    }
                }

                jsonArray = jsonObject["pvEnergy"].toArray();                       // suche Werte der PV-Energie

                if (jsonObject.contains("pvEnergy")) {
                    QJsonValue pvEnergy = jsonObject.value("pvEnergy");

                    qDebug() << "Wert von" << "pvEnergy" << ":" << pvEnergy.toVariant(); // oder value.toVariant() für andere Typen

                    m_PVGesamtErtrag = pvEnergy.toDouble();
                }
                qDebug() << "pvEnergy fertig";

                // gesamte aktuelle Generatorleistung (PV-Erzeugung m_generatorPowerTotal)
                jsonArray = jsonObject["pvPower"].toArray();                       // suche Werte des Netzes

                if (jsonObject.contains("pvPower")) {
                    QJsonValue pvPower = jsonObject.value("pvPower");

                    qDebug() << "Wert von" << "pvPower" << ":" << pvPower.toVariant(); // oder value.toVariant() für andere Typen

                    m_generatorPowerTotal = pvPower.toDouble();
                }
                qDebug() << "pvPower fertig";

                qDebug() << "WR fertig";

                // bis zu welchem %-Satz muss der Hausakku geladen sein, bevor das EV anfängt zu laden (bei knapper Sonne)
                if (jsonObject.contains("prioritySoc")) {
                    QJsonValue prioritySoc = jsonObject.value("prioritySoc");              // suche Werte des prioritySoc

                    qDebug() << "Wert von" << "prioritySoc" << ":" << prioritySoc.toVariant(); // oder value.toVariant() für andere Typen

                    m_prioritySOC = prioritySoc.toInt();
                }

                // bis zu welchem %-Satz muss der Hausakku geladen sein, bevor das EV anfängt zu laden (bei knapper Sonne)
                if (jsonObject.contains("prioritySoc")) {
                    QJsonValue bufferSoc = jsonObject.value("bufferSoc");              // suche Werte des prioritySoc

                    qDebug() << "Wert von" << "prioritySoc" << ":" << bufferSoc.toInt(); // oder value.toVariant() für andere Typen

                    m_bufferSOC = bufferSoc.toInt();         // Laden des EV aus dem Hausakku nur wenn der diesen %-Satz ereicht hat
                }

                if (jsonObject.contains("batteryDischargeControl")) {
                    QJsonValue batDisChControl = jsonObject.value("batteryDischargeControl"); // suche Werte des batteryDischargeControl

                    qDebug() << "Wert von" << "prioritySoc" << ":" << batDisChControl.toBool(); // oder value.toVariant() für andere Typen

                    m_batteryDisChargeControl   = batDisChControl.toBool();  // wenn true wird bei QUICK _nicht_ der Akku entladen
                }
            }
        }
        else
        {
            qDebug() << "Fehler beim Umwandeln des QString in JSON.";
        }
    }
}

EvccJSON::~EvccJSON() {}

// getter Funktionen
double  EvccJSON::getPVDachSActualPower(void)     {return m_PVDachSActualPower; }
double  EvccJSON::getPVDachNActualPower(void)     {return m_PVDachNActualPower; }
double  EvccJSON::getPVGaubeActualPower(void)     {return m_PVGaubeActualPower; }
double  EvccJSON::getPVGarageActualPower(void)    {return m_PVGarageActualPower; }
double  EvccJSON::getPVGesamtErtrag(void)         {return m_PVGesamtErtrag;}
double  EvccJSON::getPVDachNErtrag(void)          {return m_PVDachNErtrag; }
double  EvccJSON::getPVDachSErtrag(void)          {return m_PVDachSErtrag; }
double  EvccJSON::getPVGaubeErtrag(void)          {return m_PVGaubeErtrag; }
double  EvccJSON::getPVGarageErtrag(void)         {return m_PVGarageErtrag; }

double  EvccJSON::getSmartMeterActualPower(void)  {return m_SmartMeterActualPower; }
double  EvccJSON::getSmartMeterConsumption(void)  {return m_SmartMeterConsumption; }
double  EvccJSON::getSmartMeterSurplus(void)      {return m_SmartMeterSurplus; }

int     EvccJSON::getStorageSystemSOC(void)          {return m_StorageSystemSOC; }
double  EvccJSON::getStorageSystemTemperature(void)  {return m_StorageSystemTemperature; }
double  EvccJSON::getStorageSystemActualPower(void)  {return m_StorageSystemActualPower; }

double  EvccJSON::getEVEvaluationPoints(void)       {return m_EVEvaluationPoints;}
double  EvccJSON::getEVState(void)                  {return m_EVState;}
double  EvccJSON::getEVMaxPhases(void)              {return m_EVMaxPhases;}
double  EvccJSON::getEVPlug(void)                   {return m_EVPlug;}
double  EvccJSON::getEVSystemEnabled(void)          {return m_EVSystemEnabled;}
int     EvccJSON::getEVOutput(void)                 {return m_EVOutput;}
double  EvccJSON::getEVActualPower(void)            {return m_EVActualPower;}
double  EvccJSON::getEVSessionEnergy(void)          {return m_EVSessionEnergy;}
QString EvccJSON::getEVChargeMode(void)             {return m_EVChargingMode;}
ulong   EvccJSON::getEVTotalEnergy(void)            {return m_EVTotalEnergy*1000;}
int     EvccJSON::getEVconfiguredPhases(void)       {return m_EVconfiguredPhases;}
// int     EvccJSON::getEVallowedPhases(void)          {return m_EVallowedPhases;}
int     EvccJSON::getEVactivePhases(void)           {return m_EVChargerPhases;}
int     EvccJSON::getEVprioritySOC(void)            {return m_prioritySOC;}
int     EvccJSON::getEVbufferSOC(void)              {return m_bufferSOC;}
bool    EvccJSON::getEVbattDcControl(void)          {return m_batteryDisChargeControl;}



