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
    PowerNodeModel.h

SOURCES += \
    Downloader.cpp \
    EvccJSON.cpp \
    PowerNodeModel.cpp \
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

# Wir nutzen für WebAssembly die QMAKE_LFLAGS direkt
wasm {
# Wichtig: Der Wert muss ein Vielfaches von 64 KB (65536) sein.
    QMAKE_LFLAGS += -s TOTAL_MEMORY=268435456
    # Erlaube dem Speicher zu wachsen, falls 256 MB nicht reichen
    QMAKE_LFLAGS += -s ALLOW_MEMORY_GROWTH=1

# Wir nutzen nur, was deine Version sicher versteht:
    # -g3: Erzeugt Debug-Infos
    # --profiling-funcs: Erhält die Funktionsnamen (die du im Notepad gesehen hast)
    # --no-optimize: Verhindert, dass Symbole beim Linken gelöscht werden
    QMAKE_LFLAGS += -g3 -O0 --no-optimize --profiling-funcs -s DEMANGLE_SUPPORT=1
    QMAKE_LFLAGS += -s TOTAL_MEMORY=268435456 -s ALLOW_MEMORY_GROWTH=1

    QMAKE_CXXFLAGS += -g3 -O0
}
