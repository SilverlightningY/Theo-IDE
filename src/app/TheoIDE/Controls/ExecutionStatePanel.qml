import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import TheoIDE.Persistence
import Qt.labs.qmlmodels

Item {
    id: root

    property bool autoStepEnabled: false
    property alias implicitWidth: toolBar.implicitWidth

    Action {
        id: stepNextAction
        text: qsTr("Step Next Breakpoint")
        icon.name: "skip_next"
    }

    Action {
        id: stepAction
        text: qsTr("Step")
        icon.name: "step"
    }

    Action {
        id: restartAction
        text: qsTr("Restart")
        icon.name: "replay"
    }

    Action {
        id: stopAction
        text: qsTr("Stop")
        icon.name: "stop"
    }

    Action {
        id: enableAutoStepAction
        text: qsTr("Enable Auto Step")
        icon.name: "autoplay"
        onTriggered: root.autoStepEnabled = true
    }

    Action {
        id: disableAutoStepAction
        text: qsTr("Disable Auto Step")
        icon.name: "autopause"
        onTriggered: root.autoStepEnabled = false
    }

    ToolBar {
        id: toolBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        leftPadding: 8
        rightPadding: 8
        background: Rectangle {
            height: 48
            color: ApplicationSettings.background
        }

        RowLayout {
            spacing: 0

            ToolButton {
                action: stepNextAction
                display: AbstractButton.IconOnly
                icon.color: ApplicationSettings.foreground
            }
            ToolButton {
                action: stepAction
                display: AbstractButton.IconOnly
                icon.color: ApplicationSettings.foreground
            }
            ToolButton {
                action: root.autoStepEnabled ? disableAutoStepAction : enableAutoStepAction
                display: AbstractButton.IconOnly
                icon.color: ApplicationSettings.foreground
            }
            ToolButton {
                action: restartAction
                display: AbstractButton.IconOnly
                icon.color: ApplicationSettings.foreground
            }
            ToolButton {
                action: stopAction
                display: AbstractButton.IconOnly
                icon.color: ApplicationSettings.foreground
            }
        }
    }

    Rectangle {
        id: toolBarSeparator
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: toolBar.bottom
        color: ApplicationSettings.primary
        height: 1
    }

    Control {
        id: variableStateTableContainer
        anchors.top: toolBarSeparator.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        padding: 10

        background: Rectangle {
            color: ApplicationSettings.primary
        }

        contentItem: TableView {
            id: variableStateTable

            columnSpacing: 1
            rowSpacing: 1
            clip: true
            boundsBehavior: Flickable.StopAtBounds

            model: TableModel {
                TableModelColumn {
                    display: "variable"
                    decoration: qsTr("Variable")
                }
                TableModelColumn {
                    display: "value"
                    decoration: qsTr("Value")
                }
                rows: [
                    {
                        "variable": "x0",
                        "value": "1"
                    },
                    {
                        "variable": "x1",
                        "value": "5"
                    },
                    {
                        "variable": "very long variable name",
                        "value": "83993323"
                    }
                ]
            }

            columnWidthProvider: column => {
                if (!isColumnLoaded(column)) {
                    return implicitColumnWidth(column);
                }
                if (column === 0) {
                    return Math.max(implicitColumnWidth(column), variableStateTable.width / 1.5);
                }
                return Math.max(implicitColumnWidth(column), variableStateTable.width / 3);
            }

            delegate: Rectangle {
                required property string display

                color: ApplicationSettings.background
                implicitHeight: tableCellText.implicitHeight
                implicitWidth: tableCellText.implicitWidth

                Text {
                    id: tableCellText
                    text: parent.display
                    anchors.fill: parent
                    rightPadding: ApplicationSettings.editorFontSize
                    leftPadding: rightPadding
                    topPadding: ApplicationSettings.editorFontSize / 2
                    bottomPadding: topPadding
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignBottom
                    font {
                        family: ApplicationSettings.editorFontFamily
                        pointSize: ApplicationSettings.editorFontSize
                    }
                    color: ApplicationSettings.foreground
                }
            }
        }
    }
}
