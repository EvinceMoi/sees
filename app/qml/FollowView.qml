import QtQuick
import QtQuick.Layouts

import Source

Item {
    anchors {
        leftMargin: 10
        rightMargin: 10
    }
    // top
    SearchBox {
        id: search
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }
        height: 36
        width: parent.width
    }

    // bottom
    Item {
        id: bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        height: parent.height - search.height
        width: parent.width

        GridView {
            id: gv

            readonly property real w: parent.width / 3
            readonly property real h: w * 0.7

            anchors.fill: parent
            cellWidth: w
            cellHeight: h
            clip: true

            model: Source.follows()
            delegate: RoomCover {
                width: gv.w
                height: gv.h
            }
        }
    }
}
