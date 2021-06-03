import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id: window
    width: 400
    height: 520
    visible: true
    title: qsTr("PV-Anzeige")
    
    
    Rectangle {
        id: rectangle
        x: 42
        y: 10
        width: 320
        height: 90
        opacity: 1
        visible: true
        color: "#b3b3b3"
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
            text: "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\np, li { white-space: pre-wrap; }\n</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt; font-weight:600; color:#ffffff;\">PV-Erzeugung</span></p></body></html>"
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
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
            text: qsTr("11,09")
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
    }
    
    
    Rectangle {
        id: rectangle1
        x: 42
        y: 411
        width: 320
        height: 90
        color: "#b3b3b3"
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
            text: qsTr("3,18")
            font.pixelSize: 23
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenterOffset: -1
            font.family: "Arial"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        
        Image {
            id: image5
            x: 229
            y: 24
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
            fillMode: Image.PreserveAspectFit
        }
    }
    
    
    
    
    
    Rectangle {
        id: rectangle2
        x: 42
        y: 120
        width: 90
        height: 270
        color: "#b3b3b3"
        radius: 15
        border.width: 0
        
        Text {
            id: text2
            x: 1
            y: 6
            width: 88
            height: 33
            color: "#ffffff"
            text: qsTr("Batterie- ladung")
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
            text: qsTr("3,18")
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
            source: "Icons/Akku_weiss_transparent.png"
            activeFocusOnTab: false
            focus: false
            enabled: false
            sourceSize.height: 32
            sourceSize.width: 58
            fillMode: Image.PreserveAspectFit
        }
    }
    
    
    
    
    
    
    
    Rectangle {
        id: rectangle3
        x: 157
        y: 120
        width: 90
        height: 270
        color: "#b3b3b3"
        radius: 15
        border.width: 0
        
        Text {
            id: text3
            x: 2
            y: 6
            width: 87
            height: 33
            color: "#ffffff"
            text: qsTr("Gesamt-verbrauch")
            font.pixelSize: 12
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.bold: true
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
            text: qsTr("1,24")
            font.pixelSize: 23
            horizontalAlignment: Text.AlignHCenter
            font.family: "Arial"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Image {
            id: image1
            x: 0
            y: 98
            width: 90
            height: 85
            source: "Icons/Haus_weiss_transparent.png"
            fillMode: Image.PreserveAspectFit
        }
    }
    
    
    
    
    
    
    
    
    Rectangle {
        id: rectangle4
        x: 272
        y: 120
        width: 90
        height: 270
        color: "#b3b3b3"
        radius: 15
        border.width: 0
        
        Text {
            id: text4
            x: 1
            y: 6
            width: 88
            height: 33
            color: "#ffffff"
            text: qsTr("Netzbezug")
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
            text: qsTr("0,16")
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
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
}


