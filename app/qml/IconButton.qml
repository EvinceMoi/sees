import QtQuick

Item {
    id: root

    signal clicked
    property real size: 32
    property bool highlight: false
    property bool round: false
    required property string icon
    property bool hovered: hover.hovered

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
        radius: round ? root.size / 2 : 2
        color: (hover.hovered || highlight) ? "darkgray" : "transparent"

        Text {
            anchors.centerIn: parent
            text: root.icon
            font.family: Fonts.icons
            fontSizeMode: Text.Fit
            color: '#f0f0f0'
        }
    }
}
