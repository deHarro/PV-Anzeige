#pragma once

#include <QtGlobal>
#include <QObject>

class SmartChargerXML : public QObject {
    Q_OBJECT

public:
    SmartChargerXML();
    ~SmartChargerXML();

    void ReadSmartChargerXML(void);

    // getter functions
    double getSmartMeterActualPower(void);
    double getSmartMeterConsumption(void);
    double getSmartMeterSurplus(void);

    double getEVEvaluationPoints(void);
    double getEVState(void);
    double getMaxPhases(void);
    double getEVPlug(void);
    double getEVSystemEnabled(void);
    int getEVOutput(void);
    double getEVActualPower(void);
    double getEVSessionEnergy(void);
    QString getEVChargeMode(void);
    ulong getEVTotalEnergy(void);

    double getStorageSystemSOC(void);
    double getStorageSystemTemperature(void);
    double getStorageSystemActualPower(void);

private:
    double m_SmartMeterActualPower = 0.0;
    double m_SmartMeterConsumption = 0.0;
    double m_SmartMeterSurplus = 0.0;

    QString m_EVChargingMode;
    int m_EVMaxPhases = 0;
    double m_EVEvaluationPoints = 0.0;
    int m_EVState = 0;
    int m_EVPlug = 0;
    int m_EVSystemEnabled = 0;
    int m_EVOutput = 0;
    double m_EVActualPower = 0.0;
    double m_EVSessionEnergy = 0.0;
    ulong m_EVTotalEnergy = 0;

    int m_StorageSystemSOC = 0;
    double m_StorageSystemTemperature = 0.0;
    double m_StorageSystemActualPower = 0.0;

};
