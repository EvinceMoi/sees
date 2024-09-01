import MpvPlayer
import QtQuick

Item {
    property InfoView infoView

    Shortcut {
        sequence: "q"
        onActivated: Qt.quit()
    }

    Shortcut {
        sequence: "i"
        onActivated: {
            infoView.toggle();
        }
    }

}
