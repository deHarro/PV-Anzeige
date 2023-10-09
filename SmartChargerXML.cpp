#include <QApplication>
#include <QtXml>// Library needed for processing XML documents
#include <QFile>// Library needed for processing files

#include <iostream>
#include "SmartChargerXML.h"
#include "Downloader.h"

//#define INTERMEDIATES

// -----------------------------------------------------------
// read XML file SmartCharger
// -----------------------------------------------------------
SmartChargerXML::SmartChargerXML() {}

void SmartChargerXML::ReadSmartChargerXML() {

    extern QByteArray m_XMLfiledata;    // globally defined in main.cpp, loaded with data by Downloader.cpp

    QDomElement docElem;
    QDomDocument xmldoc;

    // _________________________________
    // ::: Read data from byte array :::

	// Set data into the QDomDocument before processing
    xmldoc.setContent(m_XMLfiledata);

    // _________________________
    // ::: Read the root tag :::
    // Extract the root markup
    QDomElement root=xmldoc.documentElement();

    QString RootName=root.tagName();            // Get root name// should be GetValues
    std::cout << "RootName  = " << RootName.toStdString().c_str() << std::endl;    // Display root data

    docElem=root.firstChild().toElement();		// root Element einlesen
    QDomNode node = root.firstChild();			// erstes Child

    QDomNode nodeRestart = node;                // Merker für zweite Runde (EV)

    // oberste Ebene unterhalb GetValues
    // Search node SettingsVersion		-----------------------------------------------------------------
    while ((node.nodeName().compare("SettingsVersion") != 0) && (!node.isNull()))	// überspringe alle Nodes bis SmartMeter
    {
#ifdef INTERMEDIATES
        std::cout << "   NodeNameLoop = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
        node=node.nextSibling();
    }
        // erste Ebene innerhalb GetValues
#ifdef INTERMEDIATES
        std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
    if (node.nodeName().compare("SettingsVersion")==0)
    {
        std::cout << "              SettingsVersion = " << node.firstChild().toText().data().toStdString().c_str() << std::endl;
        if ((node.firstChild().toText().data().toStdString() != "2.6.107") && (node.firstChild().toText().data().toStdString() != "1.6"))
        {
            m_messageFlag |= VERSIONFlag;
            std::cout << "              Caution!  Wrong settings version - Parser is for '1.6 or '2.6.107' and has to be checked" << std::endl;
        }
    }

    // Search node SmartMeter			-----------------------------------------------------------------
    // loopcount = 0;
    while ((node.nodeName().compare("SmartMeter") != 0) && (!node.isNull()))	// überspringe alle Nodes bis SmartMeter
    {
#ifdef INTERMEDIATES
        std::cout << "   NodeNameLoop = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
        node=node.nextSibling();
    }
        // erste Ebene innerhalb SmartMeter
#ifdef INTERMEDIATES
        std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif

        QDomNode nodeRestartTop = node;             // Merker für dritte Runde (StorageSystem)
        node = node.firstChild();

        // Search node ActualPower
        // loopcount = 0;
        while ((node.nodeName().compare("ActualPower") !=0 ) && (!node.isNull()))
        {
#ifdef INTERMEDIATES
            std::cout << "   NodeNameLoop = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
            node=node.nextSibling();
        }

#ifdef INTERMEDIATES
        std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
        if (node.nodeName().compare("ActualPower")==0)
        {
            m_SmartMeterActualPower = node.firstChild().toText().data().toDouble();
            std::cout << "              ActualPower = " << m_SmartMeterActualPower << std::endl;
        }

        // Search node MeterReadings
        // loopcount = 0;
        while ((node.nodeName().compare("MeterReadings") !=0 ) && (!node.isNull()))	// überspringe alle Nodes bis MeterReadings
        {
#ifdef INTERMEDIATES
            std::cout << "   NodeNameLoop = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
            node=node.nextSibling();
        }

            // erste Ebene innerhalb MeterReadings
#ifdef INTERMEDIATES
            std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
            node = node.firstChild();

            // Search node ActualPower
            // loopcount = 0;
            while ((node.nodeName().compare("Consumption") !=0 ) && (!node.isNull()))
            {
#ifdef INTERMEDIATES
                std::cout << "   NodeNameLoop = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
                node=node.nextSibling();
            }

#ifdef INTERMEDIATES
            std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
            if (node.nodeName().compare("Consumption")==0)
            {
                m_SmartMeterConsumption = node.firstChild().toText().data().toDouble();
                std::cout << "              Consumption = " << m_SmartMeterConsumption << std::endl;
            }

            // Read node Surplus
            node=node.nextSibling();

#ifdef INTERMEDIATES
            std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
            if (node.nodeName().compare("Surplus")==0)
            {
                m_SmartMeterSurplus = node.firstChild().toText().data().toDouble();
                std::cout << "              Surplus = " << m_SmartMeterSurplus << std::endl;
            }

    // wieder oberste Ebene, neu aufsetzen
    node = nodeRestartTop;         // neu aufsetzen

    // Search node EV					-----------------------------------------------------------------
    // loopcount = 0;
    while ((node.nodeName().compare("EV") != 0) && (!node.isNull()))	// überspringe alle Nodes bis EV
    {
#ifdef INTERMEDIATES
        std::cout << "   NodeNameLoop = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
        node=node.nextSibling();
    }
        nodeRestart = node;             // Merker für zweite Runde (Wallbox)
        // erste Ebene innerhalb EV
#ifdef INTERMEDIATES
        std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
        node = node.firstChild();

        // 		Search node Charging
        // loopcount = 0;
        while ((node.nodeName().compare("Charging") != 0) && (!node.isNull()))
        {
#ifdef INTERMEDIATES
            std::cout << "   NodeNameLoop = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
            node=node.nextSibling();
        }

        // erste Ebene innerhalb Charging
#ifdef INTERMEDIATES
        std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
        node = node.firstChild();

        // 			Search node ChargingMode
            // loopcount = 0;
            while ((node.nodeName().compare("ChargingMode") != 0) && (!node.isNull()))
            {
#ifdef INTERMEDIATES
                std::cout << "   NodeNameLoop = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
                node=node.nextSibling();
            }

#ifdef INTERMEDIATES
            std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
            if (node.nodeName().compare("ChargingMode")==0)
            {
                m_EVChargingMode = node.firstChild().toText().data(); // moegliche Werte: OFF, QUICK, SURPLUS, MANUAL
                std::cout << "              ChargingMode = " << m_EVChargingMode.toStdString().c_str() << std::endl;
            }

        // 			Search node EvaluationPoints
            // loopcount = 0;
            while ((node.nodeName().compare("EvaluationPoints") != 0) && (!node.isNull()))
            {
#ifdef INTERMEDIATES
                std::cout << "   NodeNameLoop = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
                node=node.nextSibling();
            }

#ifdef INTERMEDIATES
            std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
            if (node.nodeName().compare("EvaluationPoints")==0)
            {
                m_EVEvaluationPoints = node.firstChild().toText().data().toDouble();
                std::cout << "              EvaluationPoints = " << m_EVEvaluationPoints << std::endl;
            }

    // 		Search node Wallbox
            node = nodeRestart.firstChild();     // neu aufsetzen
            // loopcount = 0;
            while ((node.nodeName().compare("Wallbox") != 0) && (!node.isNull()))
            {
#ifdef INTERMEDIATES
                std::cout << "   NodeNameLoop = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
                node=node.nextSibling();
            }

            // erste Ebene innerhalb Wallbox

#ifdef INTERMEDIATES
            std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
            node = node.firstChild();

            // 			Search node State
/*
               "State" = Current state of the charging station
                0 : starting
                1 : not ready for charging; e.g. unplugged, X1 or "ena" not enabled, RFID not enabled, ...
                2 : ready for charging; waiting for EV charging request (S2)
                3 : charging
                4 : error
                5 : authorization rejected
*/
                // loopcount = 0;
                while ((node.nodeName().compare("State") != 0) && (!node.isNull()))
                {
#ifdef INTERMEDIATES
                    std::cout << "   NodeNameLoop = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
                    node=node.nextSibling();
                }

#ifdef INTERMEDIATES
                std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
                if (node.nodeName().compare("State")==0)
                {
                    m_EVState = node.toElement().attribute("id").toInt();
                    std::cout << "              State = " << m_EVState << std::endl;
                }

                // 			Search node Plug
/*
               "Plug" = Current condition of the loading connection
                0 unplugged
                1 plugged on charging station
                3 plugged on charging station plug locked
                5 plugged on charging station plugged on EV
                7 plugged on charging station plug locked plugged on EV

                "Enable sys" = Enable state for charging (contains Enable input, RFID, UDP,..)
*/
                node=node.nextSibling();

#ifdef INTERMEDIATES
                std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
                if (node.nodeName().compare("Plug")==0)
                {
                    m_EVPlug = node.toElement().attribute("id").toInt();           // <<<<< klappt nicht
                    std::cout << "              Plug = " << m_EVPlug << std::endl;
                }

            // 			Search node SystemEnabled
                node=node.nextSibling();

#ifdef INTERMEDIATES
                std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
                if (node.nodeName().compare("SystemEnabled")==0)
                {
                    m_EVSystemEnabled = node.firstChild().toText().data().toDouble();
                    std::cout << "              SystemEnabled = " << m_EVSystemEnabled << std::endl;
                }

    // 			Search node ActualPower
                // loopcount = 0;
                while ((node.nodeName().compare("ActualPower") != 0) && (!node.isNull()))
                {
#ifdef INTERMEDIATES
                    std::cout << "   NodeNameLoop = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
                    node=node.nextSibling();
                }

#ifdef INTERMEDIATES
                std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
                if (node.nodeName().compare("ActualPower")==0)
                {
                    m_EVActualPower = node.firstChild().toText().data().toDouble();
                    std::cout << "              ActualPower = " << m_EVActualPower << std::endl;
                }

    // 			Search node SessionEnergy
                node=node.nextSibling();

#ifdef INTERMEDIATES
                std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
                if (node.nodeName().compare("SessionEnergy")==0)
                {
                    m_EVSessionEnergy = node.firstChild().toText().data().toDouble();
                    std::cout << "              SessionEnergy = " << m_EVSessionEnergy << std::endl;
                }

    // 			Search node TotalEnergy
                // loopcount = 0;
                  while ((node.nodeName().compare("TotalEnergy") != 0) && (!node.isNull()))
                {
#ifdef INTERMEDIATES
                    std::cout << "   NodeNameLoop = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
                    node=node.nextSibling();
                }


#ifdef INTERMEDIATES
                std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
                if (node.nodeName().compare("TotalEnergy")==0)
                {
                    m_EVTotalEnergy = node.firstChild().toText().data().toULong();
                    std::cout << "              TotalEnergy = " << m_EVTotalEnergy << std::endl;
                }

    // wieder oberste Ebene, neu aufsetzen
    node = nodeRestartTop;

    // Search node StorageSystem		-----------------------------------------------------------------
    // loopcount = 0;
    while ((node.nodeName().compare("StorageSystem") != 0) && (!node.isNull()))	// überspringe alle Nodes bis StorageSystem
    {
#ifdef INTERMEDIATES
        std::cout << "   NodeNameLoop = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
        node=node.nextSibling();
    }

        nodeRestart = node;             // Merker für zweite Runde (AC)
        // erste Ebene innerhalb StorageSystem
#ifdef INTERMEDIATES
        std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
        node = node.firstChild();

        // 		Search node Battery
        // loopcount = 0;
        while ((node.nodeName().compare("Battery") != 0) && (!node.isNull()))
        {
#ifdef INTERMEDIATES
            std::cout << "   NodeNameLoop = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
            node=node.nextSibling();
        }

        // erste Ebene innerhalb Battery
#ifdef INTERMEDIATES
        std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
        node = node.firstChild();

        // 			Search node SOC
            // loopcount = 0;
            while ((node.nodeName().compare("SOC") != 0) && (!node.isNull()))
            {
#ifdef INTERMEDIATES
                std::cout << "   NodeNameLoop = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
                node=node.nextSibling();
            }

#ifdef INTERMEDIATES
            std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
            if (node.nodeName().compare("SOC")==0)
            {
                m_StorageSystemSOC = node.firstChild().toText().data().toInt();
                std::cout << "              SOC = " << m_StorageSystemSOC << std::endl;
            }

            node=node.nextSibling();

            // 			Search node Temperature
#ifdef INTERMEDIATES
            std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
            if (node.nodeName().compare("Temperature")==0)
            {
                m_StorageSystemTemperature = node.firstChild().toText().data().toDouble();
                std::cout << "              Temperature = " << m_StorageSystemTemperature << std::endl;
            }

            // wieder eine Ebene höher, neu aufsetzen
            node = nodeRestart;

            // 		Search node AC
            // erste Ebene innerhalb StorageSystem
#ifdef INTERMEDIATES
            std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
            node = node.firstChild();

            // loopcount = 0;
            while ((node.nodeName().compare("AC") != 0) && (!node.isNull()))
            {
#ifdef INTERMEDIATES
                std::cout << "   NodeNameLoop = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
                node=node.nextSibling();
            }

    // 			Search node ActualPower
            // erste Ebene innerhalb AC
#ifdef INTERMEDIATES
            std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
            node = node.firstChild();

            // 			Search node ActualPower
                // loopcount = 0;
                while ((node.nodeName().compare("ActualPower") != 0) && (!node.isNull()))
                {
#ifdef INTERMEDIATES
                    std::cout << "   NodeNameLoop = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
                    node=node.nextSibling();
                }

#ifdef INTERMEDIATES
                std::cout << "   NodeName = " << node.nodeName().toStdString().c_str() << std::endl;
#endif
                if (node.nodeName().compare("ActualPower")==0)
                {
                    m_StorageSystemActualPower = node.firstChild().toText().data().toDouble();
                    std::cout << "              ActualPower = " << m_StorageSystemActualPower << std::endl;
                }
}

SmartChargerXML::~SmartChargerXML()
{}

double SmartChargerXML::getSmartMeterActualPower(void)
{
    return m_SmartMeterActualPower;
}
double SmartChargerXML::getSmartMeterConsumption(void)
{
    return m_SmartMeterConsumption;
}
double SmartChargerXML::getSmartMeterSurplus(void)
{
    return m_SmartMeterSurplus;
}

double SmartChargerXML::getEVEvaluationPoints(void)
{
    return m_EVEvaluationPoints;
}
double SmartChargerXML::getEVState(void)
{
    return m_EVState;
}
double SmartChargerXML::getEVPlug(void)
{
    return m_EVPlug;
}
double SmartChargerXML::getEVSystemEnabled(void)
{
    return m_EVSystemEnabled;
}
double SmartChargerXML::getEVActualPower(void)
{
    return m_EVActualPower;
}
double SmartChargerXML::getEVSessionEnergy(void)
{
    return m_EVSessionEnergy;
}
ulong SmartChargerXML::getEVTotalEnergy(void)
{
    return m_EVTotalEnergy;
}

double SmartChargerXML::getStorageSystemSOC(void)
{
    return m_StorageSystemSOC;
}
double SmartChargerXML::getStorageSystemTemperature(void)
{
    return m_StorageSystemTemperature;
}
double SmartChargerXML::getStorageSystemActualPower(void)
{
    return m_StorageSystemActualPower;
}
QString SmartChargerXML::getEVChargeMode(void)
{
    return m_EVChargingMode;
}
