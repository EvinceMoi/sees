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

        height: 64
        width: parent.width

        property string selectedType

        anchors {
            top: parent.top
            left: parent.left
        }

        ButtonGroup {
            id: buttonGroup
        }

        Row {
            id: row
            height: 32
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
        }

        Component.onCompleted: {
            const ts = Source.getTypes();
            ts.forEach((it) => {
                const name = Source.getNameByType(it);
                const qmlButton = `
                    import QtQuick.Controls;
                    TabButton {
                        text: "${name}"
                    }
                `;
                const b = Qt.createQmlObject(qmlButton, row);
                buttonGroup.addButton(b);
                b.clicked.connect(function() {
                    top.selectedType = it;
                });
            });
            buttonGroup.buttons[0].checked = true;
            buttonGroup.buttons[0].clicked();
        }

        SearchBox {
            id: search

            onSearch: function(text) {
                let type = top.selectedType;
                Source.search(type, text);
            }

            anchors {
                right: parent.right
                left: parent.left
                bottom: parent.bottom
                top: row.bottom
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
