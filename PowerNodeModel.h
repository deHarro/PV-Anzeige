#pragma once

#include <QObject>
#include <QTimer>

class PowerNodeModel : public QObject {
    Q_OBJECT

public:
    PowerNodeModel();
    ~PowerNodeModel();

    // generator properties
    Q_PROPERTY(double generatorPowerTotal MEMBER m_generatorPowerTotal NOTIFY generatorDataChanged)
    // battery properties
    Q_PROPERTY(double batteryPower MEMBER m_batteryPower NOTIFY batteryDataChanged)
    Q_PROPERTY(double batteryPercentage MEMBER m_batteryPercentage NOTIFY batteryDataChanged)
    // consumption properties
    Q_PROPERTY(double consumptionPower MEMBER m_totalPowerConsumption NOTIFY consumptionDataChanged)
    Q_PROPERTY(double consumptionEnergy MEMBER m_totalEnergyConsumption NOTIFY consumptionDataChanged)
    // grid properties
    Q_PROPERTY(double gridPower MEMBER m_gridPower NOTIFY gridDataChanged)
    Q_PROPERTY(double gridEnergyImport MEMBER m_gridEnergyImport NOTIFY gridDataChanged)
    Q_PROPERTY(double gridEnergyExport MEMBER m_gridEnergyExport NOTIFY gridDataChanged)
    // charging properties
    Q_PROPERTY(double chargingPower MEMBER m_chargingPower NOTIFY chargingDataChanged)

Q_SIGNALS:
    void generatorDataChanged();
    void batteryDataChanged();
    void consumptionDataChanged();
    void gridDataChanged();
    void chargingDataChanged();

private:
    void onConnected();
    void onDisconnected();

    double m_generatorPowerTotal = 0.0;
    double m_batteryPower = 0.0;
    double m_batteryPercentage = 0.0;
    double m_totalPowerConsumption = 0.0;
    double m_totalEnergyConsumption = 0.0;
    double m_gridPower = 0.0;
    double m_gridEnergyImport = 0.0;
    double m_gridEnergyExport = 0.0;
    double m_chargingPower = 0.0;

    // Members for demo purposes
    QTimer m_dataTimer;
    void onDataTimer();
};
