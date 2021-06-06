#pragma once

//#include "Types.h"
#include "D:\Programme\Qt\5.12.11\Src\qtwebengine\src\3rdparty\chromium\third_party\glslang\src\glslang\Include\Types.h"

#include <QDateTime>
#include <QMqttClient>
#include <QTimer>

struct StringConfig;
class StringData;
class QmlMqttClient;
class QMqttMessage;
class QMqttSubscription;

class InverterModel : public QObject {
    Q_OBJECT
    //Q_PROPERTY(QMqttTopicFilter topic MEMBER m_topic NOTIFY topicChanged)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString lastUpdate READ lastUpdate NOTIFY liveDataChanged)
    Q_PROPERTY(double yieldTotal MEMBER m_yieldTotal NOTIFY liveDataChanged)
    Q_PROPERTY(double yieldToday MEMBER m_yieldToday NOTIFY liveDataChanged)
    Q_PROPERTY(QList<QObject*> stringLiveData READ stringLiveData NOTIFY liveDataChanged)
    //Q_PROPERTY(QList<QObject*> stringStatsData READ stringStatsData NOTIFY statsDataChanged)
    Q_PROPERTY(double powerAc MEMBER m_powerAcNow NOTIFY liveDataChanged)
    Q_PROPERTY(double powerDcTotal MEMBER m_powerDcTotal NOTIFY liveDataChanged)
    Q_PROPERTY(double powerAcMaxToday MEMBER m_powerAcMaxToday NOTIFY powerMaxTodayChanged)

public:
    InverterModel(const InverterConfig& config);
    ~InverterModel();

    QString name() const;
    QList<QObject*> stringLiveData();

    void connectToHost();
    void disconnectFromHost();

Q_SIGNALS:
    void messageReceived(const QString& msg);
    void liveDataChanged();
    void statsDataChanged();
    void powerMaxTodayChanged();

private:
    void onConnected();
    void onDisconnected();
    void onStateChanged(QMqttClient::ClientState state);
    void onErrorChanged(QMqttClient::ClientError error);

    void onLiveDataReceived(const QMqttMessage& qmsg);
    void onStatsDataReceived(const QMqttMessage& qmsg);

    QString lastUpdate();

    InverterConfig m_config;
    QMqttClient* m_client;

    QMqttSubscription* m_liveSub = nullptr;
    QMqttSubscription* m_statsSub = nullptr;

    QTimer  m_updateTimer;

    QDateTime m_lastUpdate;
    double m_yieldTotal = 0.0;
    double m_yieldToday = 0.0;
    QList<StringData*> m_stringLiveData;
    double m_powerAcNow = 0.0;
    double m_powerDcTotal = 0.0;
    double m_powerAcMaxToday = 0.0;
};
