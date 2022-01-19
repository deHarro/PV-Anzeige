#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

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
   QNetworkAccessManager *manager;
};

