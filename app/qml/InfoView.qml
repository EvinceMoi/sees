import QtQuick
import QtQuick.Layouts

Item {
    id: root

    property variant info
    readonly property real fontSize: 11
    readonly property color fontColor: "snow"

    function toggle() {
        if (!info) {
            root.visible = false;
            return ;
        }
        root.visible = !root.visible;
    }

    function format_ts(ts) {
        const date = new Date(ts * 1000);
        return date.getFullYear().toString() + '-' + (date.getMonth() + 1).toString().padStart(2, '0') + '-' + date.getDate().toString().padStart(2, '0') + ' ' + date.getHours().toString().padStart(2, '0') + ':' + date.getMinutes().toString().padStart(2, '0') + ':' + date.getSeconds().toString().padStart(2, '0');
    }

    visible: false

    ColumnLayout {
        anchors.fill: parent
        spacing: 2

        Text {
            font.pointSize: fontSize
            color: fontColor
            horizontalAlignment: Text.AlignRight
            text: info ? info.type : ""
        }

        Text {
            font.pointSize: fontSize
            color: fontColor
            horizontalAlignment: Text.AlignRight
            text: info ? info.rid : ""
        }

        Text {
            font.pointSize: fontSize
            color: fontColor
            horizontalAlignment: Text.AlignRight
            text: info ? info.nick : ""
        }

        Text {
            font.pointSize: fontSize
            color: fontColor
            horizontalAlignment: Text.AlignRight
            text: info ? info.title : ""
        }

        Text {
            font.pointSize: fontSize
            color: fontColor
            horizontalAlignment: Text.AlignRight
            text: info ? format_ts(info.startTime) : ""
        }

        Item {
            Layout.fillHeight: true
        }

    }

}
