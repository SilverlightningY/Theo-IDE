import QtQuick
import QtQuick.Controls.Material
import TheoIDE.Persistence
import Qt.labs.qmlmodels

TableView {
    id: root

    columnSpacing: 1
    rowSpacing: 1
    clip: true
    boundsBehavior: Flickable.StopAtBounds
    implicitHeight: contentHeight

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

    columnWidthProvider: function (column) {
        if (!isColumnLoaded(column)) {
            return implicitColumnWidth(column);
        }
        if (column === 0) {
            return Math.max(implicitColumnWidth(column), root.width / 1.5);
        }
        return Math.max(implicitColumnWidth(column), root.width / 3);
    }

    delegate: Rectangle {
        required property string display

        color: ApplicationSettings.background
        implicitHeight: tableCellText.implicitHeight
        implicitWidth: tableCellText.implicitWidth
        radius: Material.ExtraSmallScale

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
            font: ApplicationSettings.editorFont
            color: ApplicationSettings.foreground
            clip: true
        }
    }
}
