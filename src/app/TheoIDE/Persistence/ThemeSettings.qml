pragma Singleton

import QtCore
import QtQuick
import QtQuick.Controls.Material

Settings {
    category: "Theme"
    property color accentDark: "#d65d0e"
    property color backgroundDark: "#282828"
    property color foregroundDark: "#ebdbb2"
    property color primaryDark: "#1d2021"
    property color accentLight: "#d65d0e"
    property color backgroundLight: "#fbf1c7"
    property color foregroundLight: "#3c3836"
    property color primaryLight: "#ebdbb2"
    property int uiFontSize: 9
    property int editorFontSize: 9
    property int theme: Material.System
    property string editorFontFamily: "monospace"
    readonly property color accent: Material.theme === Material.Dark ? accentDark : accentLight
    readonly property color background: Material.theme === Material.Dark ? backgroundDark : backgroundLight
    readonly property color foreground: Material.theme === Material.Dark ? foregroundDark : foregroundLight
    readonly property color primary: Material.theme === Material.Dark ? primaryDark : primaryLight
}
