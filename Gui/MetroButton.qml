import QtQuick 2.4
import Cardirector.Device 1.0

Item {
    property bool enabled: true
    property alias text: title.text
    property alias textColor: title.color
    property alias textFont: title.font
    property alias backgroundColor: bg.color
    property alias border: bg.border
    property alias iconSource: icon.source

    signal clicked

    id: button
    width: Device.gu(189)
    height: Device.gu(46)

    Rectangle {
        id: bg
        anchors.fill: parent
        color: "black"
        border.width: Device.gu(2)
        border.color: "white"
        opacity: 0.8
    }

    states: [
        State {
            name: "hovered"; when: mouse.containsMouse
            PropertyChanges { target: bg; color: "white" }
            PropertyChanges { target: title; color: "black" }
        },
        State {
            name: "disabled"; when: !enabled
            PropertyChanges { target: button; opacity: 0.2 }
        }
    ]

    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: parent.enabled
        onReleased: parent.clicked()
    }

    Row {
        anchors.centerIn: parent
        spacing: Device.gu(5)
        Image {
            id: icon
            anchors.verticalCenter: parent.verticalCenter
            fillMode: Image.PreserveAspectFit
        }

        Text {
            id: title
            font.pixelSize: 18
            font.family: "WenQuanYi Micro Hei"
            anchors.verticalCenter: parent.verticalCenter
            text: ""
            color: "white"
        }
    }
}

