#pragma once

#include <QNetworkReply>
#include <QDir>

// global flag memory for error messages
extern quint8 m_messageFlag;
// Flag: Bit 0 = 1 -> EDLD antwortet nicht korrekt
// Flag: Bit 1 = 1 -> MBMD antwortet nicht korrekt
// Flag: Bit 2 = 1 -> Version der EDLD XML Daten stimmt nicht -> muss geprüft werden!
// Flag: Bit 3 = 1 -> Einer der WR liefert keine Daten -> muss rückgesetzt werden!
// Flag: Bit 4 = 1 -> Fehler bei der Verarbeitung des SetMode Befehls im EDLD
// Flag: Bit 5 = 1 -> Fehler bei der Verarbeitung des SetManualCurrent Befehls im EDLD
// Flag: Bit 6 = 1 -> Fehler bei der Verarbeitung des SetChargerPhases Befehls im EDLD
// Flag: Bit 7 = 1 -> EVCC antwortet nicht korrekt
#define EDLDFlag 1
#define MBMDFlag 2
#define VERSIONFlag 4
#define WRFlag 8
#define SETMODEFlag 10
#define SETCURRENTFlag 20
#define SETPHASESFlag 40
#define EVCCFlag 80

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = 0);

    void doDownloadXML(void);
    void doDownloadJSON(void);

    void doDownloadEVCCJSON(void);

    void doSetChargeMode(void);
    void doSetManualCurrent(void);
    void doSetChargerPhases(void);

    QString getDataProvider(void);          // liefert den DataProvider zur Entscheidung, welcher Parser Verwendung findet
    QString getiniVersion(void);            // liefert die Version der INI-Datei zur Entscheidung, ob die INI zur Version passt

signals:

public slots:
    void replyFinishedXML (QNetworkReply *reply);
    void replyFinishedJSON (QNetworkReply *reply);

    void replyFinishedEVCCJSON (QNetworkReply *reply);

    void replyFinishedSetMode(QNetworkReply *reply);
    void replyFinishedSetManualCurrent(QNetworkReply *reply);
    void replyFinishedSetChargerPhases(QNetworkReply *reply);

private:
    void getRPiParameter(void);

    QNetworkAccessManager *jsonManager;
    QNetworkAccessManager *xmlManager;
    QNetworkAccessManager *jsonEVCCManager;

    QString m_smartChargerIP;                                   // IP des RPi mit SmartCharger (EDLD)
    QString m_smartChargerPort;                                 // Port SmartCharger (EDLD)
    QString m_mbmdPort;                                         // Port MBMD (läuft auf demselben RPi)
    QString m_EvccIP;                                           // IP des RPi mit EVCC
    QString m_EvccPort;                                         // Port EVCC
    QString m_DataProvider;                                     // Daten für PV-Anzeige kommen von "EVCC" oder "EDLD" (-> EDLD + MBMD)
    QString m_iniVersion;                                       // Version der INI-Datei. Zusätzliche Werte ab Vx.xx

    QString manualCurrentTmp;
};

