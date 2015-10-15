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

    EdgeHandler {
        id: top
        anchors {
            top: parent.top
            right: parent.right
            left: parent.left
        }

        height: edgeSize
        direction: Direction.TopToBottom
        source: edgeHandlers.topEdgeHandler
        onPositionChanged: {
            var diff = mouse.y - lastY
            if (diff > height) {
                if (sendStart) {
                    sendStart = false
                    item.started()
                }

                item.dragProgress(diff)
            }
        }

        onReleased: {
            var diff = mouse.y - lastY
            item.dragProgress(diff)
            item.done(diff)
        }
    }

    EdgeHandler {
        id: right
        anchors {
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }

        width: edgeSize
        direction: Direction.RightToLeft
        source: edgeHandlers.rightEdgeHandler
        onPositionChanged: {
            var diff = lastX - mouse.x
            if (diff > width) {
                if (sendStart) {
                    sendStart = false
                    item.started()
                }

                item.dragProgress(diff)
            }
        }

        onReleased: {
            var diff = lastX - mouse.x
            item.dragProgress(diff)
            item.done(diff)
        }
    }

    EdgeHandler {
        id: bottom
        anchors {
            right: parent.right
            left: parent.left
            bottom: parent.bottom
        }
        height: edgeSize
        direction: Direction.BottomToTop
    }

    EdgeHandler {
        id: left
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        width: edgeSize
        direction: Direction.LeftToRight
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
            onDoubleClicked: screenLockAnimationProgress = 0
        }
    }
}
