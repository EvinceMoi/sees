import MpvPlayer
import QtCore
import QtQuick
import QtQuick.Controls
import Source

ApplicationWindow {
    id: root

    property int refreshTimeout: 180 // in seconds
    property bool showInfo: false

    function toggleFullScreen() {
        if (root.visibility === Window.FullScreen)
            root.visibility = Window.Windowed;
        else
            root.visibility = Window.FullScreen;
    }

    function toggleStayOnTop() {
        // probably not working in some wayland sessions
        root.flags ^= Qt.WindowStaysOnTopHint;
    }

    function toggleFrameless() {
        root.flags ^= Qt.FramelessWindowHint;
    }

    width: 800
    height: 450
    flags: Qt.Window | Qt.FramelessWindowHint
    visible: true
    Component.onCompleted: {
        Source.refresh(0);
    }

    Settings {
        property alias x: root.x
        property alias y: root.y
        property alias width: root.width
        property alias height: root.height
    }

    Shortcuts {
        window: root
        infoView: iv
        drawer: drawer
    }

    Player {
        id: player

        anchors.fill: parent
    }

    InfoView {
        id: iv
    }

    Timer {
        interval: refreshTimeout * 1000
        running: true
        repeat: true
        triggeredOnStart: false
        onTriggered: {
            Source.refresh(refreshTimeout);
        }
    }

    Connections {
        id: sm

        function onMediaInfoFetched(video, audio, subtitle) {
            player.loadMedia(video, audio, subtitle);
            // update info
            iv.info = Source.roomInfo();
        }

        target: Source
    }

    DropArea {
        id: dropArea

        anchors.fill: parent
        onEntered: function(drag) {
            drag.accept(Qt.LinkAction);
        }
        onDropped: function(drop) {
            if (drop.hasUrls)
                player.loadFile(drop.urls[0]);

        }
    }

    MouseArea {
        anchors.fill: parent
        onPressed: {
            root.startSystemMove();
        }
    }

    MouseArea {
        anchors.left: parent.left
        anchors.top: parent.top
        width: 0.1 * parent.width
        height: 0.6 * parent.height // the rest should be to video controller
        hoverEnabled: true
        onEntered: {
            drawer.open();
        }
    }

    Drawer {
        id: drawer

        width: 0.7 * parent.width
        height: parent.height
        dragMargin: 100
        dim: true

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true

            Panel {
                id: panel

                anchors.fill: parent
            }

        }

    }

}
