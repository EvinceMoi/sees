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
        height: 32
        width: parent.width
        onSearch: function (text) {
            Source.search("douyu", text)
        }
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

            model: Source.searchModel()
            delegate: RoomCover {
                id: rc
                width: gv.w
                height: gv.h

                property real iconSize: width * 0.16

                ColumnLayout {
                    visible: rc.hovered || b1.hovered
                    height: parent.height - 4
                    width: rc.iconSize + 4
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 2

                    IconButton {
                        id: b1
                        size: rc.iconSize
                        icon: Icons.follow
                        round: true

                        Layout.alignment: Qt.AlignHCenter

                        onClicked: {
                            Source.follow(rc.type, rc.rid, true)
                        }
                    }
                    Item {
                        Layout.fillHeight: true
                    }
                }
            }
        }
    }
}
