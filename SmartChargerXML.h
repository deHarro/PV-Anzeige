#pragma once

#include <QObject>

class SmartChargerXML : public QObject {
    Q_OBJECT

public:
    SmartChargerXML();
    ~SmartChargerXML();

    void ReadSmartChargerXML(void);

    // getter functions
    double  getSmartMeterActualPower(void);
    double  getSmartMeterConsumption(void);
    double  getSmartMeterSurplus(void);

    double  getEVEvaluationPoints(void);
    double  getEVState(void);
    double  getEVMaxPhases(void);
    double  getEVPlug(void);
    double  getEVSystemEnabled(void);
    int     getEVOutput(void);
    double  getEVActualPower(void);
    double  getEVSessionEnergy(void);
    QString getEVChargeMode(void);
    ulong   getEVTotalEnergy(void);

    double  getStorageSystemSOC(void);
    double  getStorageSystemTemperature(void);
    double  getStorageSystemActualPower(void);

private:
    double  m_SmartMeterActualPower = 0.0;          // aktuelle Leistung ins/vom Netz
    double  m_SmartMeterConsumption = 0.0;          // gesamte aus dem Netz gezogene Energie
    double  m_SmartMeterSurplus = 0.0;              // gesame ins Netz eingespeiste Energie

    QString m_EVChargingMode;                       // Lademodus: Surplus, Quick, Manual, Off
    int     m_EVMaxPhases = 0;                      // Festlegung der maximal nutzbaren Anzahl Phasen aus SmartCharger Settings
    double  m_EVEvaluationPoints = 0.0;
    int     m_EVState = 0;
    int     m_EVPlug = 0;
    int     m_EVSystemEnabled = 0;                  // Wallbox ist aktiviert, kann laden
    int     m_EVOutput = 0;                         // Rückmeldung für Phasenumschaltrelais (0 -> 1 Phase, 1 -> 1 -> 3 Phasen)
    double  m_EVActualPower = 0.0;                  // aktuell ans EV abgegebene Leistung
    double  m_EVSessionEnergy = 0.0;                // zuletzt ins EV geladene Energie
    ulong   m_EVTotalEnergy = 0;                    // gesamte jemals in ein EV eingeladene Energie
    int     m_EVChargerPhases;                      // Rückmeldung von Wallbox, Anzahl Phasen beim Laden: 1 oder 3

    int     m_StorageSystemSOC = 0;                 // aktuelle prozentuale Ladung des Hausakku
    double  m_StorageSystemTemperature = 0.0;       // aktuelle Temperatur des Hausakku/des Wechselrichters (?)
    double  m_StorageSystemActualPower = 0.0;       // aktuell vom Akku abgegebene/aufgenommene Leistung

};
