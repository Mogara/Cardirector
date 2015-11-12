import QtQuick 2.4

Item {
    id: container
    property int minWidth: 100
    property int minHeight: 100

    width: Math.max(parent.width, minWidth)
    height: Math.max(parent.height, minHeight)
    readonly property real scale: Math.min(parent.width / width, parent.height / height)

    transform: [
        Scale {
            xScale: container.scale
            yScale: container.scale
        },
        Translate {
            x: (container.parent.width - container.width * container.scale) / 2
            y: (container.parent.height - container.height * container.scale) / 2
        }
    ]
}
