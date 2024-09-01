import MpvPlayer
import QtQuick

MpvPlayer {
    id: player

    property int volumeEventCount: 0 // ignore first change

    function ignoreVolumeEvent() {
        return ++volumeEventCount < 3;
    }

    function updateVolume(delta) {
        let vol = player.volume;
        vol += delta;
        vol = Math.min(Math.max(0, vol), 200);
        player.volume = vol;
    }

    onVolumeChanged: {
        volIcon.volume = player.volume;
        if (!ignoreVolumeEvent())
            volIcon.show();

    }
    onMuteChanged: {
        volIcon.mute = player.mute;
        if (!ignoreVolumeEvent())
            volIcon.show();

    }

    Shortcut {
        sequence: "m"
        onActivated: player.toggleMute()
    }

    Shortcut {
        sequence: "9"
        onActivated: updateVolume(-2)
    }

    Shortcut {
        sequence: "0"
        onActivated: updateVolume(2)
    }

    Shortcut {
        sequence: "Shift+i"
        onActivated: {
            player.showInfo();
        }
    }

    Text {
        id: volIcon

        property int volume // cache value
        property bool mute: false

        function show() {
            volIcon.visible = true;
            volTimer.restart();
        }

        function volText() {
            if (mute)
                return Icons.mute;
            else
                return Icons.volume + ' ' + volume.toString().padStart(3, ' ');
        }

        visible: false
        color: 'snow'
        text: volText()
        font.pointSize: 15
        horizontalAlignment: Text.AlignRight

        anchors {
            right: parent.right
            top: parent.top
            margins: 8
        }

        Timer {
            id: volTimer

            interval: 1500
            onTriggered: volIcon.visible = false
        }

    }

}
