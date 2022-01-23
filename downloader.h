#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

// global flag memory for error messages
extern quint8 m_messageFlag;
// Flag: Bit 0 = 1 -> EDLD antwortet nicht korrekt
// Flag: Bit 1 = 1 -> MBMD antwortet nicht korrekt
#define EDLDFlag 1
#define MBMDFlag 2

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
    QNetworkAccessManager *jsonManager;
    QNetworkAccessManager *xmlManager;
};

