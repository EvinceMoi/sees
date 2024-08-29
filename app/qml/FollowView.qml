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
        onTextChanged: function (text) {
            Source.filterFollows(text)
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

            anchors.fill: parent
            cellWidth: parent.width / 3
            cellHeight: cellWidth * 0.7
            clip: true

            model: Source.followsModel()
            delegate: RoomCover {
                id: rc
                width: gv.cellWidth
                height: gv.cellHeight

                property real iconSize: Math.max(width * 0.12, 14)

                ColumnLayout {
                    visible: rc.hovered || follow.hovered || fav.hovered
                    height: parent.height
                    width: height * 0.2
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.margins: 2

                    IconButton {
                        id: follow
                        size: parent.width - 4
                        icon: Icons.unfollow
                        round: true
                        visible: !rc.fav

                        Layout.alignment: Qt.AlignHCenter

                        onClicked: {
                            Source.follow(rc.type, rc.rid, false)
                        }
                    }
                    IconButton {
                        id: fav
                        size: parent.width - 4
                        icon: rc.fav ? Icons.like : Icons.dislike
                        round: true

                        Layout.alignment: Qt.AlignHCenter

                        onClicked: {
                            Source.fav(rc.type, rc.rid, !rc.fav)
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
