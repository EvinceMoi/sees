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
    required property bool live
    required property bool record
    property bool hovered: hover.hovered

    signal clicked()
    // 关注
    signal follow()
    signal unfollow()
    // 特别关注
    signal like()
    signal unlike()

    Rectangle {
        anchors.fill: parent
        anchors.margins: 4
        color: hover.hovered ? palette.active.highlight : "transparent"

        HoverHandler {
            id: hover
        }

        Item {
            id: top

            height: parent.height * 0.8
            width: parent.width

            anchors {
                top: parent.top
                left: parent.left
            }

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: function(event) {
                    if (event.button === Qt.RightButton) {
                    }
                }
                onDoubleClicked: {
                    Source.fetchMedia(root.type, root.rid);
                    Source.selectRoom(root.type, root.rid);
                }
            }

            Image {
                id: snap

                anchors.fill: parent
                fillMode: Image.PreserveAspectCrop
                clip: false
                source: root.snapshot
            }

            // heat icon
            Rectangle {
                color: 'black'
                opacity: 0.7
                height: 16
                width: height * 4

                anchors {
                    bottom: parent.bottom
                    bottomMargin: 2
                    right: parent.right
                    rightMargin: 2
                }

                Text {
                    function formatHeat(h) {
                        if (h > 10000) {
                            let n = Math.floor(h / 1000) / 10;
                            return `${n}万`;
                        }
                        return h;
                    }

                    anchors.fill: parent
                    anchors.rightMargin: 4
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    color: 'white'
                    text: Icons.hot + ' ' + formatHeat(root.heat)
                }

            }

            // live icon
            Rectangle {
                visible: !root.live || root.record
                color: 'black'
                opacity: 0.7
                height: 16
                width: height

                anchors {
                    top: parent.top
                    topMargin: 2
                    left: parent.left
                    leftMargin: 2
                }

                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: 'white'
                    text: root.record ? Icons.video : Icons.offline
                }

            }

        }

        Item {
            id: bottom

            height: parent.height - top.height
            width: parent.width

            anchors {
                left: parent.left
                bottom: parent.bottom
            }

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
