QT += quick svg
QT += gui
QT += network
QT += xml

CONFIG += c++14
CONFIG += QMQTT_NO_SSL
CONFIG += NO_UNIT_TESTS
DEFINES += MSGPACK_STATIC

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    Downloader.h \
    PowerNodeModel.h \
    SmartChargerXML.h \
    Types.h

SOURCES += \
    Downloader.cpp \
    PowerNodeModel.cpp \
    SmartChargerXML.cpp \
    Types.cpp \
    main.cpp

RESOURCES += qml.qrc \
    Icons.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#include(thirdparty/qmqtt/qmqtt.pri)
#include(thirdparty/qmsgpack/qmsgpack.pri)

FORMS +=
