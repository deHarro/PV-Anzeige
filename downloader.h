#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

extern quint8 m_messageFlag;

// Flag: Bit 0 = 1 -> EDLD antwortet nicht korrekt
// Flag: Bit 1 = 1 -> MBMD antwortet nicht korrekt
// Flag: Bit 2 = 1 -> Version der EDLD XML Daten stimmt nicht -> muss geprüft werden!
// Flag: Bit 3 = 1 -> Einer der WR liefert keine Daten -> muss rückgesetzt werden!
// Flag: Bit 4 = 1 -> Fehler bei der Verarbeitung des SetMode Befehls im EDLD
// Flag: Bit 5 = 1 -> Fehler bei der Verarbeitung des SetManualCurrent Befehls im EDLD
// Flag: Bit 6 = 1 -> Fehler bei der Verarbeitung des SetChargerPhases Befehls im EDLD
// Flag: Bit 7 = 1 -> EVCC antwortet nicht korrekt
#define EDLDFlag 1              // obsolet
#define MBMDFlag 2              // obsolet
#define VERSIONFlag 4           // obsolet
#define WRFlag 8                // obsolet
#define SETMODEFlag 16          // obsolet
#define SETCURRENTFlag 32       // obsolet
#define SETPHASESFlag 64        // obsolet
#define EVCCFlag 128

class Downloader : public QObject
{
    Q_OBJECT

public:
    explicit Downloader(QObject *parent = nullptr);

    void doDownloadEVCCJSON(void);

    // Die Steuer-Befehle (bleiben aktiv)
    void doSetChargeMode(void);
    void doSetManualCurrent(void);
    void doSetChargerPhases(void);
    void doSetBatDcControl(bool);
    void doSetBufferSoc(int);
    void doSetPrioritySoc(int);

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
