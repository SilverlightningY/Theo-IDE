import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import TheoIDE.Persistence
import Qt.labs.qmlmodels

Item {
    id: root

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

    Rectangle {
        id: toolBarBorder
        color: "transparent"
        border {
            width: 1
            color: ThemeSettings.primary
        }
        width: parent.width
        height: toolBar.height + border.width

        Pane {
            id: toolBar
            spacing: 8
            implicitHeight: 48
            topPadding: 0
            bottomPadding: 0
            leftPadding: 8
            rightPadding: 8
            RowLayout {
                anchors.fill: parent

                Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                Layout.leftMargin: 16
                Layout.rightMargin: 16
                spacing: 0

                ToolButton {
                    action: stepNextAction
                    display: AbstractButton.IconOnly
                }
                ToolButton {
                    action: stepAction
                    display: AbstractButton.IconOnly
                }
                ToolButton {
                    action: restartAction
                    display: AbstractButton.IconOnly
                }
                ToolButton {
                    action: stopAction
                    display: AbstractButton.IconOnly
                }
            }
        }
    }

    Control {
        id: variableStateTableContainer
        anchors.top: toolBarBorder.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        padding: 10

        background: Rectangle {
            color: ThemeSettings.primary
        }

        contentItem: TableView {
            id: variableStateTable

            columnSpacing: 1
            rowSpacing: 1
            clip: true

            model: TableModel {
                TableModelColumn {
                    display: "variable"
                }
                TableModelColumn {
                    display: "value"
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

                color: ThemeSettings.background
                implicitHeight: tableCellText.implicitHeight
                implicitWidth: tableCellText.implicitWidth

                Text {
                    id: tableCellText
                    text: parent.display
                    anchors.fill: parent
                    rightPadding: ThemeSettings.editorFontSize
                    leftPadding: rightPadding
                    topPadding: ThemeSettings.editorFontSize / 2
                    bottomPadding: topPadding
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignBottom
                    font {
                        family: ThemeSettings.editorFontFamily
                        pointSize: ThemeSettings.editorFontSize
                    }
                }
            }
        }
    }
}
