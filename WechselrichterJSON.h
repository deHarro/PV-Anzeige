#pragma once

#include <QtGlobal>
#include <QObject>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

// WR DachN in Verabrbeitung aufnehmen (raus nehmen durch auskommentieren)
#define WRDACHN

// global flag memory for error messages
extern quint8 m_messageFlag;

class WechselrichterJSON : public QObject {
    Q_OBJECT

public:
    WechselrichterJSON();
    ~WechselrichterJSON();

    void ReadWechselrichterJSON(void);

    // getter functions
    double getPVDachSActualPower(void);
    double getPVDachNActualPower(void);
    double getPVGaubeActualPower(void);
    double getPVGarageActualPower(void);
    double getPVGesamtErtrag(void);
    double getPVDachNErtrag(void);
    double getPVDachSErtrag(void);
    double getPVGaubeErtrag(void);
    double getPVGarageErtrag(void);

private:
    double m_PVDachSActualPower = 0.0;      // Momentanwert Dach Süd
    double m_PVDachNActualPower = 0.0;      // Momentanwert Dach Nord
    double m_PVGaubeActualPower = 0.0;      // Momentanwert Gaube
    double m_PVGarageActualPower = 0.0;     // Momentanwert Garage
    double m_PVDachSErtrag = 0.0;           // Gesamtertrag Dach Süd
    double m_PVDachNErtrag = 0.0;           // Gesamtertrag Dach Nord
    double m_PVGaubeErtrag = 0.0;           // Gesamtertrag Gaube
    double m_PVGarageErtrag = 0.0;          // Gesamtertrag Garage
    double m_PVGesamtErtrag = 0.0;          // Gesamtertrag Anlage insgesamt

};

