import QtQuick 2.12
import QtQuick.Window 2.12

import Smarf.PowerNodeModel 1.0

Window {
    id: window
    width: 400
    height: 560
    opacity: 1
    visible: true
    color: "whitesmoke"                     // sehr helles Grau für Fensterhintergrund
    title: qsTr("PV-Anzeige")

    property alias rectangle1: rectangle1
    property alias rectangle4: rectangle4
    property alias rectangle3: rectangle3
    property alias rectangle2: rectangle2
    property alias rectangle5: rectangle5


    Rectangle {
        id: rectangle3
        x: 157
        y: 140
        width: 90
        height: 270
        color: PowerNodeModel.homeColor
        //color: "#b3b3b3"                  // hellgrau, keine QML Basic/SVG color
        //color: "limegreen"                // Hellgrün
        radius: 15
        border.width: 0
        //clip: true

        // Überlagerte Farben in der Haus-Box mit Texten und Bild
            /*
                id: rectangleRed
                x: 237
                y: 140
                width: 10
                height: PowerNodeModel.homeTopRedH
                opacity: 1
                visible: true
                color: "FIREBRICK"
                radius: 0
                border.width: 0
            */

            /*
                id: rectangleGreen
                x: 157
                y: 410 - PowerNodeModel.homeBotGreenH
                width: 10
                height: PowerNodeModel.homeBotGreenH
                opacity: 1
                visible: true
                color: "FORESTGREEN"
                radius: 0
                border.width: 0
          */

            Rectangle {
                id: rectangleRed
                layer.enabled: true
                x: 0
                y: 0
                width: 90
                height: PowerNodeModel.homeTopRedH
                visible: true
                color: "FIREBRICK"
                opacity: 1
                radius: 0
                border.width: 0
            }

            Rectangle {
                id: rectangleGreen
                layer.enabled: true
                x: 0
                y: 270 - PowerNodeModel.homeBotGreenH
                width: 90
                height: PowerNodeModel.homeBotGreenH
                visible: true
                color: "FORESTGREEN"
                opacity: 1
                radius: 0
                border.width: 0
            }

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
                x: 0
                y: 95
                width: 90
                height: 85
                source: "Icons/Haus_weiss_transparent.png"
                fillMode: Image.PreserveAspectFit
            }

            Text {
                id: text36
                x: 29
                y: 198
                color: "#f9bcbc"
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
                color: "#ffffff"
                text: (PowerNodeModel.consumptionPower / 1000.0).toFixed(2)
                font.pixelSize: 23
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenterOffset: 1
                font.family: "Arial"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                id: text37
                x: 8
                y: 214
                width: 74
                height: 14
                color: "#f9bcbc"
                // ohne Nachkommastellen, zentriert
                text: (PowerNodeModel.gridEnergyImport)
                font.pixelSize: 12
                horizontalAlignment: Text.AlignHCenter
            }

            Text {
                id: text38
                x: 33
                y: 229
                color: "#f9bcbc"
                text: qsTr("kWh")
                font.pixelSize: 12
            }

        Image {
            id: image6
            x: -25
            y: 123
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
            id: image12
            x: -25
            y: 123
            width: 25
            height: 25
            opacity: 0.5
            visible: PowerNodeModel.house2batt
            source: "Icons/FF.png"
            clip: false
            antialiasing: false
            smooth: true
            fillMode: Image.PreserveAspectFit
            rotation: 180
        }

        Image {
            id: image7
            x: 90
            y: 123
            width: 25
            height: 25
            opacity: 0.5
            source: "Icons/FF.png"
            clip: false
            antialiasing: false
            smooth: true
            rotation: 180
            fillMode: Image.PreserveAspectFit
            visible: PowerNodeModel.grid2house
        }
    }


    Rectangle {
        objectName: "rectangle5"
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
            color: "#ffffff"
            text: (PowerNodeModel.generatorPowerTotal / 1000.0).toFixed(2)
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
            source: "Icons/Sonne_weiss_transparent.png"
            fillMode: Image.PreserveAspectFit
        }
        Text {
            id: text22
            x: 220
            y: 11
            color: "#f9bcbc"
            text: qsTr("Dach")
            font.pixelSize: 12
        }
        Text {
            id: text23
            x: 220
            y: 34
            color: "#f9bcbc"
            text: qsTr("Gaube")
            font.pixelSize: 12
        }
        Text {
            id: text24
            x: 220
            y: 55
            color: "#f9bcbc"
            text: qsTr("Garage")
            font.pixelSize: 12
        }
        Text {
            id: text25
            x: 256
            y: 11
            width: 33
            height: 14
            color: "#f9bcbc"
            text: (PowerNodeModel.generatorPowerDach / 1000.0).toFixed(2)
            font.pixelSize: 12
            horizontalAlignment: Text.AlignRight
        }
        Text {
            id: text27
            x: 256
            y: 34
            width: 33
            height: 14
            color: "#f9bcbc"
            text: (PowerNodeModel.generatorPowerGaube / 1000.0).toFixed(2)
            font.pixelSize: 12
            horizontalAlignment: Text.AlignRight
        }
        Text {
            id: text30
            x: 256
            y: 55
            width: 33
            height: 14
            color: "#f9bcbc"
            text: (PowerNodeModel.generatorPowerGarage / 1000.0).toFixed(2)
            font.pixelSize: 12
            horizontalAlignment: Text.AlignRight
        }
        Text {
            id: text26
            x: 294
            y: 11
            color: "#f9bcbc"
            text: qsTr("kW")
            font.pixelSize: 12
        }

        Text {
            id: text29
            x: 294
            y: 34
            width: 17
            color: "#f9bcbc"
            text: qsTr("kW")
            font.pixelSize: 12
        }

        Text {
            id: text32
            x: 294
            y: 56
            color: "#f9bcbc"
            text: qsTr("kW")
            font.pixelSize: 12
        }

        Image {
            id: image8
            x: 33
            y: 90
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
            x: 146
            y: 90
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
            x: 263
            y: 90
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
    }



    Rectangle {
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
            color: "#ffffff"
            text: (PowerNodeModel.chargingPower / 1000.0).toFixed(2)
            font.pixelSize: 23
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenterOffset: -1
            font.family: "Arial"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Image {
            id: image5
            x: 229
            y: 22
            width: 82
            height: 55
            source: "Icons/electric-car-icon_weiss_transparent.png"
            fillMode: Image.PreserveAspectFit
        }
        Image {
            id: image4
            x: 12
            y: 26
            width: 64
            height: 39
            source: "Icons/electric-scooter_icon_weiss_transparent_rad.png"
            rotation: 0
            fillMode: Image.PreserveAspectFit
        }
        Text {
            id: text16
            x: 12
            y: 71
            color: "#f9bcbc"
            text: qsTr("Zuletzt:")
            font.pixelSize: 12
        }
        Text {
            id: text17
            x: 51
            y: 71
            width: 40
            height: 14
            color: "#f9bcbc"
            font.pixelSize: 12
            horizontalAlignment: Text.AlignRight
            text: (PowerNodeModel.sessionEnergy / 1000.0).toFixed(2)
        }
        Text {
            id: text18
            x: 96
            y: 71
            color: "#f9bcbc"
            text: qsTr("kWh")
            font.pixelSize: 12
        }
        Text {
            id: text19
            x: 194
            y: 71
            color: "#f9bcbc"
            text: qsTr("Gesamt:")
            font.pixelSize: 12
        }
        Text {
            id: text20
            x: 240
            y: 71
            width: 44
            height: 14
            color: "#f9bcbc"
            text: (PowerNodeModel.chargedEnergy / 1000.0).toFixed(2)
            font.pixelSize: 12
            horizontalAlignment: Text.AlignRight
        }
        Text {
            id: text21
            x: 288
            y: 71
            color: "#f9bcbc"
            text: qsTr("kWh")
            font.pixelSize: 12
        }

        Image {
            id: image11
            x: 148
            y: -25
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
    }





    Rectangle {
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
            color: "#ffffff"
            text: (PowerNodeModel.batteryPower / 1000.0).toFixed(2)
            font.pixelSize: 23
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Arial"
        }
        Image {
            id: image
            x: 7
            y: 119
            width: 77
            height: 43
            visible: true
            source: PowerNodeModel.batteryImage
            activeFocusOnTab: false
            focus: false
            enabled: false
            sourceSize.height: 32
            sourceSize.width: 58
            fillMode: Image.PreserveAspectFit
        }
        Text {
            id: text33
            x: 7
            y: 216
            width: 45
            height: 26
            color: "#ffffff"
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

    }

    Rectangle {
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
            color: "#ffffff"
            text: (PowerNodeModel.gridPower / 1000.0).toFixed(2)
            font.pixelSize: 23
            horizontalAlignment: Text.AlignHCenter
            font.family: "Arial"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Image {
            id: image2
            x: 3
            y: 103
            width: 85
            height: 87
            source: "Icons/Mast_weiss_transparent.png"
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: text39
            x: 13
            y: 199
            color: "#f9bcbc"
            text: qsTr("Einspeisung")
            font.pixelSize: 12
        }

        Text {
            id: text40
            x: 8
            y: 214
            width: 74
            height: 14
            color: "#f9bcbc"
            // ohne Nachkommastellen, zentriert
            text: (PowerNodeModel.gridEnergyExport)
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
        }

        Text {
            id: text41
            x: 33
            y: 229
            color: "#f9bcbc"
            text: qsTr("kWh")
            font.pixelSize: 12
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.33}
}
##^##*/
