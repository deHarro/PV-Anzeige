#pragma once

//#include <QObject>
//#include <QNetworkAccessManager>
//#include <QNetworkRequest>
#include <QNetworkReply>
//#include <QUrl>
#include <QDir>
//#include <QCoreApplication>

// global flag memory for error messages
extern quint8 m_messageFlag;
// Flag: Bit 0 = 1 -> EDLD antwortet nicht korrekt
// Flag: Bit 1 = 1 -> MBMD antwortet nicht korrekt
// Flag: Bit 2 = 1 -> Version der EDLD XML Daten stimmt nicht -> muss geprüft werden!
// Flag: Bit 3 = 1 -> Einer der WR liefert keine Daten -> muss rückgesetzt werden!
// Flag: Bit 4 = 1 -> Fehler bei der Verarbeitung des SetMode Befehls im EDLD
// Flag: Bit 5 = 1 -> Fehler bei der Verarbeitung des SetManualCurrent Befehls im EDLD
#define EDLDFlag 1
#define MBMDFlag 2
#define VERSIONFlag 4
#define WRFlag 8
#define SETMODEFlag 10
#define SETCURRENTFlag 20

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = 0);

    void doDownloadXML(void);
    void doDownloadJSON(void);

    void doSetChargeMode(void);
    void doSetManualCurrent(void);
    //void doSetEVPercent(void);

signals:

public slots:
    void replyFinishedXML (QNetworkReply *reply);
    void replyFinishedJSON (QNetworkReply *reply);

    void replyFinishedSetMode(QNetworkReply *reply);
    void replyFinishedSetManualCurrent(QNetworkReply *reply);
    //void replyFinishedSetEVPercent(QNetworkReply *reply);

private:
    void getRPiParameter(void);

    QNetworkAccessManager *jsonManager;
    QNetworkAccessManager *xmlManager;

    QString m_smartChargerIP;
    QString m_smartChargerPort;
    QString m_mbmdPort;
    QString manualCurrentTmp;
    //QString manualEVPercentTmp;
};

