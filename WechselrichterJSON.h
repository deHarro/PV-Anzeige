#pragma once

#include <QtGlobal>
#include <QObject>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

// global flag memory for error messages
extern quint8 m_messageFlag;

class WechselrichterJSON : public QObject {
    Q_OBJECT

public:
    WechselrichterJSON();
    ~WechselrichterJSON();

    void ReadWechselrichterJSON(void);

    // getter functions
    double getPVDachActualPower(void);
    double getPVGaubeActualPower(void);
    double getPVGarageActualPower(void);
    double getPVGesamtErtrag(void);

private:
    double m_PVDachActualPower = 0.0;
    double m_PVGaubeActualPower = 0.0;
    double m_PVGarageActualPower = 0.0;
    double m_PVGesamtErtrag = 0.0;

};

