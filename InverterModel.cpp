#include "InverterModel.h"

#include "Types.h"

#include <QDateTime>
#include <QMqttClient>
#include <QMqttMessage>
#include <QMqttSubscription>

#include <msgpack.h>

InverterModel::InverterModel(const InverterConfig& config)
    : m_config(config),
      m_client(new QMqttClient(this))
{
    connect(m_client, &QMqttClient::connected, this, &InverterModel::onConnected);
    connect(m_client, &QMqttClient::disconnected, this, &InverterModel::onDisconnected);
    connect(m_client, &QMqttClient::stateChanged, this, &InverterModel::onStateChanged);
    connect(m_client, &QMqttClient::errorChanged, this, &InverterModel::onErrorChanged);
    m_client->setHostname(m_config.mqttHost);
    m_client->setPort(m_config.mqttPort);

    connectToHost();
}

InverterModel::~InverterModel()
{
    disconnectFromHost();
}

QString InverterModel::name() const
{
    return m_config.name;
}

QList<QObject*> InverterModel::stringLiveData()
{
    QList<QObject*> objects;
    for (auto& data : m_stringLiveData) {
        objects.push_back(data);
    }
    return objects;
}

void InverterModel::connectToHost()
{
    m_client->connectToHost();
}

void InverterModel::disconnectFromHost()
{
    m_client->disconnectFromHost();
}

void InverterModel::onConnected()
{
    QString topic = "sbfspot_" + QString::number(m_config.serial) + "/live";
    m_liveSub = m_client->subscribe(topic, 0);
    topic = "sbfspot_" + QString::number(m_config.serial) + "/today/stats";
    m_statsSub = m_client->subscribe(topic, 1);

    connect(m_liveSub, &QMqttSubscription::messageReceived, this, &InverterModel::onLiveDataReceived);
    connect(m_statsSub, &QMqttSubscription::messageReceived, this, &InverterModel::onStatsDataReceived);

    for (const auto& config : m_config.strings) {
        auto data = new StringData;
        data->name = config.name;
        data->powerPeak = config.powerPeak;
        m_stringLiveData.push_back(data);
    }

    m_updateTimer.callOnTimeout(this, &InverterModel::liveDataChanged);
    m_updateTimer.start(30*1000);
}

void InverterModel::onDisconnected()
{
    qDebug() << "onDisconnected";
    m_liveSub->unsubscribe();
    m_statsSub->unsubscribe();
    m_updateTimer.stop();

    for (auto& data : m_stringLiveData) {
        delete data;
    }
    m_stringLiveData.clear();
    emit liveDataChanged();
}

void InverterModel::onStateChanged(QMqttClient::ClientState state)
{
    qDebug() << "onState:" << state;
}

void InverterModel::onErrorChanged(QMqttClient::ClientError error)
{
    qDebug() << "onError:" << error;
}

void InverterModel::onLiveDataReceived(const QMqttMessage& message)
{
    auto topic = message.topic();
    qDebug() << topic;

    // Inverter live data
    auto variant = MsgPack::unpack(message.payload()).toMap();
    m_lastUpdate = variant.value(toIntString(InverterProperty::Timestamp)).toDateTime();
    m_yieldTotal = variant.value(toIntString(InverterProperty::YieldTotal)).toDouble();
    m_yieldToday = variant.value(toIntString(InverterProperty::YieldToday)).toDouble();
    m_powerAcNow = variant.value(toIntString(InverterProperty::Power)).toDouble();

    // String live data
    auto strings = variant.value(toIntString(InverterProperty::Strings)).toList();
    int i = 0;
    int j = 0;
    m_powerDcTotal = 0.0;
    for (; (i < strings.size()) && (j < m_stringLiveData.size()); ++i, ++j) {
        m_stringLiveData[i]->power = strings.value(i).toMap().value(toIntString(InverterProperty::StringPower)).toReal();
        m_powerDcTotal += m_stringLiveData[i]->power;
    }

    emit liveDataChanged();
}

void InverterModel::onStatsDataReceived(const QMqttMessage& message)
{
    auto topic = message.topic();
    qDebug() << topic << ":" << message.payload().size();

    // Inverter stats data
    auto variant = MsgPack::unpack(message.payload()).toMap();

    // String stats data
    auto strings = variant.value(toIntString(InverterProperty::Strings)).toList();
    int i = 0;
    int j = 0;
    for (; (i < strings.size()) && (j < m_stringLiveData.size()); ++i, ++j) {
        m_stringLiveData[i]->powerPeakToday = strings.value(i).toMap().value(toIntString(InverterProperty::StringPowerMaxToday)).toReal();
    }

    emit liveDataChanged();
}

QString InverterModel::lastUpdate()
{
    QString out;

    QDateTime now = QDateTime::currentDateTime();

    if (m_lastUpdate.secsTo(now) < 90) {
        out = "online";
    } else if (m_lastUpdate.secsTo(now) < 3570) {
        out = "last seen " + QString::number((m_lastUpdate.secsTo(now) + 30)/60) + " min ago";
    } else if (m_lastUpdate.daysTo(now) == 0) {
        out = "last seen today at " + m_lastUpdate.toString("hh:mm");
    } else if (m_lastUpdate.daysTo(now) == 1) {
        out = "last seen yesterday at " + m_lastUpdate.toString("hh:mm");
    } else {
        out = "last seen " + m_lastUpdate.toString("d.M.yy") + " at " + m_lastUpdate.toString("hh:mm");
    }

    return out;
}
