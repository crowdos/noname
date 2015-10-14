import QtQuick 2.0
import QtCompositor 1.0
import Compositor 1.0

Rectangle {
    id: root
    width: 200
    height: 400
    color: "white"
    focus: true

    // TODO: configurable
    readonly property real edgeSize: 20

    QtObject {
        id: edgeHandlers
        property string emptyTopEdgeHandler: Qt.resolvedUrl("plugins/edge/Lock.qml")
        property string emptyRightEdgeHandler: Qt.resolvedUrl("plugins/edge/")
        property string emptyBottomEdgeHandler: Qt.resolvedUrl("plugins/edge/")
        property string emptyLeftEdgeHandler: Qt.resolvedUrl("plugins/edge/")
        property string fullTopEdgeHandler: Qt.resolvedUrl("plugins/edge/")
        property string fullRightEdgeHandler: Qt.resolvedUrl("plugins/edge/Minimize.qml")
        property string fullBottomEdgeHandler: Qt.resolvedUrl("plugins/edge/")
        property string fullLeftEdgeHandler: Qt.resolvedUrl("plugins/edge/")
        property string topEdgeHandler: compositor.fullScreenSurface ? fullTopEdgeHandler : emptyTopEdgeHandler
        property string rightEdgeHandler: compositor.fullScreenSurface ? fullRightEdgeHandler : emptyRightEdgeHandler
        property string bottomEdgeHandler: compositor.fullScreenSurface ? fullBottomEdgeHandler : emptyBottomEdgeHandler
        property string leftEdgeHandler: compositor.fullScreenSurface ? fullLeftEdgeHandler : emptyLeftEdgeHandler
    }

    // Those properties are for edge handlers:
//    property Item fullScreenSurface: compositor.fullScreenSurface
    property real minimizeAnimationProgress: 0
    property bool minimizeAnimationRunning: false

//    property Item screenLocker: dimmer
    property real screenLockAnimationProgress: 0
    property bool screenLockAnimationRunning: false

    WindowList {
        model: compositor.windowList
    }

    Rectangle {
        id: fs
        color: "white"
        anchors.fill: parent
        visible: opacity > 0
        opacity: minimizeAnimationRunning ? 1 - minimizeAnimationProgress : compositor.fullScreenSurface == null ? 0 : 1

        Behavior on opacity {
            enabled: !minimizeAnimationRunning
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
    }

    function windowAdded(window) {
    }

    function windowRemoved(window) {
    }

    MouseArea {
        id: top
        anchors {
            top: parent.top
            right: parent.right
            left: parent.left
        }
        height: edgeSize

        property real lastX
        property real lastY
        property bool sendStart: false

        onPressed: {
            sendStart = true
            if (!topLoader.item) {
                mouse.accepted = false
            } else {
                lastX = mouse.x
                lastY = mouse.y
            }
        }

        onPositionChanged: {
            var diff = mouse.y - lastY
            if (diff > height) {
                if (sendStart) {
                    sendStart = false
                    topLoader.item.started()
                }

                topLoader.item.dragProgress(diff)
            }
        }

        onReleased: {
            var diff = mouse.y - lastY
            topLoader.item.dragProgress(diff)
            topLoader.item.done(diff)
        }

        Loader {
            id: topLoader
            anchors.fill: parent
            source: edgeHandlers.topEdgeHandler
        }
    }

    MouseArea {
        id: right
        anchors {
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }

        width: edgeSize

        property real lastX
        property real lastY
        property bool sendStart: false

        onPressed: {
            sendStart = true
            if (!rightLoader.item) {
                mouse.accepted = false
            } else {
                lastX = mouse.x
                lastY = mouse.y
            }
        }

        onPositionChanged: {
            var diff = lastX - mouse.x
            if (diff > width) {
                if (sendStart) {
                    sendStart = false
                    rightLoader.item.started()
                }

                rightLoader.item.dragProgress(diff)
            }
        }

        onReleased: {
            var diff = lastX - mouse.x
            rightLoader.item.dragProgress(diff)
            rightLoader.item.done(diff)
        }

        Loader {
            id: rightLoader
            anchors.fill: parent
            source: edgeHandlers.rightEdgeHandler
        }
    }

    MouseArea {
        id: bottom
        anchors {
            right: parent.right
            left: parent.left
            bottom: parent.bottom
        }
        height: edgeSize

        Loader {
            id: bottomLoader
            anchors.fill: parent
            source: edgeHandlers.bottomEdgeHandler
        }
    }

    MouseArea {
        id: left
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        width: edgeSize

        Loader {
            id: leftLoader
            anchors.fill: parent
            source: edgeHandlers.leftEdgeHandler
        }
    }

    Rectangle {
        // TODO:
        id: dimmer
        anchors.fill: parent
        color: "black"
        visible: opacity > 0
        opacity: screenLockAnimationProgress

        Behavior on opacity {
            NumberAnimation {duration: 250}
        }

        MouseArea {
            anchors.fill: parent
            enabled: dimmer.opacity > 0
        }
    }
}
