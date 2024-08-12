import QtQuick.Controls
import QtQuick
import TheoIDE.Persistence

Control {
    id: root
    required property int lineNumber
    required property bool breakPointSet

    implicitWidth: Math.max(leftInset + implicitBackgroundWidth + rightInset, leftPadding + implicitContentWidth + rightPadding)
    implicitHeight: Math.max(topInset + implicitBackgroundHeight + bottomInset, topPadding + implicitContentHeight + bottomPadding)

    background: Rectangle {
        color: root.breakPointSet ? ThemeSettings.accent : "transparent"
        radius: height / 2
    }
    contentItem: Text {
        text: root.lineNumber
        leftPadding: height / 2
        rightPadding: leftPadding
        color: root.breakPointSet ? ThemeSettings.background : ThemeSettings.foreground
        font {
            pointSize: ThemeSettings.editorFontSize
            family: ThemeSettings.editorFontFamily
        }
    }
}
