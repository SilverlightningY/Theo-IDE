import QtQuick
import QtQml.Models
import TheoIDE.Persistence
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    id: root
    property alias currentIndex: lineInfoColumnFlickable.currentIndex
    property alias currentItem: lineInfoColumnFlickable.currentItem
    readonly property int innerMargin: 8

    Rectangle {
        id: currentLineHighlight
        color: ThemeSettings.primary
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.currentItem ? parent.currentItem.height : 0
        y: parent.currentItem ? parent.currentItem.y + parent.innerMargin : 0
        z: 0
    }

    ListView {
        id: lineInfoColumnFlickable
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        leftMargin: root.innerMargin
        rightMargin: root.innerMargin
        bottomMargin: root.innerMargin
        topMargin: root.innerMargin
        contentWidth: contentItem.childrenRect.width
        width: contentWidth + leftMargin + rightMargin
        clip: true
        currentIndex: 1
        model: ListModel {
            ListElement {
                lineNumber: 1
                breakPointSet: true
            }
            ListElement {
                lineNumber: 2
                breakPointSet: false
            }
            ListElement {
                lineNumber: 123
                breakPointSet: true
            }
        }
        delegate: LineInfoCell {
            anchors.right: parent ? parent.right : undefined
        }
        highlightFollowsCurrentItem: false
    }

    ScrollView {
        id: textEditFlickable
        anchors.top: parent.top
        anchors.left: lineInfoColumnFlickable.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        TextEdit {
            anchors.fill: parent
            bottomPadding: root.innerMargin
            leftPadding: root.innerMargin
            rightPadding: root.innerMargin
            topPadding: root.innerMargin
            wrapMode: TextEdit.NoWrap
            font {
                family: ThemeSettings.editorFontFamily
                pointSize: ThemeSettings.editorFontSize
            }
            text: "Content File 1"
        }
    }
}
