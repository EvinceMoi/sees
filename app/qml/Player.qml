import MpvPlayer
import QtQuick

MpvPlayer {
    id: player

    function updateVolume(delta) {
        let vol = player.volume;
        vol += delta;
        vol = Math.min(Math.max(0, vol), 200);
        player.volume = vol;
    }

    onVolumeChanged: {
        const u = currentUrl.toString();
        if (!u)
            return ;

        volIcon.show(player.volume);
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

        property int volume

        function show(vol) {
            volume = vol;
            volIcon.visible = true;
            volTimer.restart();
        }

        visible: false
        color: 'snow'
        text: Icons.volume + ' ' + volume.toString().padStart(3, ' ')
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
