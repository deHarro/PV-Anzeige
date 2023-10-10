#include <QFile>
#include <iostream>
#include <QGuiApplication>
#include <QDir>
#include <QIODevice>

#include "Downloader.h"
#include "PowerNodeModel.h"

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
    getRPiParameter();
}

// set charging mode of SmartCharger ----------------------------------------
// moegliche Charge Modes
// http://192.168.xx.xx:18001/remote?mode=off
// http://192.168.xx.xx:18001/remote?mode=quick
// http://192.168.xx.xx:18001/remote?mode=surplus
// http://192.168.xx.xx:18001/remote?mode=manual

void Downloader::doSetChargeMode(void)
{
    extern QString m_setChargeModeString;                       //

    xmlManager = new QNetworkAccessManager(this);

    connect(xmlManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinishedSetMode(QNetworkReply*)));

    QUrl SmartChargerAddr = "http://" + m_smartChargerIP + ":" + m_smartChargerPort + "/remote?mode=" + m_setChargeModeString;  // PowerNodeModel.getChargeModeString();
    xmlManager->get(QNetworkRequest(SmartChargerAddr));
}

void Downloader::replyFinishedSetMode (QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR with SmartCharger";
        qDebug() << reply->errorString();
        m_messageFlag |= SETMODEFlag;                           // Fehler bei der Verarbeitung des SetMode Befehls
    }
    else
    {
//       extern QByteArray m_XMLfiledata;                         // globally defined in main.cpp

//        m_XMLfiledata.clear();
//        m_XMLfiledata.append(reply->readAll());
//        m_messageFlag &= !EDLDFlag;                             // EDL Daemon ist ok -> ausblenden
    }

    reply->deleteLater();
    xmlManager->deleteLater();
    xmlManager = nullptr;
}

// download XML data from SmartCharger ----------------------------------------
void Downloader::doDownloadXML(void)
{
    xmlManager = new QNetworkAccessManager(this);

    connect(xmlManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinishedXML(QNetworkReply*)));

    QUrl SmartChargerAddr = "http://" + m_smartChargerIP + ":" + m_smartChargerPort + "/xml";
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
       extern QByteArray m_XMLfiledata;                         // globally defined in main.cpp

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

    QUrl ModbusDaemonAddr = "http://" + m_smartChargerIP + ":" + m_mbmdPort + "/api/last";
    jsonManager->get(QNetworkRequest(ModbusDaemonAddr));
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
       extern QString m_JSONfiledata;                           // globally defined in main.cpp

        m_JSONfiledata.clear();
        m_JSONfiledata.append(reply->readAll());
        m_messageFlag &= !MBMDFlag;                             // MBMD Daemon ist ok -> ausblenden
    }

    reply->deleteLater();

    jsonManager->deleteLater();                                 // delete manager -> prevent handle leaking
    jsonManager = nullptr;                                      // invalidate manager
}

// get access parameter for Raspberry Pi
void Downloader::getRPiParameter()
{
    // open PVconfig.ini
    QDir dir("./");
    QString filepath = dir.absoluteFilePath("./");              // path of PV-Anzeige.exe _at runtime_ (_not_ in Qt Creator!!)
    QFile file;

    // catch running in QT Creator
    if (filepath.contains("-Debug"))
        file.setFileName(filepath + "debug/PVconfig.ini");      // add filename to path
    else if (filepath.contains("-Release"))
        file.setFileName(filepath + "release/PVconfig.ini");    // add filename to path
    else
        file.setFileName(filepath + "PVconfig.ini");            // add filename to path

    if(file.exists())                                           // check file access
    {
        if (!file.open(QIODevice::ReadOnly))
        {
            // Error while loading file
            std::cerr << "Error while loading PVconfig.ini" << std::endl;
        }
        else
        {
            while (!file.atEnd()) {
                QString line = file.readLine();
                if (line.contains("[SMARTCHARGERIP]"))
                    m_smartChargerIP = QString(QString(file.readLine()).remove(QChar('\r'))).remove(QChar('\n'));
                if (line.contains("[SMARTCHARGERPORT]"))
                    m_smartChargerPort = QString(QString(file.readLine()).remove(QChar('\r'))).remove(QChar('\n'));
                if (line.contains("[MBMDPORT]"))
                    m_mbmdPort = QString(QString(file.readLine()).remove(QChar('\r'))).remove(QChar('\n'));
            }
        }
        file.close();
    }
    else
    {
        // Error while loading file
        std::cerr << "Error: PVconfig.ini doesn't exist (at the given path)" << std::endl;
    }
}
