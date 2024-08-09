import QtQuick

Item {

    // radius: 4
    id: root

    required property string img
    required property string nick
    required property string avatar
    required property string title
    required property string heat

    signal clicked
    signal followed
    // 关注
    signal unfollowed
    signal liked
    // 特别关注
    signal unliked

    Rectangle {
        width: parent.width - 8
        height: parent.height - 8
        anchors.centerIn: parent

        color: "transparent"
        border.color: hover.hovered ? "green" : "transparent"
        border.width: 1

        HoverHandler {
            id: hover
        }

        Item {
            id: top

            height: parent.height * 0.8
            width: parent.width
            anchors.top: parent.top
            anchors.left: parent.left

            Image {
                id: snap

                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                clip: true
                source: root.img
            }
        }

        Item {
            id: bottom

            height: parent.height - top.height
            width: parent.width
            anchors.left: parent.left
            anchors.bottom: parent.bottom

            // avartar
            Image {
                height: parent.height * 0.8
                width: parent.height * 0.8
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.margins: 2
                fillMode: Image.PreserveAspectFit
                clip: true
                source: root.avatar
            }

            // info
            Item {
                height: parent.height
                width: parent.width - parent.height
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right

                // anchors.margins: 2
                Item {
                    height: parent.height / 2
                    width: parent.width
                    anchors.right: parent.right
                    anchors.top: parent.top

                    Text {
                        anchors.fill: parent
                        color: 'snow'
                        text: root.title
                        font.pointSize: Math.max(height * 0.5, 6)
                        elide: Text.ElideRight
                    }
                }

                Item {
                    height: parent.height / 2
                    width: parent.width
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom

                    Text {
                        anchors.fill: parent
                        color: 'snow'
                        text: root.nick
                        font.pointSize: Math.max(height * 0.5, 6)
                    }
                }
            }
        }
    }
}
