import QtQuick

Item {
    id: root

    signal clicked
    property real size: 32
    required property string icon

    width: size
    height: size

    HoverHandler {
        id: hover
    }
    TapHandler {
        id: tap
        onTapped: root.clicked()
    }

    Rectangle {
        id: bg
        anchors.fill: parent
        radius: 2
        color: hover.hovered ? "darkgray" : "transparent"

        Text {
            anchors.centerIn: parent
            text: root.icon
            font.family: Fonts.icons
            font.pointSize: root.size * 0.54
            color: '#f0f0f0'
        }
    }
}
