//#include <QFile>
#include <iostream>
//#include <QGuiApplication>
//#include <QDir>
//#include <QIODevice>

#include "Downloader.h"
//#include "PowerNodeModel.h"

// globally defined in main.cpp
extern QString m_setChargeModeString;
extern QString m_JSONfiledata;
extern int m_ManualSetCurrent;
extern int m_ChargerPhases;
extern QByteArray m_XMLfiledata;
//extern int m_EVPercent;


/* alle möglichen Remote-Befehle für SmartCharger
*
* – Parameter: „mode“ mit den Werten: „quick“, „manual“, „surplus“ und „off“
* – Parameter: „carmincharge“ mit den Werten: 6000, 7020, 7980, 9000, 10020, 10980, 12000, 13020, 13980, 15000, 16020 (mA)
* – Parameter: „carmaxphases“ mit den Werten: 1, 2 oder 3 (maximale Ladephasen im Auto)
* – Parameter: „manualcurrent“ mit den Werten: 6000, 7020, 7980, 9000, 10020, 10980, 12000, 13020, 13980, 15000, 16020,
*             16980, 18000, 19020, 19980, 21000, 22020, 22980, 24000, 25020, 25980, 27000, 28020, 28980, 30000, 31020, 31980 (mA)
* – Parameter: „surplusactivate“ mit den Werten: 0 bis 22000 (Watt an Überschuss)
* – Parameter: „action=restart“ startet die Software neu und lädt die aktuellen Einstellungen aus der Konfiguration
*
* Beispiele:
* http://192.168.xx.xx:18001/remote?mode=surplus&surplusactivate=1380
* http://192.168.xx.xx:18001/remote?mode=quick
* http://192.168.xx.xx:18001/remote?mode=off
* http://192.168.xx.xx:18001/remote?action=restart
* http://192.168.xx.xx:18001/remote?mode=manual&manualcurrent=12000
* http://192.168.xx.xx:18001/remote?mode=surplus&carmincharge=6000&carmaxphases=1&surplusactivate=680
*
* Achtung! Der Prozentsatz der EV-ChargeRate kann NICHT per remote geändert werden.
*/

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
    getRPiParameter();
}

// set charging mode of SmartCharger ----------------------------------------
// possible Charge Modes
// http://192.168.xx.xx:18001/remote?mode=off
// http://192.168.xx.xx:18001/remote?mode=quick
// http://192.168.xx.xx:18001/remote?mode=surplus
// http://192.168.xx.xx:18001/remote?mode=manual

void Downloader::doSetChargeMode(void)
{
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

// set manual current of SmartCharger ----------------------------------------
// moegliche Ladeströme: 6000, 7020, 7980, 9000, 10020, 10980, 12000, 13020, 13980, 15000, 16020, 16980, 18000, 19020, 19980,
//                       21000, 22020, 22980, 24000, 25020, 25980, 27000, 28020, 28980, 30000, 31020, 31980 (mA)
// http://192.168.xx.xx:18001/remote?manualcurrent=6000
// http://192.168.xx.xx:18001/remote?manualcurrent=12000
// http://192.168.xx.xx:18001/remote?manualcurrent=18000
// http://192.168.xx.xx:18001/remote?manualcurrent=31980 (für 32000)

void Downloader::doSetManualCurrent(void)
{
    manualCurrentTmp = QString::number(m_ManualSetCurrent);

    xmlManager = new QNetworkAccessManager(this);

    connect(xmlManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinishedSetMode(QNetworkReply*)));

    QUrl SmartChargerAddr = "http://" + m_smartChargerIP + ":" + m_smartChargerPort + "/remote?manualcurrent=" + manualCurrentTmp;
    xmlManager->get(QNetworkRequest(SmartChargerAddr));
}

void Downloader::replyFinishedSetManualCurrent (QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR with SmartCharger";
        qDebug() << reply->errorString();
        m_messageFlag |= SETCURRENTFlag;                 // Fehler bei der Verarbeitung des SetMode Befehls
    }

    reply->deleteLater();
    xmlManager->deleteLater();
    xmlManager = nullptr;
}

void Downloader::doSetChargerPhases(void)
{
    xmlManager = new QNetworkAccessManager(this);

    connect(xmlManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinishedSetMode(QNetworkReply*)));

    QUrl SmartChargerAddr = "http://" + m_smartChargerIP + ":" + m_smartChargerPort + "/remote?carmaxphases=" + QString::number(m_ChargerPhases);
    xmlManager->get(QNetworkRequest(SmartChargerAddr));
}

void Downloader::replyFinishedSetChargerPhases (QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR with SmartCharger";
        qDebug() << reply->errorString();
        m_messageFlag |= SETCURRENTFlag;                  // Fehler bei der Verarbeitung des SetMode Befehls
    }

    reply->deleteLater();
    xmlManager->deleteLater();
    xmlManager = nullptr;
}

/*
void Downloader::doSetEVPercent(void)
{
    manualEVPercentTmp = QString::number(m_EVPercent);

    xmlManager = new QNetworkAccessManager(this);

    connect(xmlManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinishedSetMode(QNetworkReply*)));

    QUrl SmartChargerAddr = "http://" + m_smartChargerIP + ":" + m_smartChargerPort + "/remote?manualcurrent=" + manualEVPercentTmp;
    xmlManager->get(QNetworkRequest(SmartChargerAddr));
}

void Downloader::replyFinishedSetEVPercent (QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR with SmartCharger";
        qDebug() << reply->errorString();
        m_messageFlag |= SETCURRENTFlag;                           // Fehler bei der Verarbeitung des SetMode Befehls
    }

    reply->deleteLater();
    xmlManager->deleteLater();
    xmlManager = nullptr;
}
*/

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
        m_JSONfiledata.clear();
        m_JSONfiledata.append(reply->readAll());
        m_messageFlag &= !MBMDFlag;                             // MBMD Daemon ist ok -> ausblenden
    }

    reply->deleteLater();

    jsonManager->deleteLater();                                 // delete manager -> prevent handle leaking
    jsonManager = nullptr;                                      // invalidate manager
}

// get access parameter to reach the Raspberry Pi
void Downloader::getRPiParameter()
{
    // open PVconfig.ini
    QDir dir("./");
    // to use/start from the current directory, we have to apply "./" as absoluteFilePath...
    QString filepath = dir.absoluteFilePath("./");              // path of PV-Anzeige.exe _at runtime_ (_not_ in Qt Creator!!)
    QFile file;

    // ... but to remove the dot in the middle of the resulting path, this "./" has to be removed again, before adding "debug/release" below
    // this is not absolutely neccessary but it's cleaner ;)
    filepath.chop(2);                                           // get rid of "./" in the middle of the path

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
