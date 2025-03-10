import QtQuick 2.15
import QtQuick.Window 2.12
import QtQuick.Controls 2.15
// import QtGraphicalEffects 1.0                // use this line when working with Qt 5
import Qt5Compat.GraphicalEffects         // use this line when working with Qt 6

import PowerNodeModel 1.0


Window {
    id: window
    width: 404
    height: 560
    minimumWidth: width                      // prevent resizing
    maximumWidth: width
    minimumHeight: height
    maximumHeight: height
    opacity: 1
    visible: true
    color: PowerNodeModel.backgroundColor    // sehr helles Grau oder helles Rosa (bei Fehler) für Fensterhintergrund
    title: PowerNodeModel.windowTitle
    //    title: qsTr("PV-Anzeige")

    property alias rectangle1: rectangle1
    property alias rectangle4: rectangle4
    property alias rectangle3: rectangle3
    property alias rectangle2: rectangle2
    property alias rectangle5: rectangle5

    property real coordHouseX: 157
    property real coordHouseY: 140
    property real coordHouseW: 90
    property real coordHouseH: 270

    // kleiner grauer Punkt als Hint wo mit der Maus gezogen werden muss, damit der Drawer aufgeht
    Rectangle {
        x: 379
        y: 65
        width: 10
        height: width
        color: "#d4d4d4"
        radius: width*0.5
    }

    // Mit swipe Bewegung der Maus von rechts nach links die Ertragswerte einblenden
    Drawer {
        id: drawer1
        y: 12                               // -- das muss zu window.height passen -> auf SonnenBox zentriert
        width: 0.66 * window.width
        height: 0.205 * window.height       // -- window.height -> und der Abstand zu den Markierungen passt
        dragMargin: 0.25 * window.width
        edge: Qt.RightEdge
        interactive: true
        position: 0.5

        background: Rectangle {
            Rectangle {
                x: 1
                width: parent.width - 2
                y: 1
                height: parent.height - 2
                color: "#00ac00"            // LIMEGREEN
            }
        }
        // <pre> bewirkt, dass mehrere Leerzeichen nicht zu Einem zusammengefasst werden -> Formatierung der Zeilen
        Label {
            color: "white"
            width: parent.width - 20        // kleiner Rand um den Text herum
            height: parent.height - 20
            minimumPointSize: 10
            font.pointSize: 60
            fontSizeMode: Text.Fit          // formatfüllend anzeigen
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter

            text: "<pre><b>" +
                 "Ertragswerte der Wechselrichter" + "<br>" +
                 "Dach Nord: " + PowerNodeModel.generatorDachNEnergy  + " kWh<br>" +
                 "Dach Süd:  " + PowerNodeModel.generatorDachSEnergy  + " kWh<br>" +
                 "Gaube:     " + PowerNodeModel.generatorGaubeEnergy  + " kWh<br>" +
                 "Garage:    " + PowerNodeModel.generatorGarageEnergy + " kWh<br>" +
                 "Gesamt:     " + PowerNodeModel.genTotalEnergy + " MWh" +
                 "</b></pre>";
        }
    }

    // Überlagerte Farben in der Haus-Box mit Texten und Bild
    Item {
        id: root
        x: coordHouseX
        y: coordHouseY
        width: coordHouseW
        height: coordHouseH

        Rectangle {
            id: outerBackground
            anchors.fill: parent
            radius: 15
            color: "Whitesmoke"

            Rectangle {
                id: innerFillArea
                anchors.fill: parent
                anchors.margins: 0
                radius:  15
                color: "Yellow"           // wird nie angezeigt
                Item {
                    id: innerMaskSource
                    anchors.fill: parent
                    layer.enabled: true
                }
            }
        }

        Rectangle {
            objectName: "House"
            id: rectangle3
            x: 0
            y: 0
            width: 90
            height: 270
            color: PowerNodeModel.homeColor
            //color: "#b3b3b3"                  // hellgrau, keine QML Basic/SVG color
            //color: "limegreen"                // Hellgrün
            radius: 15
            border.width: 0
            opacity: 1
            clip: false

        }

        Item {
            id: indicatorContainer
            anchors.fill: parent
            anchors.margins: 0

            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: innerFillArea
            }

            Item {
                anchors.fill: parent

                Rectangle {
                    id: rectangleGreen
                    anchors.left: parent.left
                    //                    anchors.top: parent.top
                    anchors.margins: 0
                    height: PowerNodeModel.homeTopGreenH
                    x: coordHouseX              // 157
                    y: 270 - PowerNodeModel.homeBotRedH - PowerNodeModel.homeTopGreenH
                    //                    y: coordHouseY              // 140
                    width: coordHouseW          //  90
                    visible: true
                    color: "FORESTGREEN"
                    opacity: 1
                    radius: 0
                    border.width: 0
                    clip: true
                }


                Rectangle {
                    id: rectangleRed
                    anchors.left: parent.left
                    y:  270 - PowerNodeModel.homeBotRedH
                    width: 90
                    height: PowerNodeModel.homeBotRedH
                    anchors.margins: 0
                    visible: true
                    color: "FIREBRICK"
                    opacity: 1
                    radius: 0
                    clip: true
                }

                Item {
                    id: textimage
                    x: 0
                    y: 0
                    Text {
                        id: text3
                        x: 1
                        y: 6
                        width: 88
                        height: 33
                        color: "#ffffff"
                        text: qsTr("Gesamt-verbrauch")
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                        font.bold: true
                    }

                    Image {
                        id: image1
                        x: 2
                        y: 99
                        width: 86
                        height: 83
                        source: "Icons/Haus2_weiss_transparent.png"
                        fillMode: Image.PreserveAspectFit
                    }

                    Text {
                        id: text36
                        x: 29
                        y: 198
                        color: "#ffffff"   // "#f9bbcbc"
                        text: qsTr("Zähler")
                        font.pixelSize: 12
                    }

                    Text {
                        id: text9
                        x: 36
                        y: 78
                        color: "#ffffff"
                        text: qsTr("kW")
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        minimumPixelSize: 14
                        font.bold: true
                    }

                    Text {
                        id: text13
                        x: 7
                        y: 46
                        width: 77
                        height: 26
                        // consumptionPower hängt an EDLD _und_ MBMD, 2022-05-26
                        color: PowerNodeModel.consumptionPowerfigures
                        //                        color: "#ffffff"
//                        text: (PowerNodeModel.consumptionPower / 1000).toFixed(2)
                        text: PowerNodeModel.consumptionPower
                        font.pixelSize: 23
                        horizontalAlignment: Text.AlignHCenter
                        font.family: "Arial"
                    }

                    Text {
                        id: text37
                        x: 8
                        y: 214
                        width: 74
                        height: 14
                        color: PowerNodeModel.EDLDfigures
                        //                        color: "#ffffff"   // "#f9bbcbc"
                        // eine Nachkommastelle, zentriert
                        text: (PowerNodeModel.gridEnergyImport).toFixed(1)
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                    }

                    Text {
                        id: text38
                        x: 33
                        y: 229
                        color: "#ffffff"   // "#f9bbcbc"
                        text: qsTr("kWh")
                        font.pixelSize: 12
                    }
                }

            }

        }

    }



    Rectangle {
        objectName: "PV-Generators"
        id: rectangle5
        x: 42
        y: 25
        width: 320
        height: 90
        opacity: 1
        visible: true
        color: PowerNodeModel.generatorColor
        //color: "#b3b3b3"                  // hellgrau, keine QML Basic/SVG color
        //color: "limegreen"                // Hellgrün
        radius: 15
        border.color: "#00000000"
        border.width: 0
        clip: false

        Rectangle {
            id: rectangleHintergrundSonne
            x: 9
            y: 7
            width: 74
            height: 74
            radius: 37
            color: PowerNodeModel.sunBGColor
            z: -0
            clip: false
        }

        Text {
            id: text1
            x: 114
            y: 6
            width: 92
            height: 23
            color: "#ffffff"
            text: "PV-Erzeugung"
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            font.bold: true
            textFormat: Text.RichText
        }
        Text {
            id: text7
            x: 151
            y: 67
            color: "#ffffff"
            text: qsTr("kW")
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            minimumPixelSize: 14
            font.bold: true
        }
        Text {
            id: text11
            x: 114
            y: 32
            width: 92
            height: 26
            color: PowerNodeModel.MBMDfigures
            //            color: "#ffffff"
//            text: (PowerNodeModel.generatorPowerTotal / 1000).toFixed(2)
            text: PowerNodeModel.generatorPowerTotal
            font.pixelSize: 23
            horizontalAlignment: Text.AlignHCenter
            font.family: "Arial"
        }
        Image {
            id: image3
            x: 4
            y: 6
            width: 83
            height: 78
            //            source: "Icons/Sonne_weiss_transparent.png"
            source: PowerNodeModel.sunColor
            fillMode: Image.PreserveAspectFit
            //            rotation: 22.5    // Startwert
            rotation: PowerNodeModel.sunAngle

            RoundButton {
                width: 70
                height: 70
                text: ""
                anchors.verticalCenter: parent.verticalCenter
                z: 1
                flat: true
                anchors.horizontalCenter: parent.horizontalCenter
                activeFocusOnTab: false
                hoverEnabled: false // Unicode Character 'CHECK MARK'
                visible: true
//                onClicked: PowerNodeModel.sunColor = "Icons/Sonne_invers_hellgrau.png"  // 2022-05-26
                onClicked: PowerNodeModel.openPopUpMsg()       // 2024-01-28
            }
        }
        Text {
            id: text22
            x: 216
            y: 6
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("Dach N")
            font.pixelSize: 12
        }
        Text {
            id: text52
            x: 216
            y: 21
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("Dach S")
            font.pixelSize: 12
        }
        Text {
            id: text23
            x: 216
            y: 36
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("Gaube")
            font.pixelSize: 12
        }
        Text {
            id: text24
            x: 216
            y: 51
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("Garage")
            font.pixelSize: 12
        }
        Text {
            id: text25
            x: 256
            y: 6
            width: 33
            height: 14
            color: PowerNodeModel.MBMDfigures
            //            color: "#ffffff"   // "#f9bbcbc"
//            text: (PowerNodeModel.generatorPowerDach / 1000).toFixed(2)
            text: PowerNodeModel.generatorPowerDachN
            font.pixelSize: 12
            horizontalAlignment: Text.AlignRight
        }
        Text {
            id: text53
            x: 256
            y: 21
            width: 33
            height: 14
            color: PowerNodeModel.MBMDfigures
            //            color: "#ffffff"   // "#f9bbcbc"
//            text: (PowerNodeModel.generatorPowerDach / 1000).toFixed(2)
            text: PowerNodeModel.generatorPowerDachS
            font.pixelSize: 12
            horizontalAlignment: Text.AlignRight
        }
        Text {
            id: text27
            x: 256
            y: 36
            width: 33
            height: 14
            color: PowerNodeModel.MBMDfigures
            //            color: "#ffffff"   // "#f9bbcbc"
//            text: (PowerNodeModel.generatorPowerGaube / 1000).toFixed(2)
            text: PowerNodeModel.generatorPowerGaube
            font.pixelSize: 12
            horizontalAlignment: Text.AlignRight
        }
        Text {
            id: text30
            x: 256
            y: 51
            width: 33
            height: 14
            color: PowerNodeModel.MBMDfigures
            //            color: "#ffffff"   // "#f9bbcbc"
//            text: (PowerNodeModel.generatorPowerGarage / 1000).toFixed(2)
            text: PowerNodeModel.generatorPowerGarage
            font.pixelSize: 12
            horizontalAlignment: Text.AlignRight
        }
        Text {
            id: text26
            x: 292
            y: 6
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("kW")
            font.pixelSize: 12
        }

        Text {
            id: text54
            x: 292
            y: 21
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("kW")
            font.pixelSize: 12
        }

        Text {
            id: text29
            x: 292
            y: 36
            width: 17
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("kW")
            font.pixelSize: 12
        }

        Text {
            id: text32
            x: 292
            y: 51
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("kW")
            font.pixelSize: 12
        }

        Row {
        }

        Text {
            id: text28
            x: 216
            y: 67
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("Ertrag")
            font.pixelSize: 12
        }

        Text {
            id: text31
            x: 244
            y: 67
            width: 45
            height: 14
            color: PowerNodeModel.MBMDfigures
            //            color: "#ffffff"   // "#f9bbcbc"
//            text: (PowerNodeModel.generatorTotalEnergy / 1000).toFixed(2)
            text: PowerNodeModel.generatorTotalEnergy.toFixed(2)
            font.pixelSize: 12
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: text42
            x: 292
            y: 67
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("MWh")
            font.pixelSize: 12
        }
    }

// Drawer für Auswahl ChargeMode
    // kleiner grauer Punkt als Hint wo mit der Maus gezogen werden muss, damit der Drawer aufgeht
    Rectangle {
        x: 379
        y: 476
        width: 10
        height: width
        color: "#d4d4d4"
        radius: width*0.5
    }

    // Mit swipe Bewegung der Maus von rechts nach links die ChargeMode Alternativen einblenden
    Drawer {
        id: drawer2
        y: 435-12                           // -- das muss zu window.height passen -> auf WallboxBox zentriert
        width: 0.66 * window.width
        height: 0.205 * window.height       // -- window.height -> und der Abstand zu den Markierungen passt
        dragMargin: 0.25 * window.width
        edge: Qt.RightEdge
        interactive: true

        background: Rectangle {
            Rectangle {
                x: 1
                width: parent.width - 2
                y: 1
                height: parent.height - 2
                color: "#00ac00"            // LIMEGREEN
            }
        }
        // Festlegung des Charge Mode (Off, Manual, Surplus, Quick)
        Button {        // change ChargeMode der Wallbox
            x: 20
            y: 10
            width: 100
            height: 16
            text: "OFF"
            z: 1
            flat: false
            activeFocusOnTab: false
            hoverEnabled: true
            onHoveredChanged: PowerNodeModel.showChargeMode()
            visible: true
            onClicked: PowerNodeModel.showChargeModeOFF()
        }
        Button {        // change ChargeMode der Wallbox
            x: 20
            y: 30
            width: 100
            height: 16
            text: "QUICK"
            z: 1
            flat: false
            activeFocusOnTab: false
            hoverEnabled: true
            onHoveredChanged: PowerNodeModel.showChargeMode()
            visible: true
            onClicked: PowerNodeModel.showChargeModeQUICK()
        }
        Button {        // change ChargeMode der Wallbox
            x: 20
            y: 50
            width: 100
            height: 16
            text: "SURPLUS"
            z: 1
            flat: false
            activeFocusOnTab: false
            hoverEnabled: true
            onHoveredChanged: PowerNodeModel.showChargeMode()
            visible: true
            onClicked: PowerNodeModel.showChargeModeSURPLUS()
        }
        Button {        // change ChargeMode der Wallbox
            x: 20
            y: 70
            width: 100
            height: 16
            text: "MANUAL"
            z: 1
            flat: false
            activeFocusOnTab: false
            hoverEnabled: true
            onHoveredChanged: PowerNodeModel.showChargeMode()
            visible: true
            onClicked: PowerNodeModel.showChargeModeMANUAL()
        }
        Text {          // Überschrift
            id: text51drawer
            x: 160
            y: 10
            width: 70
            color: "white"
            text: "Charge Mode"
            font.pixelSize: 16
            font.weight: Font.DemiBold
            horizontalAlignment: Text.AlignHCenter
        }
        Text {          // ChargeMode (OFF, SURPLUS, QUICK, MANUAL)
            id: text51drawer1
            x: 160
            y: 40
            width: 70
            color: "white"
            text: PowerNodeModel.chargeMode
            font.pixelSize: 16
            font.weight: Font.DemiBold
            horizontalAlignment: Text.AlignHCenter
        }
    }   // \drawer2
// \Drawer für Auswahl ChargeMode

// Drawer für Auswahl Manual ChargeCurrent
    // kleiner grauer Punkt als Hint wo mit der Maus gezogen werden muss, damit der Drawer aufgeht
    Rectangle {
        x: 15
        y: 476
        width: 10
        height: width
        color: "#d4d4d4"
        radius: width*0.5
    }

    // Mit swipe Bewegung der Maus von links nach rechts die ManualCurrent Alternativen einblenden
    Drawer {
        id: drawer3
        y: 435-12                           // -- das muss zu window.height passen -> auf WallboxBox zentriert
        width: 0.66 * window.width
        height: 0.205 * window.height       // -- window.height -> und der Abstand zu den Markierungen passt
        dragMargin: 0.25 * window.width
        edge: Qt.LeftEdge
        interactive: true

        background: Rectangle {
            Rectangle {
                x: 1
                width: parent.width - 2
                y: 1
                height: parent.height - 2
                color: "#00ac00"            // LIMEGREEN
            }
        }

// Festlegung des Manual Charge Current in 3 Stufen (6, 12, 18 A)
        Button {        // change ManualCurrent der Wallbox
            x: 20
            y: 10
            width: 100
            height: 16
            text: "  6A = 1380 W"
            z: 1
            flat: false
            activeFocusOnTab: false
            hoverEnabled: true
            onHoveredChanged: PowerNodeModel.showManualCurrent()    // set to 6A = 1380 W
            visible: true
            onClicked: PowerNodeModel.setManualCurrent6000()
        }
        Button {        // change ManualCurrent der Wallbox
            x: 20
            y: 30
            width: 100
            height: 16
            text: "12A = 2760 W"
            z: 1
            flat: false
            activeFocusOnTab: false
            hoverEnabled: true
            onHoveredChanged: PowerNodeModel.showManualCurrent()    // set to 12A = 2760 W
            visible: true
            onClicked: PowerNodeModel.setManualCurrent12000()
        }
        Button {        // change ManualCurrent der Wallbox
            x: 20
            y: 50
            width: 100
            height: 16
            text: "18A = 4140 W"
            z: 1
            flat: false
            activeFocusOnTab: false
            hoverEnabled: true
            onHoveredChanged: PowerNodeModel.showManualCurrent()    // set to 18A = 4140 W
            visible: true
            onClicked: PowerNodeModel.setManualCurrent18000()
        }

        Text {          // Überschrift
            id: text51drawer3
            x: 160
            y: 10
            width: 70
            color: "white"
            text: "Manual Current"
            font.pixelSize: 16
            font.weight: Font.DemiBold
            horizontalAlignment: Text.AlignHCenter
        }
        Text {          // manual Power (1380 W, 2760 W, 4140 W)
            id: text55drawer3
            x: 160
            y: 40
            width: 70
            color: "white"
            font.pixelSize: 16
            font.weight: Font.DemiBold
            text: PowerNodeModel.manualCurrentS
            horizontalAlignment: Text.AlignHCenter
        }
/*
        Button {        // change EVPercent for EV/house battery
            x: 20
            y: 80
            width: 34
            height: 16
            text: "0"
            z: 1
            flat: false
            activeFocusOnTab: false
            hoverEnabled: true
            onHoveredChanged: PowerNodeModel.showEVPercent()        // set X %
            visible: true
            onClicked: PowerNodeModel.setEVPercent10()
        }

        Button {        // change EVPercent for EV/house battery
            x: 55
            y: 80
            width: 34
            height: 16
            text: "50"
            z: 1
            flat: false
            activeFocusOnTab: false
            hoverEnabled: true
            onHoveredChanged: PowerNodeModel.showEVPercent()        // set X %
            visible: true
            onClicked: PowerNodeModel.setEVPercent50()
        }

        Button {        // change EVPercent for EV/house battery
            x: 90
            y: 80
            width: 34
            height: 16
            text: "100"
            z: 1
            flat: false
            activeFocusOnTab: false
            hoverEnabled: true
            onHoveredChanged: PowerNodeModel.showEVPercent()        // set X %
            visible: true
            onClicked: PowerNodeModel.setEVPercent100()
        }
*/
    }   // \drawer3
// \Drawer für Auswahl Manual ChargeCurrent


    Rectangle {
        objectName: "Wallbox"
        id: rectangle1              // WallBox
        x: 42
        y: 435
        width: 320
        height: 90
        color: PowerNodeModel.wallboxColor
        //color: "#b3b3b3"          // hellgrau, keine QML Basic/SVG color
        //color: "dodgerblue"       // #1e90ff
        //color: "#0A7CEB"          // um -20 dunkleres "dogerblue"
        radius: 15
        border.width: 0

        Text {
            id: text5
            x: 120
            y: 6
            width: 81
            height: 33
            color: "#ffffff"
            text: qsTr("Ladeleistung")
            font.pixelSize: 12
            wrapMode: Text.WordWrap
            font.bold: true
        }

        Text {          // ChargeMode (OFF, SURPLUS, QUICK, MANUAL)
            id: text51
            x: 217
            y: 8
            width: 70
            color: "#ffffff"
            text: PowerNodeModel.chargeMode
            font.pixelSize: 10
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            id: text6
            x: 149
            y: 68
            color: "#ffffff"
            text: qsTr("kW")
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            minimumPixelSize: 14
            font.bold: true
        }
        Text {
            id: text15
            x: 7
            y: 32
            width: 77
            height: 26
            color: PowerNodeModel.EDLDfigures
            //            color: "#ffffff"
            text: PowerNodeModel.chargingPower
            font.pixelSize: 23
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenterOffset: -1
            font.family: "Arial"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Image {
            id: image5
            x: 207
            y: 21
            width: 104
            height: 49
            //            source: "Icons/electric-car-icon_steckt_weiss_transparent.png"
            source: PowerNodeModel.wallboxCar
            fillMode: Image.PreserveAspectFit
            mirror: true
        }

        Image {
            id: image4
            x: 8
            y: 32
            width: 76
            height: 33
            //            source: "Icons/electric-scooter_icon_steckt_weiss_transparent_rad.png"
            source: PowerNodeModel.wallboxScoot
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: text16
            x: 12
            y: 71
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("Zuletzt:")
            font.pixelSize: 12
        }
        Text {
            id: text17
            x: 51
            y: 71
            width: 40
            height: 14
            color: PowerNodeModel.EDLDfigures
            //            color: "#ffffff"   // "#f9bbcbc"
            font.pixelSize: 12
            horizontalAlignment: Text.AlignRight
//            text: (PowerNodeModel.sessionEnergy / 1000).toFixed(3)
            text: PowerNodeModel.sessionEnergy
        }
        Text {
            id: text18
            x: 96
            y: 71
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("kWh")
            font.pixelSize: 12
        }
        Text {
            id: text19
            x: 190
            y: 71
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("Gesamt:")
            font.pixelSize: 12
        }
        Text {
            id: text20
            x: 236
            y: 71
            width: 50
            height: 14
            color: PowerNodeModel.EDLDfigures
            //            color: "#ffffff"   // "#f9bbcbc"
//            text: (PowerNodeModel.chargedEnergy / 1000).toFixed(2)
            text: PowerNodeModel.chargedEnergy
            font.pixelSize: 12
            horizontalAlignment: Text.AlignRight
        }
        Text {
            id: text21
            x: 288
            y: 71
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("kWh")
            font.pixelSize: 12
        }

        Text {
            id: text43
            x: 12
            y: 6
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("Eval.Pts:")
            font.pixelSize: 12
        }

        Text {
            id: text44
            x: 60
            y: 6
            width: 16
            height: 14
            color: PowerNodeModel.EDLDfigures
            //            color: "#ffffff"   // "#f9bbcbc"
            text: PowerNodeModel.evalPoints.toFixed(0)
            font.pixelSize: 12
            horizontalAlignment: Text.AlignLeft
        }

        Image {
            id: image14
            x: 292
            y: 4
            width: 20
            height: 18
            source: "Icons/WLAN_weiss_transparent.png"
            fillMode: Image.PreserveAspectFit
            visible: PowerNodeModel.visibleComm
        }

        Button {        // change picture of car/scooter (real/icon)
            x: 12
            y: 5
            width: 298
            height: 34
            text: ""
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 0
            anchors.horizontalCenterOffset: 3
            z: 1
            flat: true
            anchors.horizontalCenter: parent.horizontalCenter
            activeFocusOnTab: false
            hoverEnabled: false
            visible: true
            onClicked: PowerNodeModel.switchEVIcons()
        }

//        Text {
//            id: text50
//            x: 75
//            y: 6
//            width: 25
//            height: 14
//            text: PowerNodeModel.evalCountDown.toFixed(0)
//            font.pixelSize: 12
//            horizontalAlignment: Text.AlignRight
//        }
    }

    Rectangle {
        objectName: "Battery"
        id: rectangle2              // Battery
        x: 42
        y: 140
        width: 90
        height: 270
        color: PowerNodeModel.batteryColor
        //color: "#b3b3b3"          // hellgrau, keine QML Basic/SVG color
        //color: "forestgreen"      // Dunkelgrün
        //color: "limegreen"        // Hellgrün
        radius: 15
        border.width: 0

        Text {
            id: text2
            x: 1
            y: 6
            width: 88
            height: 33
            color: "#ffffff"
            text: PowerNodeModel.batteryText
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.bold: true
        }
        Text {
            id: text8
            x: 36
            y: 78
            color: "#ffffff"
            text: qsTr("kW")
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            minimumPixelSize: 14
            font.bold: true
        }
        Text {
            id: text12
            x: 7
            y: 46
            width: 77
            height: 26
            color: PowerNodeModel.EDLDfigures
            //            color: "#ffffff"
//            text: (PowerNodeModel.batteryPower / 1000).toFixed(2)
            text: PowerNodeModel.batteryPower
            font.pixelSize: 23
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Arial"
        }
        Image {
            id: batteryOutline
            x: 7
            y: 115
            width: 77
            height: 43
            visible: true
            source: "qrc:/Icons/Akku_weiss_transparent00.png"
            activeFocusOnTab: false
            focus: false
            enabled: false
            sourceSize.height: 32
            sourceSize.width: 58
            fillMode: Image.PreserveAspectFit

            Rectangle {
                id: batteryFill
                x: 12.5
                y: 11
                width: (PowerNodeModel.batteryFill / 100) * 45.5
                height: 19
                color: "White"
            }
        }

        Text {
            id: text33
            x: 7
            y: 216
            width: 45
            height: 26
            color: PowerNodeModel.EDLDfigures
            //            color: "#ffffff"
            // ohne Nachkommastellen, rechtsbündig
            text: (PowerNodeModel.batteryPercentage).toFixed(0)
            font.pixelSize: 23
            horizontalAlignment: Text.AlignRight
            font.family: "Arial"
        }

        Text {
            id: text34
            x: 1
            y: 177
            width: 88
            height: 33
            color: "#ffffff"
            text: qsTr("Batterie-ladezustand")
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.bold: true
        }

        Text {
            id: text35
            x: -2
            y: 216
            width: 26
            height: 26
            color: "#ffffff"
            text: qsTr("%")
            font.pixelSize: 23
            horizontalAlignment: Text.AlignLeft
            anchors.horizontalCenterOffset: 24
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Arial"
        }


        Text {
            id: text45
            x: 11
            y: 153
            width: 33
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("Temp.")
            font.pixelSize: 12
        }

        Text {
            id: text46
            x: 56
            y: 153
            width: 16
            height: 14
            color: PowerNodeModel.EDLDfigures
            //            color: "#ffffff"   // "#f9bbcbc"
            text: PowerNodeModel.battTemp.toFixed(1)
            font.pixelSize: 12
            horizontalAlignment: Text.AlignRight
        }

        Text {
            id: text47
            x: 73
            y: 152
            width: 6
            height: 14
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("°")
            font.pixelSize: 12
        }

    }



    Rectangle {
        objectName: "Grid"
        id: rectangle4                      // Grid
        x: 272
        y: 140
        width: 90
        height: 270
        color: PowerNodeModel.gridColor
        //        color: "#b3b3b3"          // hellgrau, keine QML Basic/SVG color
        //        color: "limegreen"        // Hellgrün
        //        color: "firebrick"        // Dunkelrot
        radius: 15
        border.width: 0
        
        Text {
            id: text4
            x: 1
            y: 6
            width: 88
            height: 33
            color: "#ffffff"
            //text: qsTr("Netz-einspeisung")
            //text: qsTr("Netzbezug")
            text: PowerNodeModel.gridText
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.bold: true
        }
        
        
        Text {
            id: text10
            x: 36
            y: 78
            color: "#ffffff"
            text: qsTr("kW")
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            minimumPixelSize: 14
            font.bold: true
        }

        Text {
            id: text14
            x: 7
            y: 46
            width: 77
            height: 26
            color: PowerNodeModel.EDLDfigures
            //            color: "#ffffff"
//            text: (PowerNodeModel.gridPower / 1000).toFixed(2)
            text: PowerNodeModel.gridPower
            font.pixelSize: 23
            horizontalAlignment: Text.AlignHCenter
            font.family: "Arial"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Image {
            id: image2
            x: 1
            y: 98
            width: 88
            height: 85
            source: "Icons/Mast_vec_weiss_transparent.png"
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: text39
            x: 13
            y: 199
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("Einspeisung")
            font.pixelSize: 12
        }

        Text {
            id: text40
            x: 8
            y: 214
            width: 74
            height: 14
            color: PowerNodeModel.EDLDfigures
            //            color: "#ffffff"   // "#f9bbcbc"
            // eine Nachkommastelle, zentriert
            text: (PowerNodeModel.gridEnergyExport).toFixed(1)
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            id: text41
            x: 33
            y: 229
            color: "#ffffff"   // "#f9bbcbc"
            text: qsTr("kWh")
            font.pixelSize: 12
        }

        Text {
            // "EDLD hat Probleme"
            id: text48
            x: -28
            y: 276
            width: 117
            height: 14
            color: "#000000"  // schwarz // "#ff0000" rot
            text: PowerNodeModel.EDLDProblemText
            font.bold : true
            font.pixelSize: 12
            horizontalAlignment: Text.AlignRight
        }

        Text {
            // "MBMD hat Probleme"
            id: text49
            x: -230
            y: 276
            width: 117
            height: 14
            color: "#000000"  // schwarz // "#ff0000" rot
            text: PowerNodeModel.MBMDProblemText
            font.bold : true
            font.pixelSize: 12
            horizontalAlignment: Text.AlignLeft
        }

        Text {
            // "Ein Wechselrichter hat Probleme"
            id: text50
            x: -240
            y: -133
            width: 338
            height: 14
            color: "#000000"  // schwarz // "#ff0000" rot
            text: PowerNodeModel.WRProblemText
            font.bold : true
            font.pixelSize: 12
            horizontalAlignment: Text.AlignLeft
        }
    }




    Item {
        id: arrows
        Image {
            id: image6
            x: 132
            y: 263
            width: 25
            height: 25
            opacity: 0.5
            visible: PowerNodeModel.batt2house
            source: "Icons/FF.png"
            clip: false
            antialiasing: false
            smooth: true
            fillMode: Image.PreserveAspectFit
        }

        Image {
            id: image7
            x: 247
            y: 263
            width: 25
            height: 25
            opacity: 0.5
            source: "Icons/FF.png"
            fillMode: Image.PreserveAspectFit
            clip: false
            antialiasing: false
            smooth: true
            rotation: 180
            visible: PowerNodeModel.grid2house
        }

        Image {
            id: image12
            x: 132
            y: 263
            width: 25
            height: 25
            opacity: 0.5
            visible: PowerNodeModel.house2batt
            source: "Icons/FF.png"
            layer.enabled: true
            clip: false
            antialiasing: false
            smooth: true
            fillMode: Image.PreserveAspectFit
            rotation: 180
        }

        Image {
            id: image8
            x: 75
            y: 115
            width: 25
            height: 25
            opacity: 0.5
            source: "Icons/FF.png"
            clip: false
            antialiasing: false
            smooth: true
            rotation: 90
            fillMode: Image.PreserveAspectFit
            visible: PowerNodeModel.pv2batt
        }

        Image {
            id: image9
            x: 188
            y: 115
            width: 25
            height: 25
            opacity: 0.5
            source: "Icons/FF.png"
            fillMode: Image.PreserveAspectFit
            clip: false
            antialiasing: false
            smooth: true
            rotation: 90
            visible: PowerNodeModel.pv2house
        }

        Image {
            id: image10
            x: 305
            y: 115
            width: 25
            height: 25
            opacity: 0.5
            source: "Icons/FF.png"
            fillMode: Image.PreserveAspectFit
            clip: false
            antialiasing: false
            smooth: true
            rotation: 90
            visible: PowerNodeModel.pv2grid
        }

        Image {
            id: image11
            x: 190
            y: 410
            width: 25
            height: 25
            opacity: 0.5
            source: "Icons/FF.png"
            fillMode: Image.PreserveAspectFit
            clip: false
            antialiasing: false
            smooth: true
            rotation: 90
            visible: PowerNodeModel.house2charger
        }

        Image {
            id: image13
            x: 247
            y: 263
            width: 25
            height: 25
            opacity: 0.5
            visible: PowerNodeModel.house2grid
            source: "Icons/FF.png"
            fillMode: Image.PreserveAspectFit
            rotation: 0
            smooth: true
            clip: false
            antialiasing: false
        }
    }
}



/*##^##
Designer {
    D{i:0;formeditorZoom:1.33}
}
##^##*/
