import QtQuick 2.0
import Compositor 1.0

MouseArea {
    id: mouse

    property alias source: loader.source
    property alias direction: loader.direction
    property alias item: loader.item

    property real lastX
    property real lastY
    property bool sendStart

    onPressed: {
        sendStart = true
        if (!loader.item) {
            mouse.accepted = false
        } else {
            lastX = mouse.x
            lastY = mouse.y
        }
    }

    Loader {
        id: loader
        property int direction
        anchors.fill: parent
        source: edgeHandlers.topEdgeHandler
    }
}