import QtQuick 2.4

Rectangle {
    id: rect
    color: "#0000007f"
    width: parent.width
    height: parent.height
    anchors.horizontalCenter: parent.horizontalCenter
    state: "showing"

    Behavior on scale {
        NumberAnimation { easing.type: Easing.OutBack; duration: 1000; }
    }

    states: [
        State {
            name: "zoomout"
            PropertyChanges {
                target: rect
                scale: 0.3
            }

        },
        State {
            name: "showing"
            extend: "zoomout"
            AnchorChanges {
                target: rect
                anchors.right: rect.parent.left
            }
        },
        State {
            name: "normal"
            AnchorChanges {
                target: rect
                anchors.verticalCenter: rect.parent.verticalCenter
            }
        },
        State {
            name: "hiding"
            extend: "zoomout"
            AnchorChanges {
                target: rect
                anchors.left: rect.parent.right
            }
        }
    ]

    transitions: [
        Transition {
            AnchorAnimation { duration: 1000 }
        }
    ]

    Component.onCompleted: state = "normal"

    MouseArea {
        onClicked: state = "hiding"
    }
}
