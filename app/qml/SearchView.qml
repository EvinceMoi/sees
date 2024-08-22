import QtQuick
import QtQuick.Layouts

import Source

Item {
    anchors {
        leftMargin: 10
        rightMargin: 10
    }
    // top
    Item {
        id: top

        readonly property real topHeight: 36

        height: topHeight
        width: parent.width
        anchors {
            top: parent.top
            left: parent.left
        }

        TextInput {
            id: input
            anchors {
                right: search_btn.left
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            color: "snow"
            font.pointSize: 11
            padding: 10
            maximumLength: 30
            // disable input when searching ongoing
        }

        IconButton {
            id: search_btn
            anchors {
                verticalCenter: parent.verticalCenter
                right: parent.right
            }
            size: parent.topHeight * 0.6
            icon: Icons.search
        }

        // border
        Rectangle {
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            color: "black"
        }
    }

    // bottom
    Item {
        id: bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        height: parent.height - top.height
        width: parent.width

        ListModel {
            id: lstModel
            ListElement {
                img: "https://rpic.douyucdn.cn/asrpic/240808/1870001_src_1706.avif/dy1"
                nick: "大Mu金仙丶"
                avatar: "https://apic.douyucdn.cn/upload/avatar_v3/202207/f6c31f8d33314b6795e2b8334c845317_middle.jpg"
                title: "《大Mu的随缘直播间》"
                heat: "89.2万"
            }
        }

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
