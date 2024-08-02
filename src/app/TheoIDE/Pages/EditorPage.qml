import QtQuick
import QtQuick.Controls.Basic

Page {
    header: ToolBar {
        Label {
            text: qsTr("Theo IDE")
            font.pixelSize: 20
            anchors.centerIn: parent
        }
    }
    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        Rectangle {
            color: "red"
            SplitView.preferredWidth: 100
            SplitView.fillWidth: true
        }
        Rectangle {
            color: "green"
            SplitView.preferredWidth: 200
        }
    }
}
