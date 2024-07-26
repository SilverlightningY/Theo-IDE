import QtQuick
import QtQuick.Controls
import io.qt.examples.chattutorial

Page {
    id: root

    ListView {
        id: listView

        anchors.fill: parent
        topMargin: 48
        leftMargin: 48
        bottomMargin: 48
        rightMargin: 48
        spacing: 20

        model: SqlContactModel {
        }

        delegate: ItemDelegate {
            text: model.display
            width: listView.width - listView.leftMargin - listView.rightMargin
            height: avatar.implicitHeight
            leftPadding: avatar.implicitWidth + 32
            onClicked: root.StackView.view.push("ConversationPage.qml", {
                "inConversationWith": model.display
            })

            Image {
                id: avatar

                source: "qrc:/qt/qml/QtTest/assets/images/" + model.display.replace(" ", "_") + ".png"
            }

        }

    }

    header: ToolBar {
        Label {
            text: qsTr("Contacts")
            font.pixelSize: 20
            anchors.centerIn: parent
        }

    }

}
