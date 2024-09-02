import QtQuick
import TheoIDE.Persistence
import TheoIDE.Controls

Item {
    id: root
    property alias currentIndex: lineInfoColumn.currentIndex
    property alias currentItem: lineInfoColumn.currentItem
    readonly property int innerMargin: 8
    property alias text: textEdit.text
    property alias textDocument: textEdit.textDocument

    Rectangle {
        id: currentLineHighlight
        color: ThemeSettings.primary
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.currentItem ? parent.currentItem.height : 0
        y: parent.currentItem ? parent.currentItem.y + parent.innerMargin : 0
        z: 0
        visible: false
    }

    LineInfoColumn {
        id: lineInfoColumn
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        leftMargin: root.innerMargin
        rightMargin: root.innerMargin
        bottomMargin: root.innerMargin
        topMargin: root.innerMargin
        width: implicitWidth
        currentIndex: 1
        model: LineInfoColumnModel {
            id: lineInfoColumnModel
            textDocument: textEdit.textDocument
        }
        contentY: textEdit.contentY
        interactive: false
    }

    PlainTextEditor {
        id: textEdit
        anchors.top: parent.top
        anchors.left: lineInfoColumn.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        innerMargin: root.innerMargin
    }
}
