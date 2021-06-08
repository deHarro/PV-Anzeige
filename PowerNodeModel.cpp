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
    emit generatorDataChanged();


    m_chargedEnergy += 10;
    m_sessionEnergy += 10;

    m_chargingPower = rand() % 10000;
    emit chargingDataChanged();
}
