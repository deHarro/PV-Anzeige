#pragma once

#include <QObject>

class PowerNodeModel : public QObject {
    Q_OBJECT

public:
    PowerNodeModel();
    ~PowerNodeModel();

    // TODO: Add generator properties
    Q_PROPERTY(double totalGeneratorPower MEMBER m_totalGeneratorPower NOTIFY generatorDataChanged)
    // TODO: Add battery properties
    // TODO: Add consumption properties
    // TODO: Add grid properties
    // TODO: Add charging properties


Q_SIGNALS:
    void generatorDataChanged();
    void batteryDataChanged();
    void consumptionDataChanged();
    void gridDataChanged();
    void chargingDataChanged();

private:
    void onConnected();
    void onDisconnected();

    double m_totalGeneratorPower = 0.0;

};
