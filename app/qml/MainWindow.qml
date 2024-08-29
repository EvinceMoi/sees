import QtCore
import QtQuick
import QtQuick.Controls

import MpvPlayer
import Source

ApplicationWindow {
    id: root
    width: 800
    height: 480
    visible: true

    property int refreshTimeout: 180 // in seconds

    Shortcut {
        sequence: "q"
        onActivated: Qt.quit()
    }
    Shortcut {
        sequence: "m"
        onActivated: player.setPropertyAsync(MpvProps.Mute,
                                             !player.getProperty(MpvProps.Mute))
    }

    MpvPlayer {
        id: player
        anchors.fill: parent
    }

    Timer {
        interval: refreshTimeout * 1000
        running: true
        repeat: true
        triggeredOnStart: false
        onTriggered: {
            Source.refresh(refreshTimeout)
        }
    }

    Component.onCompleted: {
        Source.refresh(0)
    }

    Connections {
        id: sm
        target: Source
        function onMediaInfoFetched(video, audio, subtitle) {
            player.loadMedia(video, audio, subtitle)
        }
    }

    DropArea {
        id: dropArea
        anchors.fill: parent
        onEntered: function (drag) {
            drag.accept(Qt.LinkAction)
        }
        onDropped: function (drop) {
            if (drop.hasUrls)
                player.loadFile(drop.urls[0])
        }
    }

    MouseArea {
        anchors.left: parent.left
        anchors.top: parent.top
        width: 0.1 * parent.width
        height: 0.8 * parent.height // the rest should be to video controller

        hoverEnabled: true

        onEntered: {
            drawer.open()
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
            onExited: {

                // drawer.close()
            }

            Panel {
                id: panel
                anchors.fill: parent
            }
        }
    }
}
