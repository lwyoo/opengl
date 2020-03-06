import QtQuick 2.0
import OpenGLUnderQML 1.0

Item {
    width: 1000
    height: 1000

    Squircle {
        x : 0
        y : 0
        width: 512
        height: 512
       // rotation: 30
        ///scale : 1.2
        texture : ":/wall.jpg"

        SequentialAnimation on t {
            id:asd
            NumberAnimation { from: 0; to: 1; duration: 1; easing.type: Easing.InQuad }
//            NumberAnimation { to: 0; duration: 1; easing.type: Easing.OutQuad }
//            loops: Animation.Infinite
            running: false
        }
    }

    Rectangle {
        x: 100
        y: 100
        width: 100
        height: 100
        color: "yellow"
        MouseArea{
            anchors.fill: parent
            onClicked: {
                console.log("touch ~~~")
                asd.start()
            }
        }


    }
}
