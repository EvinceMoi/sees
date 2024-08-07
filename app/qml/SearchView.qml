import QtQuick
import QtQuick.Layouts

Item {
	anchors {
		leftMargin: 10
		rightMargin: 10
	}
	// top
	Item {
		id: top

        readonly property real topHeight: 36

		height: topHeight
		width: parent.width
		anchors {
			top: parent.top
			left: parent.left
		}

		TextInput {
			id: input
			anchors {
				right: search_btn.left
				left: parent.left
				top: parent.top
				bottom: parent.bottom
				verticalCenter: parent.verticalCenter
			}
			color: "snow"
			font.pointSize: 11
			padding: 10
			maximumLength: 30
			// disable input when searching ongoing
		}

		IconButton {
			id: search_btn
			anchors {
				verticalCenter: parent.verticalCenter
				// top: parent.top
				right: parent.right
			}
			size: parent.topHeight * 0.6
			icon: Icons.search
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

	// bottom
	Item {
		id: bottom
		anchors.bottom: parent.bottom
		anchors.left: parent.left

		height: parent.height - top.height
		width: parent.width
	}
}
