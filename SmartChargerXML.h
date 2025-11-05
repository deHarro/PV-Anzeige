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
    double  m_SmartMeterActualPower = 0.0;
    double  m_SmartMeterConsumption = 0.0;
    double  m_SmartMeterSurplus = 0.0;

    QString m_EVChargingMode;                       // Lademodus: Surplus, Quick, Manual, Off
    int     m_EVMaxPhases = 0;                      // Festlegung der maximal nutzbaren Anzahl Phasen aus SmartCharger Settings
    double  m_EVEvaluationPoints = 0.0;
    int     m_EVState = 0;
    int     m_EVPlug = 0;
    int     m_EVSystemEnabled = 0;
    int     m_EVOutput = 0;                         // Ausgang für Phasenumschaltrelais (0 -> 1 Phase, 1 -> 1 -> 3 Phasen)
    double  m_EVActualPower = 0.0;
    double  m_EVSessionEnergy = 0.0;
    ulong   m_EVTotalEnergy = 0;
    int     m_EVChargerPhases;                        // Festlegung der Anzahl Phasen beim Laden: 1 oder 3

    int     m_StorageSystemSOC = 0;                 // prozentuale Ladung des Hausakku
    double  m_StorageSystemTemperature = 0.0;
    double  m_StorageSystemActualPower = 0.0;

};
