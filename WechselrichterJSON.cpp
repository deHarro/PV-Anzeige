#include "WechselrichterJSON.h"
#include "Downloader.h"

WechselrichterJSON::WechselrichterJSON() {}

void WechselrichterJSON::ReadWechselrichterJSON() {

    extern QString m_JSONfiledata;    // globally defined in main.cpp, loaded with data by Downloader.cpp

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
            m_PVGesamtErtrag = exportGarage.toDouble();

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
            QString exportDach = m_JSONfiledata.mid(positionColon + 1, positionComma - positionColon - 1);

            qDebug() << "exportDach: " << exportDach;
            m_PVGesamtErtrag += exportDach.toDouble();

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
            m_PVGesamtErtrag += exportGaube.toDouble();

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
            m_PVGesamtErtrag += exportDachN.toDouble();

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
