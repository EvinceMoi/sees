import MpvPlayer
import QtQuick

Item {
    property MpvPlayer player

    function updateVolume(delta) {
        let vol = player.getProperty(MpvProps.Volume);
        vol += delta;
        vol = Math.min(Math.max(0, vol), 100);
        player.setProperty(MpvProps.Volume, vol);
    }

    Shortcut {
        sequence: "q"
        onActivated: Qt.quit()
    }

    Shortcut {
        sequence: "i"
        onActivated: {
            console.log('show room info');
        }
    }

}
