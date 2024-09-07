import MpvPlayer
import QtQuick
import QtQuick.Controls

Item {
    property ApplicationWindow window
    property InfoView infoView
    property Drawer drawer

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

    Shortcut {
        sequence: "f"
        onActivated: window.toggleFullScreen()
    }

    Shortcut {
        sequence: "alt+f"
        onActivated: window.toggleFrameless()
    }

    Shortcut {
        sequence: "t"
        onActivated: window.toggleStayOnTop()
    }

    Shortcut {
        context: Qt.ApplicationShortcut
        sequence: "tab"
        onActivated: {
            if (drawer.opened)
                drawer.close();
            else
                drawer.open();
        }
    }

}
