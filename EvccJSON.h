#pragma once

#include <qjsonarray.h>

// WR DachN in Verabrbeitung aufnehmen (raus nehmen durch auskommentieren)
#define WRDACHN

// global flag memory for error messages
extern quint8 m_messageFlag;

class EvccJSON : public QObject {
    Q_OBJECT

public:
    EvccJSON();
    ~EvccJSON();

    void ReadEvccJSON(void);

    // getter functions
    double getPVDachNActualPower  (void);
    double getPVDachSActualPower  (void);
    double getPVGaubeActualPower  (void);
    double getPVGarageActualPower (void);
    double getPVDachNErtrag       (void);
    double getPVDachSErtrag       (void);
    double getPVGaubeErtrag       (void);
    double getPVGarageErtrag      (void);
    double getPVGesamtErtrag      (void);

    // nachfolgende Werte kamen vom SmartCharger (EDLD)
    double getSmartMeterActualPower (void);
    double getSmartMeterConsumption (void);
    double getSmartMeterSurplus     (void);

    QString EVChargingMode          (void);
    int     EVMaxPhases             (void);
    int     EVSystemEnabled         (void);
    int     EVOutput                (void);
    double  EVActualPower           (void);
    double  EVSessionEnergy         (void);
    ulong   EVTotalEnergy           (void);
    int     EVChargerPhases         (void);
    int     getStorageSystemSOC        (void);
    double  getStorageSystemTemperature(void);
    double  getStorageSystemActualPower(void);

    // nachfolgende Werte kamen vom SmartChargerXML (EDLD)
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
    int     getEVconfiguredPhases();
    int     getEVallowedPhases();
    int     getEVactivePhases();

    // nachfolgende Werte kamen vom PowerNodeModel
//    void doSetChargerPhases(int);             // set Charger Phases to 0(automatic), 1, 3


private:
    double m_PVDachNActualPower = 0.0;              // Momentanwert Dach Nord
    double m_PVDachSActualPower = 0.0;              // Momentanwert Dach Süd
    double m_PVGaubeActualPower = 0.0;              // Momentanwert Gaube
    double m_PVGarageActualPower = 0.0;             // Momentanwert Garage
    double m_PVDachNErtrag = 0.0;                   // Gesamtertrag Dach Nord
    double m_PVDachSErtrag = 0.0;                   // Gesamtertrag Dach Süd
    double m_PVGaubeErtrag = 0.0;                   // Gesamtertrag Gaube
    double m_PVGarageErtrag = 0.0;                  // Gesamtertrag Garage
    double m_PVGesamtErtrag = 0.0;                  // Gesamtertrag Anlage insgesamt
    double m_generatorPowerTotal = 0.0;             // Momentanwert Gesamt (alle Panels)

    // nachfolgende Werte kamen vom SmartCharger (EDLD)
    double  m_SmartMeterActualPower     = 0.0;      // aktuelle Leistung ins/vom Netz
    double  m_SmartMeterConsumption     = 0.0;      // gesamte aus dem Netz gezogene Energie
    double  m_SmartMeterSurplus         = 0.0;      // gesame ins Netz eingespeiste Energie

    QString m_EVChargingMode;                       // Lademodus: Surplus, Quick, Manual, Off
    int     m_EVMaxPhases               = 0;        // Festlegung der maximal nutzbaren Anzahl Phasen aus SmartCharger Settings
    int     m_EVSystemEnabled           = 0;        // Wallbox ist aktiviert, kann laden
    int     m_EVOutput                  = 0;        // Ausgang für Phasenumschaltrelais (0 -> 1 Phase, 1 -> 1 -> 3 Phasen)
    double  m_EVActualPower             = 0.0;      // aktuell ans EV abgegebene Leistung
    double  m_EVSessionEnergy           = 0.0;      // zuletzt ins EV geladene Energie
    ulong   m_EVTotalEnergy             = 0;        // gesamte jemals in ein EV eingeladene Energie
    int     m_EVChargerPhases;                      // Rückmeldung der aktiven Anzahl Phasen beim Laden: 1 oder 3
    double  m_EVEvaluationPoints = 0.0;
    int     m_EVState = 0;
    int     m_EVPlug = 0;
    int     m_EVconfiguredPhases;
    int     m_EVallowedPhases;                      // Vorgabe an EVCC
    int     m_StorageSystemSOC          = 0;        // aktuelle prozentuale Ladung des Hausakku
    double  m_StorageSystemTemperature  = 0.0;      // aktuelle Temperatur des Hausakku/des Wechselrichters (?)
    double  m_StorageSystemActualPower  = 0.0;      // aktuell vom Akku abgegebene/aufgenommene Leistung
    bool    m_configuredPhases0;
    bool    m_configuredPhases1;
    bool    m_configuredPhases3;

    // nachfolgender Wert wurde in PowerNodeModel.cpp berechnet
    int m_totalPowerConsumption         = 0;
};

