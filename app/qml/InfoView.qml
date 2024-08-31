import QtQuick
import QtQuick.Layouts

Item {
    id: root

    property string typeName
    property string rid
    property string title
    property string startTime
    // inner
    readonly property real fontSize: 11
    readonly property color fontColor: "snow"

    ColumnLayout {
        anchors.fill: parent
        spacing: 2

        Text {
            font.pointSize: fontSize
            color: fontColor
            horizontalAlignment: Text.AlignRight
            text: typeName
        }

        Text {
            font.pointSize: fontSize
            color: fontColor
            horizontalAlignment: Text.AlignRight
            text: rid
        }

        Text {
            font.pointSize: fontSize
            color: fontColor
            horizontalAlignment: Text.AlignRight
            text: title
        }

        Text {
            font.pointSize: fontSize
            color: fontColor
            horizontalAlignment: Text.AlignRight
            text: startTime
        }

        Item {
            Layout.fillHeight: true
        }

    }

}
