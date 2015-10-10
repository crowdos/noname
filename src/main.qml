import QtQuick 2.0
import QtCompositor 1.0
import Compositor 1.0

Rectangle {
    id: root
    width: 200
    height: 400
    color: "white"
    focus: true

    WindowList {
        model: compositor.windowList
    }

    Rectangle {
        id: fs
        color: "white"
        anchors.fill: parent
        visible: opacity > 0
        opacity: minimizer.targetOpacity != 1 ? minimizer.targetOpacity : compositor.fullScreenSurface == null ? 0 : 1

        Behavior on opacity {
            NumberAnimation {duration: 250}
        }

        ShaderEffect {
            anchors.fill: parent
            property variant source: compositor.item(compositor.fullScreenSurface)
            enabled: source != null
            onSourceChanged: {
                if (source) {
                    source.parent = fs
                }
            }
        }
/*
        MouseArea {
            anchors.fill: parent
            onClicked: compositor.fullScreenSurface = null
        }*/
    }

    function windowAdded(window) {
    }

    function windowRemoved(window) {
    }

    MouseArea {
        id: minimizer
        anchors {
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }

        width: 20

        property real lastX
        property real lastY
        property real targetOpacity: 1

        onPressed: {
            if (compositor.fullScreenSurface == null) {
                mouse.accepted = false
            }

            lastX = mouse.x
            lastY = mouse.y
        }

        onPositionChanged: {
            var diff = lastX - mouse.x
            if (diff > width) {
                if (diff > 100) {
                    targetOpacity = 0
                } else {
                    targetOpacity = 1/diff
                }
            }
        }

        onReleased: {
            var diff = lastX - mouse.x

            if (diff > 100) {
                compositor.fullScreenSurface = null
                targetOpacity = 1
            } else {
                targetOpacity = 1
            }
        }
    }
}
