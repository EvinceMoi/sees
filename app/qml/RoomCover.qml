import QtQuick
import Source

Item {
    id: root

    required property string type
    required property string rid
    required property string snapshot
    required property string nick
    required property string avatar
    required property string title
    required property string heat
    required property string category
    required property bool fav

    signal clicked
    // 关注
    signal follow
    signal unfollow
    // 特别关注
    signal like
    signal unlike

    property bool hovered: hover.hovered

    Rectangle {
        anchors.fill: parent
        anchors.margins: 2

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

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: function (event) {
                    if (event.button === Qt.RightButton) {

                        // Source.fetchMeta(root.type, root.rid)
                    }
                }
                onDoubleClicked: {
                    Source.fetchMedia(root.type, root.rid)
                }
            }

            Image {
                id: snap

                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                clip: true
                source: root.snapshot
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
