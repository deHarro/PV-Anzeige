#include "downloader.h"
#include "PowerNodeModel.h"

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
}

// download XML data from SmartCharger ----------------------------------------
void Downloader::doDownloadXML(void)
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinishedXML(QNetworkReply*)));

    manager->get(QNetworkRequest(QUrl("http://192.168.1.92:18001/xml")));
}

PowerNodeModel powerNodeModel;

void Downloader::replyFinishedXML (QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR with SmartCharger";
        qDebug() << reply->errorString();
        m_messageFlag |= EDLDFlag;                             // EDL Daemon ist abgestuerzt -> Meldung
//        emit powerNodeModel.setEDLDWarning();                         // EDL Daemon ist abgestuerzt -> Meldung
    }
    else
    {
       extern QByteArray m_XMLfiledata;    // globally defined in main.cpp

        m_XMLfiledata.clear();
        m_XMLfiledata.append(reply->readAll());
        m_messageFlag &= !EDLDFlag;                            // EDL Daemon ist ok -> ausblenden
//        emit powerNodeModel.setEDLDWarning();                        // EDL Daemon ist ok -> ausblenden
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
    if(reply->error())
    {
        qDebug() << "ERROR with MBMD";
        qDebug() << reply->errorString();
//        emit powerNodeModel.setMBMDWarning();                         // MBMD Daemon ist abgestuerzt -> Meldung
        m_messageFlag |= MBMDFlag;                         // MBMD Daemon ist abgestuerzt -> Meldung
    }
    else
    {
       extern QString m_JSONfiledata;    // globally defined in main.cpp

        m_JSONfiledata.clear();
        m_JSONfiledata.append(reply->readAll());
//        emit powerNodeModel.setMBMDWarning();                        // MBMD Daemon ist ok -> ausblenden
        m_messageFlag &= !MBMDFlag;                        // MBMD Daemon ist ok -> ausblenden
    }

    reply->deleteLater();
}

