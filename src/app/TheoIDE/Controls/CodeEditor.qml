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
            color: ApplicationSettings.background
        }
        height: Math.max(background.implicitHeight, contentItem.implicitHeight)

        Repeater {
            model: root.model
            delegate: ClosableTabButton {
                required property string displayTabName
                required property var model
                onCloseTriggered: model.open = false
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
        color: ApplicationSettings.primary
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
                required property var model
                text: storedTabText
                Component.onCompleted: model.textDocument = textDocument
            }
        }
    }
}
