import MpvPlayer
import QtCore
import QtQuick
import QtQuick.Controls
import Source

ApplicationWindow {
    id: root

    property int refreshTimeout: 180 // in seconds
    property bool showInfo: false

    width: 800
    height: 450
    visible: true
    Component.onCompleted: {
        Source.refresh(0);
    }

    Shortcuts {
    }

    Player {
        id: player

        anchors.fill: parent
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
            // drawer.close()

            anchors.fill: parent
            hoverEnabled: true
            onExited: {
            }

            Panel {
                id: panel

                anchors.fill: parent
            }

        }

    }

}
