#include "PowerNodeModel.h"

#include <chrono>

using namespace std::chrono_literals;

PowerNodeModel::PowerNodeModel() {
    connect(&m_dataTimer, &QTimer::timeout, this, &PowerNodeModel::onDataTimer);
    m_dataTimer.start(1000ms);
}

PowerNodeModel::~PowerNodeModel() {
}

void PowerNodeModel::onDataTimer() {
    m_generatorPowerTotal = rand() % 10000;
    emit generatorDataChanged();
}
