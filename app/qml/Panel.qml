import QtQuick

Item {
    id: panel

    Rectangle {
        id: toolbar
        height: parent.height
        width: 36
        color: '#242424'

        readonly property real vpadding: 10
        readonly property real iconSize: width * 0.7

        // right border
        Rectangle {
            height: parent.height
            width: 1
            anchors.top: parent.top
            anchors.right: parent.right
            color: "black"
        }

        // search
        IconButton {
            id: search_btn
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: parent.vpadding
            size: parent.iconSize
            icon: Icons.search
            onClicked: {
                console.log('search clicked')
            }
        }

        // followed
        IconButton {
            id: followed_btn
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: search_btn.bottom
            anchors.topMargin: parent.vpadding
            size: parent.iconSize
            icon: Icons.followed
            onClicked: {
                console.log('search clicked')
            }
        }

        // fav
        IconButton {
            id: fav_btn
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: followed_btn.bottom
            anchors.topMargin: parent.vpadding
            size: parent.iconSize
            icon: Icons.fav
            onClicked: {
                console.log('fav clicked')
            }
        }

        // all
        IconButton {
            id: all_btn
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: fav_btn.bottom
            anchors.topMargin: parent.vpadding
            size: parent.iconSize
            icon: Icons.all
            onClicked: {
                console.log('all clicked')
            }
        }

        // settings
        IconButton {
            id: setttings_btn
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.vpadding
            size: parent.iconSize
            icon: Icons.settings
            onClicked: {
                console.log('settings clicked')
            }
        }
    }

    Rectangle {
        id: content
        anchors.top: parent.top
        anchors.right: parent.right

        height: parent.height
        width: parent.width - toolbar.width
        color: '#262629'

        FollowView {
            anchors.fill: parent
        }
    }
}
