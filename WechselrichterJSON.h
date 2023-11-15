#pragma once

#include <QtGlobal>
#include <QObject>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

// WR DachN in Verabrbeitung aufnehmen (raus nehmen durch auskommentieren)
//#define WRDACHN

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

private:
    double m_PVDachSActualPower = 0.0;
    double m_PVDachNActualPower = 0.0;
    double m_PVGaubeActualPower = 0.0;
    double m_PVGarageActualPower = 0.0;
    double m_PVGesamtErtrag = 0.0;

};

