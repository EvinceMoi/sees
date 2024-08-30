import QtQuick
import QtQuick.Layouts

Item {
    id: root

    enum Pages {
        Follow,
        Search,
        Fav,
        Category,
        Settings
    }

    property int pageIndex: Panel.Pages.Follow

    Rectangle {
        id: toolbar

        readonly property real iconSize: width * 0.7

        height: parent.height
        width: 36
        color: '#242424'

        // right border
        Rectangle {
            height: parent.height
            width: 1
            anchors.top: parent.top
            anchors.right: parent.right
            color: "black"
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 8

            // followed
            IconButton {
                id: followed_btn

                size: toolbar.iconSize
                icon: Icons.followed
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.topMargin: 8
                Layout.preferredHeight: toolbar.iconSize
                highlight: pageIndex == Panel.Pages.Follow
                onClicked: {
                    pageIndex = Panel.Pages.Follow;
                }
            }

            // search
            IconButton {
                id: search_btn

                size: toolbar.iconSize
                icon: Icons.search
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.preferredHeight: toolbar.iconSize
                highlight: pageIndex == Panel.Pages.Search
                onClicked: {
                    pageIndex = Panel.Pages.Search;
                }
            }

            // fav
            IconButton {
                id: fav_btn

                size: toolbar.iconSize
                icon: Icons.fav
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.preferredHeight: toolbar.iconSize
                highlight: pageIndex == Panel.Pages.Fav
                onClicked: {
                    pageIndex = Panel.Pages.Fav;
                }
            }

            // category
            IconButton {
                id: cate_btn

                size: toolbar.iconSize
                icon: Icons.all
                Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                Layout.preferredHeight: toolbar.iconSize
                highlight: pageIndex == Panel.Pages.Category
                onClicked: {
                    pageIndex = Panel.Pages.Category;
                }
            }

            Item {
                id: strecher

                Layout.fillHeight: true
            }

            // settings
            IconButton {
                id: setttings_btn

                size: toolbar.iconSize
                icon: Icons.settings
                Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
                Layout.bottomMargin: 8
                Layout.preferredHeight: toolbar.iconSize
                highlight: pageIndex == Panel.Pages.Settings
                onClicked: {
                    pageIndex = Panel.Pages.Settings;
                }
            }

        }

    }

    Rectangle {
        id: content

        height: parent.height
        width: parent.width - toolbar.width
        color: '#262629'

        anchors {
            top: parent.top
            right: parent.right
        }

        StackLayout {
            id: stack

            anchors.fill: parent
            currentIndex: pageIndex

            FollowView {
            }

            SearchView {
            }

            Item {
                // fav

            }

            Item {
                // cate

            }

            Item {
                // setting

            }

        }

    }

}
