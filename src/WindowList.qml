import QtQuick 2.0
import Compositor 1.0

GridView {
    id: view
    anchors.fill: parent
    cellWidth: root.width / 3
    cellHeight: root.height / 3

    delegate: Rectangle {
        width: view.cellWidth
        height: view.cellHeight
        color: "transparent"

        MouseArea {
            anchors.centerIn: parent
            width: parent.width * 0.75
            height: parent.height * 0.75
            onClicked: compositor.fullScreenSurface = item
            SurfaceContainerView {
                anchors.fill: parent
                surface: item
            }
        }
    }
}