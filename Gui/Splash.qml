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
            running: true
            duration: 1000
            from: 0
            to: 1
            easing.type: Easing.OutQuad
        }
        NumberAnimation on opacity {
            id: opacityAni
            running: false
            duration: 2000
            to: 0
        }
    }

    Item {
        anchors.centerIn: parent
        width: cardirector.width
        height: cardirector.height

        ParticleSystem { id: dots }
        CustomParticle {
            system: dots
            property real maxWidth: cardirector.width
            property real maxHeight: cardirector.height

            ShaderEffectSource {
                id: pictureSource
                sourceItem: cardirector
            }
            ShaderEffectSource {
                id: particleSource
                sourceItem: particle
                hideSource: true
            }
            Rectangle {
                id: particle
                width: 16
                height: 16
                radius: 8
            }

            vertexShader:"
            uniform highp float maxWidth;
            uniform highp float maxHeight;
            varying highp vec2 fTex2;
            varying lowp float fFade;
            uniform lowp float qt_Opacity;

            void main() {

                fTex2 = vec2(qt_ParticlePos.x, qt_ParticlePos.y);
                //Comment this next line for each particle to use solid color at the center of the particle, instead of the full texture.
                fTex2 = fTex2 + ((- qt_ParticleData.z / 2. + qt_ParticleData.z) * qt_ParticleTex); //Adjusts size so it's like a chunk of image.
                fTex2 = fTex2 / vec2(maxWidth, maxHeight);
                highp float t = (qt_Timestamp - qt_ParticleData.x) / qt_ParticleData.y;
                fFade = min(t*4., (1.-t*t)*.75) * qt_Opacity;
                defaultMain();
            }
        "

            property variant particleTexture: particleSource
            property variant pictureTexture: pictureSource

            fragmentShader: "
            uniform sampler2D particleTexture;
            uniform sampler2D pictureTexture;
            varying highp vec2 qt_TexCoord0;
            varying highp vec2 fTex2;
            varying lowp float fFade;
            void main() {
                gl_FragColor = texture2D(pictureTexture, fTex2) * texture2D(particleTexture, qt_TexCoord0).w * fFade;
        }"

        }

        Emitter {
            id: emitter
            system: dots
            enabled: false
            lifeSpan: 8000
            maximumEmitted: 4000
            anchors.fill: parent
            size: 16
            acceleration: PointDirection { xVariation: 8; yVariation: 8 }
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
        id: textAni

        running: true
        NumberAnimation { target: blur; property: "radius"; to: 0; duration: 800; easing.type: Easing.InOutQuad }
        NumberAnimation { target: scale; property: "xScale"; to: 1; duration: 1000; easing.type: Easing.OutQuad }
        NumberAnimation { target: scale; property: "yScale"; to: 1; duration: 1000; easing.type: Easing.OutQuad }
        NumberAnimation { target: blur; property: "opacity"; from: 0; to: 1; duration: 500 }
    }

    NumberAnimation on opacity {
        id: fadeAni
        running: false
        duration: 3000; easing.type: Easing.OutQuad; to: 0;
        onStopped: bg.disappearing()
    }

    Timer {
        id: fadeTimer
        interval: 8000
        onTriggered: fadeAni.start();
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
            if (!textAni.running) {
                hide();
            }
        }
    }

    Keys.onPressed: {
        if (!textAni.running) {
            hide();
            event.accepted = true;
        }
    }

    function hide() {
        emitter.burst(4000);
        opacityAni.start();
        fadeTimer.start();
    }
}
