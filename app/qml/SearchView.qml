import QtQuick
import QtQuick.Controls
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

        height: 32
        width: parent.width

        anchors {
            top: parent.top
            left: parent.left
        }

        ComboBox {
            id: combo

            width: 80
            implicitContentWidthPolicy: ComboBox.ContentItemImplicitWidth
            flat: true
            textRole: "name"
            valueRole: "type"

            anchors {
                top: parent.top
                left: parent.left
                bottom: parent.bottom
            }

            model: ListModel {
                id: lm

                Component.onCompleted: {
                    lm.clear();
                    const ts = Source.getTypes();
                    ts.forEach((it) => {
                        lm.append({
                            "type": it,
                            "name": Source.getNameByType(it)
                        });
                    });
                    combo.currentIndex = 0;
                }
            }

        }

        SearchBox {
            id: search

            height: parent.height
            onSearch: function(text) {
                let type = combo.currentValue;
                Source.search(type, text);
            }

            anchors {
                right: parent.right
                left: combo.right
                verticalCenter: parent.verticalCenter
            }

        }

    }

    // bottom
    Item {
        id: bottom

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        height: parent.height - top.height
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

                property real iconSize: width * 0.16

                width: gv.w
                height: gv.h

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
                            Source.follow(rc.type, rc.rid, true);
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
