import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import TheoIDE.Persistence

Item {
    id: root

    TabBar {
        id: openFilesTabBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        ClosableTabButton {
            text: "File_1.theo"
            width: Math.min(implicitWidth, root.width / 2)
            onCloseTriggered: console.log("Close file 1")
        }
        ClosableTabButton {
            text: "File_2_with_very_long_name.theo"
            width: Math.min(implicitWidth, root.width / 2)
            onCloseTriggered: console.log("Close file 2")
        }
    }

    Rectangle {
        id: tabBarEditorSeparator
        anchors.top: openFilesTabBar.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        height: 1
        color: ThemeSettings.primary
    }

    StackLayout {
        id: tabContentStack
        anchors.top: tabBarEditorSeparator.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        currentIndex: openFilesTabBar.currentIndex

        TabContent {
        }

        Flickable {
            TextEdit {
                anchors.fill: parent
                wrapMode: TextEdit.NoWrap
                font {
                    family: ThemeSettings.editorFontFamily
                    pointSize: ThemeSettings.editorFontSize
                }
                text: "Content File 2"
            }
        }
    }
}
