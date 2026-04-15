#include <iostream>
#include <QCoreApplication>

#include "Downloader.h"
#include "PowerNodeModel.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "EvccJSON.h"

#include <QSettings>
#include <QFile>
#include <QDir>
#include <QDebug>

// globally defined in main.cpp
extern QString m_setChargeModeString;
extern QString m_JSONfiledata;
extern QString m_JSONevccData;
extern int m_ManualSetCurrent;
extern int m_ChargerPhases;
extern QByteArray m_XMLfiledata;

EvccJSON evcc;
extern quint8 m_messageFlag;

Downloader::Downloader(QObject *parent) :
    QObject(parent)
{
    // holt IP und Port aus der Ini-Datei
    getRPiParameter();
    jsonEVCCManager = new QNetworkAccessManager(this);

    // Die Verbindung wird auch nur EINMAL hergestellt
    connect(jsonEVCCManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinishedEVCCJSON(QNetworkReply*)));
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

// set bufferSOC - Batterieunterstütztes Fahrzeugladen ab x% im Hausakku
void Downloader::doSetBufferSoc(int input)
{
    QString parameter = "buffersoc/" + QString::number(input);
    sendParamToEvcc(parameter);
}

// set prioritySOC - Priorisiere die Hausbatterie bis sie x% erreicht hat
void Downloader::doSetPrioritySoc(int input)
{
    QString parameter = "prioritysoc/" + QString::number(input);
    sendParamToEvcc(parameter);
}

// send parameters to evcc - umgestellt auf Lambda-Variante (Reaktion auf Ergebnis direkt im Aufruf formuliert)
void Downloader::sendParamToEvcc(QString param)
{
    QUrl EvccAddr = "http://" + m_EvccIP + ":" + m_EvccPort + "/api/" + param;
    QNetworkRequest request(EvccAddr);

    // Wir schicken den POST-Befehl ab
    QNetworkReply *reply = jsonEVCCManager->post(request, QByteArray()); // Leerer Body für API-Calls oft okay

    // DAS LAMBDA (Der "Post-it" Zettel)
    // [reply, param, this] bedeutet: Wir nehmen diese Infos mit auf den Zettel
    connect(reply, &QNetworkReply::finished, [reply, param, this]() {

        if (reply->error() == QNetworkReply::NoError) {
            this->m_lastStatus = "OK: " + param; // Hier wird 'this' gebraucht!

            // ERFOLG: Wir sehen im Output genau, welcher Parameter geklappt hat
            qDebug() << "EVCC-Update ERFOLGREICH für:" << param;

            // Hier könnten wir sogar direkt Flags setzen, z.B.:
            // m_messageFlag |= EVCCFlag;
        } else {
            // FEHLER:
            qDebug() << "EVCC-Update FEHLER bei:" << param << " Grund:" << reply->errorString();
        }

        // Den Speicher wieder freigeben
        reply->deleteLater();
    });
}
// \send parameters to evcc



// set charging mode of evcc ----------------------------------------
// possible Charge Modes
// http://192.168.xx.xx:7070/api/loadpoints/1/mode=off
// http://192.168.xx.xx:7070/api/loadpoints/1/mode=now
// http://192.168.xx.xx:7070/api/loadpoints/1/mode=pv
// http://192.168.xx.xx:7070/api/loadpoints/1/mode=minpv

void Downloader::doSetChargeMode(void)
{
    QString parameter = "loadpoints/1/mode/" + m_setChargeModeString;
    sendParamToEvcc(parameter);
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

    QString parameter = "loadpoints/1/maxcurrent/" + QString::number(m_ManualSetCurrent / 1000);
    sendParamToEvcc(parameter);
}
// \set manual current of SmartCharger ----------------------------------------

// set number of phases to use for EV ----------------------------------------
void Downloader::doSetChargerPhases(void)
{
    QString parameter = "loadpoints/1/phases/" + QString::number(m_ChargerPhases);
    sendParamToEvcc(parameter);
}
// \set number of phases to use for EV ----------------------------------------

// Wer liefert die Anzeigedaten
QString Downloader::getDataProvider(void)
{
    return m_DataProvider;                              // String mit EVCC
}

// Passt die INI zur Programmversion
QString Downloader::getiniVersion(void)
{
    return m_iniVersion;                                // String mit der version der INI-Datei
}

// download JSON data from EVCC ----------------------------------
void Downloader::doDownloadEVCCJSON(void)
{
    // Wir prüfen nur kurz, ob die Adresse stimmt
    QUrl EVCCapiAddr = "http://" + m_EvccIP + ":" + m_EvccPort + "/api/state";

    // Einfach abschicken. Der Manager weiß durch den connect im Konstruktor
    // schon ganz genau, wo er die Antwort abgeben muss.
    jsonEVCCManager->get(QNetworkRequest(EVCCapiAddr));
}

void Downloader::replyFinishedEVCCJSON(QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR with EVCC: " << reply->errorString();
        m_messageFlag |= EVCCFlag;
    }
    else
    {
        m_JSONevccData.clear();
        m_JSONevccData.append(reply->readAll());

        m_messageFlag &= ~EVCCFlag; // Bitweise Umkehrung zum Löschen des Flags
    }

    // WICHTIG: Speicher wieder freigeben
    reply->deleteLater();
}
// \download JSON data from EVCC ----------------------------------

// get access parameter to reach the Raspberry Pi
void Downloader::getRPiParameter()
{

QFile file;

#ifdef Q_OS_WASM                                // wird für WebAssembly (WASM) übersetzt oder für Windows?
    // Prüfen, ob die Datei im virtuellen Filesystem existiert
    if (QFile::exists("/config.ini")) {
        QSettings settings("/config.ini", QSettings::IniFormat);
        QString ip = settings.value("InverterIP", "127.0.0.1").toString();
        qDebug() << "Konfiguration geladen. IP:" << ip;
    } else {
        qWarning() << "KRITISCH: /config.ini wurde im WASM-Paket nicht gefunden!";
    }

    // Im Browser liegt die Datei im virtuellen Root,
    // weil wir sie oben als @/config.ini eingebettet haben
    file.setFileName("/config.ini");
#else
    // open PVconfig.ini
    QDir dir("./");
    // to use/start from the current directory, we have to apply "./" as absoluteFilePath...
    QString filepath = dir.absoluteFilePath("./");              // path of PV-Anzeige.exe _at runtime_ (_not_ in Qt Creator!!)

    // ... but to remove the dot in the middle of the resulting path, this "./" has to be removed again, before adding "debug/release" below
    // this is not absolutely neccessary but it's cleaner ;)
    filepath.chop(2);                                           // get rid of "./" in the middle of the path

    // catch running in QT Creator
    if (filepath.contains("-Debug"))
        file.setFileName(filepath + "PVconfig.ini");            // add filename to path
    else if (filepath.contains("-Release"))
        file.setFileName(filepath + "PVconfig.ini");            // add filename to path
    else
        // Unter Windows liegt sie einfach im gleichen Ordner wie die .exe
        file.setFileName(filepath + "PVconfig.ini");            // add filename to path
#endif

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

