import QtQuick 2.0
import Compositor 1.0
import org.nemomobile.configuration 1.0

Rectangle {
    ScreenBlank {
        id: screenBlank
        blank: screenLocked
    }

    ConfigurationGroup {
        id: settings
        path: "/apps/noname"
        property real edgeSize
        property string emptyTopEdgeHandler
        property string emptyRightEdgeHandler
        property string emptyBottomEdgeHandler
        property string emptyLeftEdgeHandler
        property string fullTopEdgeHandler
        property string fullRightEdgeHandler
        property string fullBottomEdgeHandler
        property string fullLeftEdgeHandler
    }

    BackgroundImage {
        anchors.centerIn: parent
        size: Math.max(parent.width, parent.height)
        // TODO: Configurable.
        source: "/usr/share/backgrounds/crowdos/76682-dark_sunset_wide_2.jpg"
    }

    id: root
    width: 200
    height: 400
    color: "white"
    focus: true

    readonly property bool animationRunning: minimizeAnimationRunning || screenLockAnimationRunning || closeAnimationRunning

    QtObject {
        id: edgeHandlers
        property string topEdgeHandler: compositor.fullScreenSurface ? settings.fullTopEdgeHandler : settings.emptyTopEdgeHandler
        property string rightEdgeHandler: compositor.fullScreenSurface ? settings.fullRightEdgeHandler : settings.emptyRightEdgeHandler
        property string bottomEdgeHandler: compositor.fullScreenSurface ? settings.fullBottomEdgeHandler : settings.emptyBottomEdgeHandler
        property string leftEdgeHandler: compositor.fullScreenSurface ? settings.fullLeftEdgeHandler : settings.emptyLeftEdgeHandler
    }

    // Those properties are for edge handlers:
//    property Item fullScreenSurface: compositor.fullScreenSurface
    property real minimizeAnimationProgress: 0
    property bool minimizeAnimationRunning: false

//    property Item screenLocker: dimmer
    property real screenLockAnimationProgress: 0
    property bool screenLockAnimationRunning: false
    property bool screenLocked: false

    property real closeAnimationProgress: 0
    property bool closeAnimationRunning: false

    WindowList {
        model: compositor.windowList
    }

    FullScreenSurfaceView {
        id: fs
        anchors.fill: parent
        visible: opacity > 0 && surface != null
        opacity: minimizeAnimationRunning ? 1 - minimizeAnimationProgress : compositor.fullScreenSurface == null ? 0 : 1

        scale: closeAnimationRunning ? Math.max(0.3, 1 - closeAnimationProgress) : 1
        enabled: compositor.fullScreenSurface != null
        focus: enabled
        surface: compositor.fullScreenSurface
        compositorObject: compositor
        Behavior on opacity {
            enabled: !minimizeAnimationRunning
            NumberAnimation {duration: 250}
        }

        Behavior on scale {
            enabled: !minimizeAnimationRunning
            NumberAnimation {duration: 50}
        }
    }

    function windowAdded(window) {
        if (!animationRunning) {
            compositor.fullScreenSurface = window
        }
    }

    function windowRemoved(window) {
    }

    function lock() {
        screenLockAnimationProgress = 1
        screenLocked = true
    }

    function unlock() {
        screenLockAnimationProgress = 0
        screenLocked = false
    }

    EdgeHandler {
        id: top
        anchors {
            top: parent.top
            right: parent.right
            left: parent.left
        }

        height: settings.edgeSize
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

        width: settings.edgeSize
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
        height: settings.edgeSize
        direction: Direction.BottomToTop
    }

    EdgeHandler {
        id: left
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        width: settings.edgeSize
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
            enabled: screenBlank.blank
            onDoubleClicked: unlock()
        }
    }

    PowerKey {
        property bool pressed: false

        onStateChanged: {
            // This is a workaround for hwhal sending the current state in the beginning
            // If we do not receive a pressed state then we just eat the released event.
            if (state == PowerKey.Pressed) {
                pressed = true
            } else if (state == PowerKey.Released && pressed) {
                if (screenBlank.blank) {
                    unlock()
                } else {
                    lock()
                }
            }
        }
    }
}
