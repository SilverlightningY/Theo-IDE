pragma Singleton

import QtCore
import QtQuick
import QtQuick.Controls.Material
import TheoIDE.Persistence

Item {
    Settings {
        id: themeSettings
        category: "Theme"
        property string accentDark: "#d65d0e"
        property string backgroundDark: "#282828"
        property string foregroundDark: "#ebdbb2"
        property string primaryDark: "#1d2021"
        property string accentLight: "#d65d0e"
        property string backgroundLight: "#fbf1c7"
        property string foregroundLight: "#3c3836"
        property string primaryLight: "#ebdbb2"
        property int theme: Material.System
    }

    property alias accentDark: themeSettings.accentDark
    property alias backgroundDark: themeSettings.backgroundDark
    property alias foregroundDark: themeSettings.foregroundDark
    property alias primaryDark: themeSettings.primaryDark
    property alias accentLight: themeSettings.accentLight
    property alias backgroundLight: themeSettings.backgroundLight
    property alias foregroundLight: themeSettings.foregroundLight
    property alias primaryLight: themeSettings.primaryLight
    property alias theme: themeSettings.theme

    Material.theme: theme

    readonly property color accent: Material.theme === Material.Dark ? accentDark : accentLight
    readonly property color background: Material.theme === Material.Dark ? backgroundDark : backgroundLight
    readonly property color foreground: Material.theme === Material.Dark ? foregroundDark : foregroundLight
    readonly property color primary: Material.theme === Material.Dark ? primaryDark : primaryLight

    Settings {
        id: fontSettings
        category: "Font"
        property int uiFontSize: SystemFontConfiguraton.defaultFontSize
        property int editorFontSize: SystemFontConfiguraton.defaultMonospaceFontSize
        property string editorFontFamily: SystemFontConfiguraton.defaultMonospaceFont.family
    }

    property alias uiFontSize: fontSettings.uiFontSize
    property alias editorFontSize: fontSettings.editorFontSize
    property alias editorFontFamily: fontSettings.editorFontFamily

    Settings {
        id: generalSettings
        category: "General"
        property int maxReadFileSizeBytes: 2 * 1024 * 1024
        property int blockSizeBytes: 1024
    }

    property alias maxReadFileSizeBytes: generalSettings.maxReadFileSizeBytes
    property alias blockSizeBytes: generalSettings.blockSizeBytes
}
