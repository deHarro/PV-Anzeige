#include "downloader.h"
#include "PowerNodeModel.h"

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
}

PowerNodeModel warnTexts;

// download XML data from SmartCharger ----------------------------------------
void Downloader::doDownloadXML(void)
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinishedXML(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("http://192.168.1.92:18001/xml")));
}

void Downloader::replyFinishedXML (QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR!";
        qDebug() << reply->errorString();
        warnTexts.setEDLDWarning(true);                         // EDL Daemon ist abgestuerzt -> Meldung
    }
    else
    {
       extern QByteArray m_XMLfiledata;    // globally defined in main.cpp

        m_XMLfiledata.clear();
        m_XMLfiledata.append(reply->readAll());
        warnTexts.setEDLDWarning(false);                        // EDL Daemon ist ok -> ausblenden
    }

    reply->deleteLater();
}

// download JSON data from MBMD PV-WR-reader ----------------------------------
void Downloader::doDownloadJSON(void)
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinishedJSON(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("http://192.168.1.92:8080/api/last")));
}

void Downloader::replyFinishedJSON(QNetworkReply *reply)
{
    PowerNodeModel warnTexts;

    if(reply->error())
    {
        qDebug() << "ERROR!";
        qDebug() << reply->errorString();
        warnTexts.setMBMDWarning(true);                         // MBMD Daemon ist abgestuerzt -> Meldung
    }
    else
    {
       extern QString m_JSONfiledata;    // globally defined in main.cpp

        m_JSONfiledata.clear();
        m_JSONfiledata.append(reply->readAll());
        warnTexts.setMBMDWarning(false);                        // MBMD Daemon ist ok -> ausblenden
    }

    reply->deleteLater();
}

