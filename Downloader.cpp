#include "Downloader.h"
#include "PowerNodeModel.h"
#include "config.h"

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
}

// download XML data from SmartCharger ----------------------------------------
void Downloader::doDownloadXML(void)
{
    xmlManager = new QNetworkAccessManager(this);

    connect(xmlManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinishedXML(QNetworkReply*)));

    QUrl SmartChargerAddr = "http://" + (QString)SMARTCHARGERIP + ":" + (QString)SMARTCHARGERPORT + "/xml";

    xmlManager->get(QNetworkRequest(SmartChargerAddr));
}

void Downloader::replyFinishedXML (QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR with SmartCharger";
        qDebug() << reply->errorString();
        m_messageFlag |= EDLDFlag;                              // EDL Daemon ist abgestuerzt -> Meldung
    }
    else
    {
       extern QByteArray m_XMLfiledata;    // globally defined in main.cpp

        m_XMLfiledata.clear();
        m_XMLfiledata.append(reply->readAll());
        m_messageFlag &= !EDLDFlag;                             // EDL Daemon ist ok -> ausblenden
    }

    reply->deleteLater();

    xmlManager->deleteLater();
    xmlManager = nullptr;
}

// download JSON data from MBMD PV-WR-reader ----------------------------------
void Downloader::doDownloadJSON(void)
{
    jsonManager = new QNetworkAccessManager(this);

    connect(jsonManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinishedJSON(QNetworkReply*)));

    jsonManager->get(QNetworkRequest(QUrl("http://192.168.1.92:8080/api/last")));
}

void Downloader::replyFinishedJSON(QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR with MBMD";
        qDebug() << reply->errorString();
        m_messageFlag |= MBMDFlag;                              // MBMD Daemon ist abgestuerzt -> Meldung
    }
    else
    {
       extern QString m_JSONfiledata;    // globally defined in main.cpp

        m_JSONfiledata.clear();
        m_JSONfiledata.append(reply->readAll());
        m_messageFlag &= !MBMDFlag;                             // MBMD Daemon ist ok -> ausblenden
    }

    reply->deleteLater();

    jsonManager->deleteLater();
    jsonManager = nullptr;
}

