import QtQuick 2.4
import Cardirector.Device 1.0
import QtGraphicalEffects 1.0
import QtQuick.Particles 2.0

Image {
    id: bg
    source: "qrc:/image/splash0.png"
    anchors.fill: parent
    z: 1000
    focus: true

    signal disappearing

    FontLoader {
        id: loader
        source: "qrc:/font/Braxton Free.otf"
    }

    Image {
        id: cardirector
        source: "qrc:/image/splash1.png"
        cache: false
        anchors.centerIn: parent
        width: parent.width
        fillMode: Image.PreserveAspectFit
        NumberAnimation on scale {
            id: ani1
            running: true
            duration: 1000
            from: 0
            to: 1
            easing.type: Easing.OutQuad
        }
    }

    Text {
        id: mogara
        text: "Mogara"
        font.family: loader.name
        color: "white"
        font.pointSize: 36
        anchors.top: cardirector.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: Device.gu(100)
        opacity: 0
    }

    FastBlur {
        id: blur
        anchors.fill: mogara
        source: mogara
        radius: 100

        transform: [
            Scale { id: scale; origin.x: blur.width / 2; origin.y: blur.height / 2; xScale: 0.2; yScale: 0.2 }
        ]
    }

    ParallelAnimation {
        id: ani2

        running: !ani1.running
        NumberAnimation { target: blur; property: "radius"; to: 0; duration: 800; easing.type: Easing.InOutQuad }
        NumberAnimation { target: scale; property: "xScale"; to: 1; duration: 1000; easing.type: Easing.OutQuad }
        NumberAnimation { target: scale; property: "yScale"; to: 1; duration: 1000; easing.type: Easing.OutQuad }
        NumberAnimation { target: blur; property: "opacity"; from: 0; to: 1; duration: 500 }
    }

    NumberAnimation on opacity {
        id: ani3
        running: false
        duration: 3000; easing.type: Easing.OutQuad; to: 0;
        onStopped: bg.disappearing()
    }

    ParticleSystem { id: particles }
    ImageParticle {
        system: particles
        sprites: Sprite {
            name: "snow"
            source: "qrc:/image/snowflake.png"
            frameCount: 51
            frameDuration: 100
            frameDurationVariation: 90
        }
    }

    Wander {
        id: wanderer
        system: particles
        anchors.fill: parent
        xVariance: 360/(wanderer.affectedParameter+1);
        pace: 100*(wanderer.affectedParameter+1);
        affectedParameter: Wander.Velocity
    }

    Emitter {
        system: particles
        emitRate: 100
        lifeSpan: parent.height / 768 * 7000
        velocity: PointDirection { y:80; yVariation: 40; }
        acceleration: PointDirection { y: 4 }
        size: 20
        sizeVariation: 10
        width: parent.width
        height: 1

        anchors.top: parent.top
    }

    MouseArea {
        acceptedButtons: Qt.AllButtons
        anchors.fill: parent
        onClicked: {
            if (!ani1.running && !ani2.running) {
                ani3.start();
            }
        }
    }

    Keys.onPressed: {
        if (!ani1.running && !ani2.running) {
            ani3.start();
            event.accepted = true
        }
    }
}
