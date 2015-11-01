import QtQuick 2.0
import Compositor 1.0

QtObject {
    function started() {
        minimizeAnimationProgress = 0
        minimizeAnimationRunning = true
    }

    function dragProgress(diff) {
        if (diff > 100) {
            minimizeAnimationProgress = 1
        } else {
            minimizeAnimationProgress = diff / 100
        }
    }

    function done(diff) {
        if (diff > 100) {
            compositor.fullScreenSurface = null
        }

        minimizeAnimationRunning = false
    }
}
