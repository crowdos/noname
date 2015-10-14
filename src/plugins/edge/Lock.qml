import QtQuick 2.0
import QtCompositor 1.0
import Compositor 1.0

QtObject {
    function started() {
        screenLockAnimationProgress = 0
        screenLockAnimationRunning = true
    }

    function dragProgress(diff) {
        if (diff > 100) {
            screenLockAnimationProgress = 1
        } else {
            screenLockAnimationProgress = diff / 100
        }
    }

    function done(diff) {
        if (diff > 100) {
            screenLockAnimationProgress = 1
        } else {
            screenLockAnimationProgress = 0
        }

        screenLockAnimationRunning = false
    }
}
