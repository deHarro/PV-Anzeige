#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include "WechselrichterJSON.h"
#include "Downloader.h"

extern QString m_JSONfiledata;    // globally defined in main.cpp, loaded with data by Downloader.cpp

WechselrichterJSON::WechselrichterJSON() {}

// Wandelt JSON in Pfad-Keys um: "SUNSPEC1.Power"
static void flattenLocal(const QJsonValue &val, const QString &prefix, QMap<QString, QJsonValue> &map) {
    if (val.isObject()) {
        QJsonObject obj = val.toObject();
        for (auto it = obj.begin(); it != obj.end(); ++it) {
            flattenLocal(it.value(), prefix.isEmpty() ? it.key() : prefix + "." + it.key(), map);
        }
    } else {
        map.insert(prefix, val);
    }
}

void WechselrichterJSON::ReadWechselrichterJSON() {
    if (m_JSONfiledata.isEmpty()) return;

    QJsonDocument doc = QJsonDocument::fromJson(m_JSONfiledata.toUtf8());
    if (doc.isNull()) return;

    // 1. Alle Daten in die flache Map einlesen
    QMap<QString, QJsonValue> data;
    flattenLocal(doc.object(), "", data);

    m_PVGesamtErtrag = 0;

    // 2. Definition der zu suchenden Wechselrichter
    // Wir nutzen ein Struct, um den Code-Duplikat-Salat zu vermeiden
    struct WRConfig {
        QString sunSpecKey;
        QString name;
        double* actualPowerVar;
        double* ertragVar;
    };

    QVector<WRConfig> configs = {
        {"SUNSPEC1", "Garage", &m_PVGarageActualPower, &m_PVGarageErtrag},
        {"SUNSPEC2", "DachS",  &m_PVDachSActualPower,  &m_PVDachSErtrag},
        {"SUNSPEC3", "Gaube",  &m_PVGaubeActualPower,  &m_PVGaubeErtrag},
        {"SUNSPEC4", "DachN",  &m_PVDachNActualPower,  &m_PVDachNErtrag}
    };

    // 3. Die Liste abarbeiten
    for (const auto& wr : configs) {
        // Wir prüfen, ob der Key im JSON existiert
        // Pfade sind z.B. "SUNSPEC1.Export" oder "SUNSPEC1.Power"
        QString exportPath = wr.sunSpecKey + ".Export";
        QString powerPath  = wr.sunSpecKey + ".Power";

        if (data.contains(exportPath)) {
            double exportVal = data.value(exportPath).toDouble();
            double powerVal  = data.value(powerPath).toDouble();

            // Werte in die Member-Variablen schreiben
            *wr.ertragVar = exportVal;
            *wr.actualPowerVar = powerVal;

            // Summe bilden
            m_PVGesamtErtrag += exportVal;

            qDebug() << "WR" << wr.name << "gelesen: P=" << powerVal << "E=" << exportVal;
        }
        else {
            qDebug() << "Wechselrichter" << wr.name << "ist ausgefallen";
            m_messageFlag |= WRFlag;
        }
    }
}

/*
void WechselrichterJSON::ReadWechselrichterJSON() {

//    qDebug() << "m_JSONfiledata: " << m_JSONfiledata;

    // ich mach es zu Fuss...
    int position = 0;
    int positionColon = 0;
    int positionComma = 0;

    if(!m_JSONfiledata.isEmpty())
    {
// WR Garage
        if ((position = m_JSONfiledata.indexOf("SUNSPEC1")) != -1)        // "SUNSPEC1" gefunden
        {
            position = m_JSONfiledata.indexOf("\"Export", position);
            positionColon = m_JSONfiledata.indexOf(":", position);
            // if converters deliver 0 (zero), there is no "comma "," after "Export:", it's a "}"
            positionComma = std::min(m_JSONfiledata.indexOf(",", positionColon), m_JSONfiledata.indexOf("}", positionColon));
            QString exportGarage = m_JSONfiledata.mid(positionColon + 1, positionComma - positionColon - 1);

            qDebug() << "exportGarage: " << exportGarage;
            m_PVGarageErtrag = exportGarage.toDouble();
            m_PVGesamtErtrag = m_PVGarageErtrag;

            if((position = m_JSONfiledata.indexOf("\"Power", position)) != -1)
            {
                positionColon = m_JSONfiledata.indexOf(":", position);
                positionComma = m_JSONfiledata.indexOf(",", positionColon);
                QString power = m_JSONfiledata.mid(positionColon + 1, positionComma - positionColon - 1);

                double dPower = power.toDouble();
                qDebug() << "Power1: " << dPower;

                m_PVGarageActualPower = dPower;
            }
        }
        else
        {
            qDebug() << "Wechselrichter Garage ist ausgefallen";
            m_messageFlag |= WRFlag;
        }

// WR DachS
        if ((position = m_JSONfiledata.indexOf("SUNSPEC2")) != -1)        // "SUNSPEC2" gefunden
        {
            position = m_JSONfiledata.indexOf("\"Export", position);
            positionColon = m_JSONfiledata.indexOf(":", position);
            // if converters deliver 0 (zero), there is no "comma "," after "Export:", it's a "}"
            positionComma = std::min(m_JSONfiledata.indexOf(",", positionColon), m_JSONfiledata.indexOf("}", positionColon));
            QString exportDachS = m_JSONfiledata.mid(positionColon + 1, positionComma - positionColon - 1);

            qDebug() << "exportDachS: " << exportDachS;
            m_PVDachSErtrag = exportDachS.toDouble();
            m_PVGesamtErtrag += exportDachS.toDouble();

            if((position = m_JSONfiledata.indexOf("\"Power", position)) != -1)
            {
                positionColon = m_JSONfiledata.indexOf(":", position);
                positionComma = m_JSONfiledata.indexOf(",", positionColon);
                QString power = m_JSONfiledata.mid(positionColon + 1, positionComma - positionColon - 1);

                double dPower = power.toDouble();
                qDebug() << "Power2: " << dPower;

                m_PVDachSActualPower = dPower;
            }
        }
        else
        {
            qDebug() << "Wechselrichter DachS ist ausgefallen";
            m_messageFlag |= WRFlag;
        }

// WR Gaube
        if ((position = m_JSONfiledata.indexOf("SUNSPEC3")) != -1)        // "SUNSPEC3" gefunden
        {
            position = m_JSONfiledata.indexOf("\"Export", position);
            positionColon = m_JSONfiledata.indexOf(":", position);
            // if converters deliver 0 (zero), there is no "comma "," after "Export:", it's a "}"
            if (m_JSONfiledata.indexOf(",", positionColon) != -1)
            {
                positionComma = std::min(m_JSONfiledata.indexOf(",", positionColon), m_JSONfiledata.indexOf("}", positionColon));
            }
            else
            {
                positionComma = m_JSONfiledata.indexOf("}", positionColon);
            }
            QString exportGaube = m_JSONfiledata.mid(positionColon + 1, positionComma - positionColon - 1);

            qDebug() << "exportGaube: " << exportGaube;
            m_PVGaubeErtrag = exportGaube.toDouble();
            m_PVGesamtErtrag += m_PVGaubeErtrag;

            if((position = m_JSONfiledata.indexOf("\"Power", position)) != -1)
            {
                positionColon = m_JSONfiledata.indexOf(":", position);
                positionComma = m_JSONfiledata.indexOf(",", positionColon);
                QString power = m_JSONfiledata.mid(positionColon + 1, positionComma - positionColon - 1);

                double dPower = power.toDouble();
                qDebug() << "Power3: " << dPower;

                m_PVGaubeActualPower = dPower;
            }
        }
        else
        {
            qDebug() << "Wechselrichter Gaube ist ausgefallen";
            m_messageFlag |= WRFlag;
        }

#ifdef WRDACHN
// WR DachN
        if ((position = m_JSONfiledata.indexOf("SUNSPEC4")) != -1)        // "SUNSPEC4" gefunden
        {
            position = m_JSONfiledata.indexOf("\"Export", position);
            positionColon = m_JSONfiledata.indexOf(":", position);
            // if converters deliver 0 (zero), there is no "comma "," after "Export:", it's a "}"
            if (m_JSONfiledata.indexOf(",", positionColon) != -1)
            {
                positionComma = std::min(m_JSONfiledata.indexOf(",", positionColon), m_JSONfiledata.indexOf("}", positionColon));
            }
            else
            {
                positionComma = m_JSONfiledata.indexOf("}", positionColon);
            }
            QString exportDachN = m_JSONfiledata.mid(positionColon + 1, positionComma - positionColon - 1);

            qDebug() << "exportDachN: " << exportDachN;
            m_PVDachNErtrag = exportDachN.toDouble();
            m_PVGesamtErtrag += m_PVDachNErtrag;

            if((position = m_JSONfiledata.indexOf("\"Power", position)) != -1)
            {
                positionColon = m_JSONfiledata.indexOf(":", position);
                positionComma = m_JSONfiledata.indexOf(",", positionColon);
                QString power = m_JSONfiledata.mid(positionColon + 1, positionComma - positionColon - 1);

                double dPower = power.toDouble();
                qDebug() << "Power4: " << dPower;

                m_PVDachNActualPower = dPower;
            }
        }
        else
        {
            qDebug() << "Wechselrichter DachN ist ausgefallen";
            m_messageFlag |= WRFlag;
        }
#endif
        // m_messageFlag |= WRFlag;                 // Test für Debug!!
    }
}
*/

WechselrichterJSON::~WechselrichterJSON() {}

double WechselrichterJSON::getPVDachSActualPower(void)
{
    return m_PVDachSActualPower;
};

double WechselrichterJSON::getPVDachNActualPower(void)
{
    return m_PVDachNActualPower;
};

double WechselrichterJSON::getPVGaubeActualPower(void)
{
    return m_PVGaubeActualPower;
};

double WechselrichterJSON::getPVGarageActualPower(void)
{
    return m_PVGarageActualPower;
};

double WechselrichterJSON::getPVGesamtErtrag(void)
{
    return m_PVGesamtErtrag;
};

double WechselrichterJSON::getPVDachNErtrag(void)
{
    return m_PVDachNErtrag;
};

double WechselrichterJSON::getPVDachSErtrag(void)
{
    return m_PVDachSErtrag;
};

double WechselrichterJSON::getPVGaubeErtrag(void)
{
    return m_PVGaubeErtrag;
};

double WechselrichterJSON::getPVGarageErtrag(void)
{
    return m_PVGarageErtrag;
};
