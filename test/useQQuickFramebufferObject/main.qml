import QtQuick 2.0
import OpenGLUnderQML 1.0

Item {
    width: 1000
    height: 1000

    Squircle {
        x : 0
        y : 0
        width: 5120
        height: 5120
       // rotation: 30
        ///scale : 1.2
        texture : ":/wall.jpg"

        SequentialAnimation on t {
            NumberAnimation { to: 1; duration: 2500; easing.type: Easing.InQuad }
            NumberAnimation { to: 0; duration: 2500; easing.type: Easing.OutQuad }
            loops: Animation.Infinite
            running: true
        }
    }
}
