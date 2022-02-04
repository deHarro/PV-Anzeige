#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDir>
#include <QCoreApplication>

// global flag memory for error messages
extern quint8 m_messageFlag;
// Flag: Bit 0 = 1 -> EDLD antwortet nicht korrekt
// Flag: Bit 1 = 1 -> MBMD antwortet nicht korrekt
// Flag: Bit 2 = 1 -> Version der EDLD XML Daten stimmt nicht -> muss geprüft werden!
#define EDLDFlag 1
#define MBMDFlag 2
#define VERSIONFlag 4

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = 0);

    void doDownloadXML(void);
    void doDownloadJSON(void);

signals:

public slots:
    void replyFinishedXML (QNetworkReply *reply);
    void replyFinishedJSON (QNetworkReply *reply);

private:
    void getRPiParameter(void);

    QNetworkAccessManager *jsonManager;
    QNetworkAccessManager *xmlManager;
    QString m_smartChargerIP;
    QString m_smartChargerPort;
    QString m_mbmdPort;
};

