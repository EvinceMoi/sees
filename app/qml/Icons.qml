import QtQuick
pragma Singleton

QtObject {
    id: icons

    readonly property string settings: ''
    readonly property string search: ''
    readonly property string clear: ''
    readonly property string fav: ''
    readonly property string like: ''
    readonly property string dislike: '♥'
    readonly property string follow: ''
    readonly property string unfollow: ''
    readonly property string followed: ''
    readonly property string refresh: ''
    readonly property string mute: ''
    readonly property string volume: ''
    readonly property string subtitle: '󰨗'
    readonly property string fullscreen: '󰊓'
    readonly property string fullscreenExit: '󰊔'
    readonly property string pause: '󰏤'
    readonly property string play: '󰐊'
    readonly property string stop: '󰓛'
    readonly property string hot: ''
    readonly property string flame: ''
    readonly property string check: ''
    readonly property string video: '󰕧'
    readonly property string videoOff: '󰕨'
    readonly property string cast: '󰄘'
    readonly property string link: '󰌷'
    readonly property string category: '󰋁' // '󰇛'
    readonly property string all: '󰨝' // 󰕮
    readonly property string picture: '󰋩'
    readonly property string imageOff: '󰠫'
    readonly property string avatar: ''
    readonly property string unavailable: '󱞃'
    readonly property string offline: ''
}
