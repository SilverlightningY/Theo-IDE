import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import TheoIDE.Persistence

Item {
    id: root

    Rectangle {
        id: tabBarFrame
        color: "transparent"
        border {
            width: 1
            color: ThemeSettings.primary
        }
        height: openFilesTabBar.implicitHeight + border.width
        width: parent.width
        TabBar {
            id: openFilesTabBar
            width: parent.width
            TabButton {
                text: "File_1.theo"
                width: Math.min(implicitWidth, root.width / 2)
            }
            TabButton {
                text: "File_2_with_very_long_name.theo"
                width: Math.min(implicitWidth, root.width / 2)
            }
        }
    }

    StackLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: tabBarFrame.bottom

        currentIndex: openFilesTabBar.currentIndex

        Flickable {
            TextEdit {
                anchors.fill: parent
                wrapMode: TextEdit.NoWrap
                font {
                    family: ThemeSettings.editorFontFamily
                    pointSize: ThemeSettings.editorFontSize
                }
                text: "Content File 1"
            }
        }
        Flickable {
            TextEdit {
                anchors.fill: parent
                wrapMode: TextEdit.NoWrap
                font.family: "monospace"
                text: "Content File 2"
            }
        }
    }
}
