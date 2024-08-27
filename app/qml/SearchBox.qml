import QtQuick
import QtQuick.Layouts

import Source

Item {
    id: root

    signal search(string text)
    property real iconSize: height * 0.6

    RowLayout {
        anchors.fill: parent
        spacing: 2

        TextInput {
            id: input
            Layout.fillWidth: true
            color: "snow"
            font.pointSize: 11
            maximumLength: 30

            Layout.leftMargin: 4

            // disable input when searching ongoing
            onAccepted: {
                if (input.length > 0) {
                    root.search(input.text)
                }
            }
        }
        IconButton {
            id: clear_btn
            size: iconSize
            icon: Icons.clear
            visible: input.length > 0

            Layout.preferredWidth: iconSize
            Layout.minimumWidth: iconSize

            onClicked: {
                input.clear()
            }
        }
        IconButton {
            id: search_btn
            size: iconSize
            icon: Icons.search

            Layout.rightMargin: 4
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredWidth: iconSize
            Layout.minimumWidth: iconSize

            onClicked: {
                if (input.length > 0) {
                    root.search(input.text)
                }
            }
        }
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
