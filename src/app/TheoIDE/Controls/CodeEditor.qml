pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import TheoIDE.Persistence
import TheoIDE.Controls

Item {
    id: root

    required property EditorModel model

    TabBar {
        id: openFilesTabBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        background: Rectangle {
            implicitHeight: 48
            color: ThemeSettings.background
        }
        height: Math.max(background.implicitHeight, contentItem.implicitHeight)

        Repeater {
            model: root.model
            delegate: ClosableTabButton {
                required property string displayTabName
                required property int index
                onCloseTriggered: root.model.closeTabAt(index)
                text: displayTabName
                width: implicitWidth
            }
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

        Repeater {
            model: root.model
            delegate: TabContent {
                required property string storedTabText
                text: storedTabText
            }
        }
    }
}
