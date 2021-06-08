#pragma once

#include <QObject>
#include <QTimer>

class PowerNodeModel : public QObject {
    Q_OBJECT

public:
    PowerNodeModel();
    ~PowerNodeModel();

    // generator properties - all generator values are updated in one call to "generatorDataChanged"
    Q_PROPERTY(double generatorPowerTotal MEMBER m_generatorPowerTotal NOTIFY generatorDataChanged)
    Q_PROPERTY(double generatorPowerDach MEMBER m_generatorPowerDach NOTIFY generatorDataChanged)
    Q_PROPERTY(double generatorPowerGaube MEMBER m_generatorPowerGaube NOTIFY generatorDataChanged)
    Q_PROPERTY(double generatorPowerGarage MEMBER m_generatorPowerGarage NOTIFY generatorDataChanged)

    // battery properties - all battery values are updated in one call to "batteryDataChanged"
    Q_PROPERTY(double batteryPower MEMBER m_batteryPower NOTIFY batteryDataChanged)
    Q_PROPERTY(double batteryPercentage MEMBER m_batteryPercentage NOTIFY batteryDataChanged)

    // home consumption properties - all home consumption values are updated in one call to "consumptionDataChanged"
    Q_PROPERTY(double consumptionPower MEMBER m_totalPowerConsumption NOTIFY consumptionDataChanged)
    Q_PROPERTY(double consumptionEnergy MEMBER m_totalEnergyConsumption NOTIFY consumptionDataChanged)

    // grid properties - all grid values are updated in one call to "gridDataChanged"
    Q_PROPERTY(double gridPower MEMBER m_gridPower NOTIFY gridDataChanged)
    Q_PROPERTY(double gridEnergyImport MEMBER m_gridEnergyImport NOTIFY gridDataChanged)                    // Zähler [kWh]
    Q_PROPERTY(double gridEnergyExport MEMBER m_gridEnergyExport NOTIFY gridDataChanged)                    // Einspeisezähler [kWh]

    // wallbox properties - all wallbox values are updated in one call to "chargingDataChanged"
    Q_PROPERTY(double chargingPower MEMBER m_chargingPower NOTIFY chargingDataChanged)                      // current power [kW]
    Q_PROPERTY(double chargedEnergy MEMBER m_chargedEnergy NOTIFY chargingDataChanged)                      // total energy [kWh]
    Q_PROPERTY(double sessionEnergy MEMBER m_sessionEnergy NOTIFY chargingDataChanged)                      // last session energy [kWh]

Q_SIGNALS:
    void generatorDataChanged();
    void batteryDataChanged();
    void consumptionDataChanged();
    void gridDataChanged();
    void chargingDataChanged();

private:
    void onConnected();
    void onDisconnected();

    // generators
    double m_generatorPowerTotal = 0.0;     // Momentanleistung gesamt [kW]
    double m_generatorPowerDach = 0.0;      // Momentanleistung String Dach
    double m_generatorPowerGaube = 0.0;     // Momentanleistung String Gaube
    double m_generatorPowerGarage = 0.0;    // Momentanleistung String Garage
    // battery
    double m_batteryPower = 0.0;            // Batterieladung/-Entladung [kW]
    double m_batteryPercentage = 0.0;       // Batterie Ladezustand [%]
    // home
    double m_totalPowerConsumption = 0.0;   // Gesamtverbrauch [kW]
    double m_totalEnergyConsumption = 0.0;  // Gesamtverbrauch aus Netz und Akku und PV - woher kommt dieser Wert?
    double m_gridPower = 0.0;               // Netzbezug/Einspeisung [kW]
    double m_gridEnergyImport = 0.0;        // Zähler [kWh]
    double m_gridEnergyExport = 0.0;        // Einspeisezähler [kWh]
    // wallbox
    double m_chargingPower = 0.0;           // current power [kW]
    double m_chargedEnergy = 0.0;           // total energy [kWh]
    double m_sessionEnergy = 0.0;           // last session energy [kWh]


    // Members for demo purposes
    QTimer m_dataTimer;
    void onDataTimer();
};
