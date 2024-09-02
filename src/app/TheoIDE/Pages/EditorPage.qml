import QtCore
import QtQuick
import QtQuick.Controls.Material
import QtQuick.Dialogs
import QtQuick.Layouts
import TheoIDE.Controls
import TheoIDE.Persistence

Page {
    id: root

    required property EditorModel model

    header: ToolBar {
        id: headerToolBar

        height: 56
        leftPadding: 14
        rightPadding: showMoreActionMenu ? 0 : 14

        readonly property bool showMoreActionMenu: width < 480

        Action {
            id: openFileAction
            icon.name: "file_open"
            text: qsTr("Open File")
            onTriggered: fileOpenDialog.open()
        }

        Action {
            id: saveAllFilesAction
            icon.name: "save"
            text: qsTr("Save All")
            shortcut: StandardKey.Save
            onTriggered: root.model.saveAllTabs()
        }

        Action {
            id: runApplicationAction
            icon.name: "play_arrow"
            text: qsTr("Run")
            shortcut: "F12"
            onTriggered: root.model.runScript()
        }

        Action {
            id: debugApplicationAction
            icon.name: "bug_report"
            text: qsTr("Debug")
            shortcut: "F11"
            onTriggered: root.model.runScriptInDebugMode()
        }

        Action {
            id: openMoreActionsMenuAction
            icon.name: "more_vert"
            text: qsTr("Open More Actions")
            onTriggered: moreActionsMenu.open()
        }

        Action {
            id: createFileAction
            icon.name: "add"
            text: qsTr("Create File")
            onTriggered: root.model.createNewTab()
        }

        Action {
            id: openSettingsAction
            icon.name: "settings"
            text: qsTr("Open Settings")
        }

        RowLayout {
            anchors.fill: parent

            readonly property int toolTipDelay: 1000

            ToolButton {
                action: createFileAction
                display: AbstractButton.IconOnly
                visible: !headerToolBar.showMoreActionMenu
                ToolTip.visible: hovered
                ToolTip.text: text
                ToolTip.delay: parent.toolTipDelay
            }
            ToolButton {
                action: openFileAction
                display: AbstractButton.IconOnly
                visible: !headerToolBar.showMoreActionMenu
                ToolTip.visible: hovered
                ToolTip.text: text
                ToolTip.delay: parent.toolTipDelay
            }
            ToolButton {
                action: saveAllFilesAction
                display: AbstractButton.IconOnly
                visible: !headerToolBar.showMoreActionMenu
                ToolTip.visible: hovered
                ToolTip.text: text
                ToolTip.delay: parent.toolTipDelay
            }

            Label {
                Layout.fillWidth: true
                elide: Label.ElideRight
                font.pixelSize: 20
                font.weight: Font.DemiBold
                horizontalAlignment: headerToolBar.showMoreActionMenu ? Qt.AlignLeft : Qt.AlignHCenter
                text: qsTr("Theo IDE")
                verticalAlignment: Qt.AlignVCenter
            }

            // Primary Action
            ToolButton {
                action: runApplicationAction
                display: AbstractButton.IconOnly
                ToolTip.visible: hovered
                ToolTip.text: text
                ToolTip.delay: parent.toolTipDelay
            }
            // Secondary Action
            ToolButton {
                action: debugApplicationAction
                display: AbstractButton.IconOnly
                ToolTip.visible: hovered
                ToolTip.text: text
                ToolTip.delay: parent.toolTipDelay
            }

            ToolButton {
                action: openSettingsAction
                display: AbstractButton.IconOnly
                ToolTip.visible: hovered
                ToolTip.text: text
                ToolTip.delay: parent.toolTipDelay
                visible: !headerToolBar.showMoreActionMenu
            }

            ToolButton {
                id: openMoreActionsMenuButton
                action: openMoreActionsMenuAction
                display: AbstractButton.IconOnly
                visible: headerToolBar.showMoreActionMenu
            }
        }
    }

    Menu {
        id: moreActionsMenu
        x: openMoreActionsMenuButton.x
        y: openMoreActionsMenuButton.y

        MenuItem {
            action: createFileAction
        }
        MenuItem {
            action: openFileAction
        }
        MenuItem {
            action: saveAllFilesAction
        }
        MenuItem {
            action: openSettingsAction
        }
    }

    SplitView {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: statusBar.top
        anchors.top: parent.top
        orientation: width < 900 ? Qt.Vertical : Qt.Horizontal

        CodeEditor {
            id: codeEditor
            SplitView.fillWidth: true
            SplitView.fillHeight: true
            model: root.model
        }

        ExecutionStatePanel {
            id: codeExecutionStatePanel
            SplitView.preferredWidth: implicitWidth
            SplitView.preferredHeight: 200
        }
    }

    Rectangle {
        id: statusBar
        height: 24
        width: parent.width
        anchors.bottom: parent.bottom
        color: ThemeSettings.primary
    }

    FileDialog {
        id: fileOpenDialog
        nameFilters: ["Theo IDE Scripts (*.theo)", "Text Files (*.txt)", "All Files (*)"]
        acceptLabel: qsTr("Open")
        rejectLabel: qsTr("Cancel")
        Component.onCompleted: {
            const documentsLocations = StandardPaths.standardLocations(StandardPaths.DocumentsLocation);
            currentFolder = documentsLocations.length > 0 ? documentsLocations[0] : undefined;
        }
        fileMode: FileDialog.OpenFiles
        onAccepted: {
            currentFiles.forEach(fileUrl => {
                root.model.openFile(fileUrl);
            });
        }
    }
}
