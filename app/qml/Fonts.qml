pragma Singleton

import QtQuick

Item {
	id: fonts

	readonly property FontLoader fontCaskaydiaCove: FontLoader {
		source: "./res/font/CaskaydiaCove.ttf"
	}

	readonly property string icons: fonts.fontCaskaydiaCove.name
}
