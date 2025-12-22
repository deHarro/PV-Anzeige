#include <iostream>
#include <QCoreApplication>

#include "Downloader.h"
#include "PowerNodeModel.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "EvccJSON.h"


// globally defined in main.cpp
extern QString m_setChargeModeString;
extern QString m_JSONfiledata;
extern QString m_JSONevccData;
extern int m_ManualSetCurrent;
extern int m_ChargerPhases;
extern QByteArray m_XMLfiledata;

EvccJSON evcc;

//#define TRANSFERTIMEOUT QNetworkRequest::DefaultTransferTimeoutConstant            // when no reaction cancel HTTPRequest after 30000 ms
#define TRANSFERTIMEOUT 4000                        // when no reaction cancel HTTPRequest after 4 seconds


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

// 3 settings for load balance "battery <> EV" in evcc
//  batterydischargecontrol/{enable}   - {enable} == true/false
//  buffersoc/{soc}                    - {soc} == 0..100
//  prioritysoc/{soc}                  - {soc} == 0..100

// set BatteryDischargeControl - Verhindere Entladung im Schnell-Modus und bei geplantem Laden.
void Downloader::doSetBatDcControl(bool input)
{
    QString parameter = "batterydischargecontrol/" + QString(input == 0 ? "false" : "true");
    sendParamToEvcc(parameter);
}

// set bufferSOC - Batterieunterstütztes Fahrzeugladen ab x%
void Downloader::doSetBufferSoc(int input)
{
    QString parameter = "buffersoc/" + QString::number(input);
    sendParamToEvcc(parameter);
}

// set prioritySOC - Priorisiere die Hausbatterie bis x%
void Downloader::doSetPrioritySoc(int input)
{
    QString parameter = "prioritysoc/" + QString::number(input);
    sendParamToEvcc(parameter);
}

// send parameters to evcc
void Downloader::sendParamToEvcc(QString param)
{
    QUrl EvccAddr = "http://" + m_EvccIP + ":" + m_EvccPort + "/api/" + param;  // PowerNodeModel.getChargeModeString();
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    // URL für die Anfrage festlegen
    QUrl url(EvccAddr);

    // QNetworkRequest erstellen
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Accept", "application/json");

    // Leere JSON-Daten (falls nötig, ersetze dies durch tatsächliche Daten)
    QJsonObject json;
    QByteArray jsonData = QJsonDocument(json).toJson();

    // POST-Anfrage senden
    QNetworkReply *reply = manager->post(request, jsonData);

    // Optional: Verbindung zu signalen, um auf die Antwort zu reagieren
    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            // Verarbeite die Antwort hier
        } else {
            // Fehlerverarbeitung
        }
        reply->deleteLater();
    });
}
// \send parameters to evcc



// set charging mode of SmartCharger ----------------------------------------
// possible Charge Modes
// http://192.168.xx.xx:18001/remote?mode=off
// http://192.168.xx.xx:18001/remote?mode=quick
// http://192.168.xx.xx:18001/remote?mode=surplus
// http://192.168.xx.xx:18001/remote?mode=manual

void Downloader::doSetChargeMode(void)
{
    if(m_DataProvider.toUpper() == "EVCC")
    {
        QString parameter = "loadpoints/1/mode/" + m_setChargeModeString;
        sendParamToEvcc(parameter);
    }
    else
    {
        xmlManager = new QNetworkAccessManager(this);
        xmlManager->setTransferTimeout(TRANSFERTIMEOUT);

        connect(xmlManager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinishedSetMode(QNetworkReply*)));

            QUrl SmartChargerAddr = "http://" + m_smartChargerIP + ":" + m_smartChargerPort + "/remote?mode=" + m_setChargeModeString;  // PowerNodeModel.getChargeModeString();
            xmlManager->get(QNetworkRequest(SmartChargerAddr));
    }
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
    xmlManager = Q_NULLPTR ;
}

// \set charging mode of SmartCharger ----------------------------------------

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

    if(m_DataProvider.toUpper() == "EVCC")
    {
        QString parameter = "loadpoints/1/maxcurrent/" + QString::number(m_ManualSetCurrent / 1000);
        sendParamToEvcc(parameter);
    }
    else
    {
        xmlManager = new QNetworkAccessManager(this);
        xmlManager->setTransferTimeout(TRANSFERTIMEOUT);


        connect(xmlManager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinishedSetMode(QNetworkReply*)));

        QUrl SmartChargerAddr = "http://" + m_smartChargerIP + ":" + m_smartChargerPort + "/remote?manualcurrent=" + manualCurrentTmp;
        xmlManager->get(QNetworkRequest(SmartChargerAddr));
    }
}

void Downloader::replyFinishedSetManualCurrent (QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR with SmartCharger";
        qDebug() << reply->errorString();
        m_messageFlag |= SETCURRENTFlag;                 // Fehler bei der Verarbeitung des SetManualCurrent Befehls
    }

    reply->deleteLater();
    xmlManager->deleteLater();
    xmlManager = Q_NULLPTR ;
}
// \set manual current of SmartCharger ----------------------------------------

// set number of phases to use for SmartCharger ----------------------------------------
void Downloader::doSetChargerPhases(void)
{
    if(m_DataProvider.toUpper() == "EVCC")
    {
        QString parameter = "loadpoints/1/phases/" + QString::number(m_ChargerPhases);
        sendParamToEvcc(parameter);
    }
    else
    {
        xmlManager = new QNetworkAccessManager(this);
        xmlManager->setTransferTimeout(TRANSFERTIMEOUT);


        connect(xmlManager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinishedSetMode(QNetworkReply*)));

        QUrl SmartChargerAddr = "http://" + m_smartChargerIP + ":" + m_smartChargerPort + "/remote?carmaxphases=" + QString::number(m_ChargerPhases);
        xmlManager->get(QNetworkRequest(SmartChargerAddr));
    }
}

// Wer liefert die Anzeigedaten
QString Downloader::getDataProvider(void)
{
    return m_DataProvider;                              // String mit EVCC oder EDLD
}

// Passt die INI zur Programmversion
QString Downloader::getiniVersion(void)
{
    return m_iniVersion;                                // String mit der version der INI-Datei
}

void Downloader::replyFinishedSetChargerPhases (QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR with SmartCharger";
        qDebug() << reply->errorString();
        m_messageFlag |= SETPHASESFlag;                  // Fehler bei der Verarbeitung des setPhases Befehls
    }

    reply->deleteLater();
    xmlManager->deleteLater();
    xmlManager = Q_NULLPTR ;
}
// \set number of phases to use for SmartCharger ----------------------------------------

// download XML data from SmartCharger ----------------------------------------
void Downloader::doDownloadXML(void)
{
    xmlManager = new QNetworkAccessManager(this);
    xmlManager->setTransferTimeout(TRANSFERTIMEOUT);


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
        m_XMLfiledata.clear();                                  // XMLdata löschen...
        m_XMLfiledata.append(reply->readAll());                 // ...und neu einlesen
        m_messageFlag &= !EDLDFlag;                             // EDL Daemon ist ok -> Flag ausblenden
    }

    reply->deleteLater();
    xmlManager->deleteLater();
    xmlManager = Q_NULLPTR ;
}
// \download XML data from SmartCharger ----------------------------------------

// download JSON data from MBMD PV-WR-reader ----------------------------------
void Downloader::doDownloadJSON(void)
{
    jsonManager = new QNetworkAccessManager(this);
    jsonManager->setTransferTimeout(TRANSFERTIMEOUT);

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
        m_JSONfiledata.clear();                                 // JSONdata löschen...
        m_JSONfiledata.append(reply->readAll());                // ... und neu einlesen
        m_messageFlag &= !MBMDFlag;                             // MBMD Daemon ist ok -> ausblenden
    }

    reply->deleteLater();

    jsonManager->deleteLater();                                 // delete manager -> prevent handle leaking
    jsonManager = Q_NULLPTR ;                                      // invalidate manager
}
// \download JSON data from MBMD PV-WR-reader ----------------------------------

// download JSON data from EVCC ----------------------------------
void Downloader::doDownloadEVCCJSON(void)
{
    jsonEVCCManager = new QNetworkAccessManager(this);
    jsonEVCCManager->setTransferTimeout(TRANSFERTIMEOUT);

    connect(jsonEVCCManager , SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinishedEVCCJSON(QNetworkReply*)));

    QUrl EVCCapiAddr = "http://" + m_EvccIP + ":" + m_EvccPort + "/api/state";
    jsonEVCCManager->get(QNetworkRequest(EVCCapiAddr));
}

void Downloader::replyFinishedEVCCJSON(QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR with EVCC";
        qDebug() << reply->errorString();
        m_messageFlag |= EVCCFlag;                              // EVCC ist abgestuerzt -> Meldung
    }
    else
    {
        m_JSONevccData.clear();                                 // JSONdata löschen...
        m_JSONevccData.append(reply->readAll());                // ... und neu einlesen
        m_messageFlag &= !EVCCFlag;                             // EVCC ist ok -> ausblenden
    }

    reply->deleteLater();

    jsonEVCCManager->deleteLater();                             // delete manager -> prevent handle leaking
    jsonEVCCManager = Q_NULLPTR ;                               // invalidate manager
}
// \download JSON data from EVCC ----------------------------------

// get access parameter to reach the Raspberry Pi
void Downloader::getRPiParameter()
{
    // open PVconfig.ini
    QDir dir("./");
    //QDir dir = QCoreApplication::applicationDirPath();
    // to use/start from the current directory, we have to apply "./" as absoluteFilePath...
    QString filepath = dir.absoluteFilePath("./");              // path of PV-Anzeige.exe _at runtime_ (_not_ in Qt Creator!!)
    QFile file;

    // ... but to remove the dot in the middle of the resulting path, this "./" has to be removed again, before adding "debug/release" below
    // this is not absolutely neccessary but it's cleaner ;)
    filepath.chop(2);                                           // get rid of "./" in the middle of the path

    // catch running in QT Creator
    if (filepath.contains("-Debug"))
        file.setFileName(filepath + "PVconfig.ini");      // add filename to path
    else if (filepath.contains("-Release"))
        file.setFileName(filepath + "PVconfig.ini");    // add filename to path
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
                if (line.contains("[INIVERSION]"))
                {
                    QString iniVersion = (QString(file.readLine()).remove(QChar('\r'))).remove(QChar('\n'));     // Ini-Version 1.xx

                    if((iniVersion.left(1) >= VERSIONMAJOR) /*&& (iniVersion.right(2) >= VERSIONMINOR)*/)
                    {
                        QString line = file.readLine();
                        if (line.contains("[REALPICS]"))
                        {
                            m_iniVersion = iniVersion;
                        }
                    }
                    else
                    {
                        // Error wrong INI-File version
                        std::cerr << "Error wrong INI-File version" << std::endl;
                    }
                }
                if (line.contains("[SMARTCHARGERIP]"))
                    m_smartChargerIP = QString(QString(file.readLine()).remove(QChar('\r'))).remove(QChar('\n'));
                if (line.contains("[SMARTCHARGERPORT]"))
                    m_smartChargerPort = QString(QString(file.readLine()).remove(QChar('\r'))).remove(QChar('\n'));
                if (line.contains("[MBMDPORT]"))
                    m_mbmdPort = QString(QString(file.readLine()).remove(QChar('\r'))).remove(QChar('\n'));
                if (line.contains("[EVCCIP]"))
                    m_EvccIP = QString(QString(file.readLine()).remove(QChar('\r'))).remove(QChar('\n'));
                if (line.contains("[EVCCPORT]"))
                    m_EvccPort = QString(QString(file.readLine()).remove(QChar('\r'))).remove(QChar('\n'));
                if (line.contains("[DATAPROVIDER]"))
                    m_DataProvider = QString(QString(file.readLine()).remove(QChar('\r'))).remove(QChar('\n'));
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
// \get access parameter to reach the Raspberry Pi

