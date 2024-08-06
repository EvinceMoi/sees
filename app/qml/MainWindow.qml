import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

import MpvPlayer

ApplicationWindow {
	id: root
	width: 800
	height: 640
	visible: true

	Shortcut {
		sequence: "q"
		onActivated: Qt.quit()
	}
	Shortcut {
		sequence: "m"
		onActivated: player.setPropertyAsync(MpvProps.Mute, !player.getProperty(MpvProps.Mute))
	}

	MpvPlayer {
		id: player
		anchors.fill: parent
	}

	DropArea {
		id: dropArea
		anchors.fill: parent
		onEntered: function(drag) {
			drag.accept(Qt.LinkAction);
		}
		onDropped: function(drop) {
			if (drop.hasUrls)
				player.loadFile(drop.urls[0]);
		}
	}

	FileDialog {
		id: fileDialog

		currentFolder: StandardPaths.standardLocations(StandardPaths.DownloadLocation)[0]
		onAccepted: {
			player.loadFile(selectedFile)
		}
	}
}
