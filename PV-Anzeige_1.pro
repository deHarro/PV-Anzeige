QT += quick svg

CONFIG += c++14
CONFIG += QMQTT_NO_SSL
CONFIG += NO_UNIT_TESTS

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    PowerNodeModel.h

SOURCES += \
    PowerNodeModel.cpp \
    main.cpp

RESOURCES += qml.qrc \
    Icons.qrc \

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
//    Icons/Akku_weiß_transparent.png

include(thirdparty/qmqtt/qmqtt.pri)
