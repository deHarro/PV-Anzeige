#include "PowerNodeModel.h"

#include <chrono>

using namespace std::chrono_literals;

PowerNodeModel::PowerNodeModel() {
    connect(&m_dataTimer, &QTimer::timeout, this, &PowerNodeModel::onDataTimer);
    m_dataTimer.start(1000);
}

PowerNodeModel::~PowerNodeModel() {
}

// demo - change / increment values
void PowerNodeModel::onDataTimer() {

    m_generatorPowerTotal = rand() % 10000;
    m_generatorPowerDach += 10;
    m_generatorPowerGaube += 10;
    m_generatorPowerGarage += 10;
    emit generatorDataChanged();

    m_batteryPower = rand() % 10000;
    m_batteryPercentage = rand() % 100;
    emit batteryDataChanged();

    m_chargingPower = rand() % 10000;
    m_chargedEnergy += 10;
    m_sessionEnergy += 10;
    emit chargingDataChanged();


}
