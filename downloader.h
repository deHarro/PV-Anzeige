#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

extern quint8 m_messageFlag;
#define EDLDFlag 1
#define MBMDFlag 2
#define VERSIONFlag 4
#define WRFlag 8
#define SETMODEFlag 16     // Korrigiert auf Zweierpotenz (0x10)
#define SETCURRENTFlag 32  // Korrigiert auf 0x20
#define SETPHASESFlag 64   // Korrigiert auf 0x40
#define EVCCFlag 128       // Korrigiert auf 0x80

class Downloader : public QObject
{
    Q_OBJECT

public:
    explicit Downloader(QObject *parent = nullptr);

    // Wir lassen die Funktionsnamen erst einmal stehen, damit die Aufrufe klappen
    void doDownloadEVCCJSON(void);

    // Die alten Methoden lassen wir als "leere Hüllen" stehen,
    // falls sie noch von Timern oder anderen Klassen aufgerufen werden
    void doDownloadXML(void);
    void doDownloadJSON(void);

    // Die Steuer-Befehle (bleiben aktiv)
    void doSetChargeMode(void);
    void doSetManualCurrent(void);
    void doSetChargerPhases(void);
    void doSetBatDcControl(bool);
    void doSetBufferSoc(int);
    void doSetPrioritySoc(int);

    // Diese beiden hier müssen vermutlich auch wieder in den public Bereich:
    QString getDataProvider(void);
    QString getiniVersion(void);

private slots:
    void replyFinishedEVCCJSON(QNetworkReply *reply);

    // Diese Zeilen müssen exakt so im Header stehen:
    // void replyFinishedSetMode(QNetworkReply *reply);
    // void replyFinishedSetManualCurrent(QNetworkReply *reply);
    // void replyFinishedSetChargerPhases(QNetworkReply *reply);

    // Und falls diese auch noch fehlen:
    // void replyFinishedXML(QNetworkReply *reply);
    // void replyFinishedJSON(QNetworkReply *reply);

private:
    void getRPiParameter(void);
    void sendParamToEvcc(QString param);

    // Hier kommen die Vermissten zurück:
    QString m_smartChargerIP;
    QString m_smartChargerPort;
    QString m_mbmdPort;
    QString m_DataProvider;
    QString m_iniVersion;

    QString m_lastStatus;

    // Benenne den Manager wieder so, wie er gestern hieß (vermutlich jsonManager oder jsonEVCCManager)
    //QNetworkAccessManager *jsonManager; // Oder der Name, den du gestern genutzt hast
    //QNetworkAccessManager *xmlManager;  // Falls MainWindow ihn noch sucht, lass ihn als Leiche drin
    QNetworkAccessManager *jsonEVCCManager;


    // Verbindungsparameter
    QString m_EvccIP;
    QString m_EvccPort;
    QString manualCurrentTmp;
};
