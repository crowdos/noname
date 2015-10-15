import QtQuick 2.0
import QtCompositor 1.0
import Compositor 1.0

QtObject {
    property real dimension

    function started() {
        closeAnimationProgress = 0
        closeAnimationRunning = true
        dimension = direction == Direction.TopToBottom || direction == Direction.BottomToTop ? root.height : root.width
    }

    function dragProgress(diff) {
        closeAnimationProgress = 1 - ((dimension - (2 * diff)) / dimension)
    }

    function done(diff) {
        if (diff > 100) {
            closeAnimationProgress = 1
            compositor.fullScreenSurface.destroySurface()
        } else {
            closeAnimationProgress = 0
        }

        closeAnimationRunning = false
    }
}
