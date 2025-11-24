TEMPLATE += app

QT += quick
QT += xml
QT += widgets

CONFIG += c++14
CONFIG += NO_UNIT_TESTS
CONFIG += QTQUICKCOMPILER

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = PV-Anzeige

HEADERS += \
    Downloader.h \
    EvccJSON.h \
    PowerNodeModel.h \
    SmartChargerXML.h \
    WechselrichterJSON.h

SOURCES += \
    Downloader.cpp \
    EvccJSON.cpp \
    PowerNodeModel.cpp \
    SmartChargerXML.cpp \
    WechselrichterJSON.cpp \
    main.cpp

RESOURCES += qml.qrc


# App Icon
RC_FILE = PV-Anzeige.rc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS +=

