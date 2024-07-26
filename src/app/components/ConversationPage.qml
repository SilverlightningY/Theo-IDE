import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import io.qt.examples.chattutorial

Page {
    id: root

    property string inConversationWith

    ColumnLayout {
        anchors.fill: parent

        ListView {
            id: listView

            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: pane.leftPadding + messageField.leftPadding
            displayMarginBeginning: 40
            displayMarginEnd: 40
            verticalLayoutDirection: ListView.BottomToTop
            spacing: 12

            model: SqlConversationModel {
                recipient: inConversationWith
            }

            delegate: Column {
                readonly property bool sentByMe: model.recipient !== "Me"

                anchors.right: sentByMe ? listView.contentItem.right : undefined
                spacing: 6

                Row {
                    id: messageRow

                    spacing: 6
                    anchors.right: sentByMe ? parent.right : undefined

                    Image {
                        id: avatar

                        source: !sentByMe ? "qrc:/qt/qml/QtTest/assets/images/" + model.author.replace(" ", "_") + ".png" : ""
                    }

                    Rectangle {
                        width: Math.min(messageText.implicitWidth + 24, listView.width - (!sentByMe ? avatar.width + messageRow.spacing : 0))
                        height: messageText.implicitHeight + 24
                        color: sentByMe ? "lightgray" : "steelblue"

                        Label {
                            id: messageText

                            text: model.message
                            color: sentByMe ? "black" : "white"
                            anchors.fill: parent
                            anchors.margins: 12
                            wrapMode: Label.Wrap
                        }

                    }

                }

                Label {
                    id: timestampText

                    text: Qt.formatDateTime(model.timestamp, "d MMM hh:mm")
                    color: "lightgray"
                    anchors.right: sentByMe ? parent.right : undefined
                }

            }

            ScrollBar.vertical: ScrollBar {
            }

        }

        Pane {
            id: pane

            Layout.fillWidth: true
            Layout.fillHeight: false

            RowLayout {
                width: parent.width

                TextArea {
                    id: messageField

                    Layout.fillWidth: true
                    placeholderText: qsTr("Compose Message")
                    wrapMode: TextArea.Wrap
                }

                Button {
                    id: sendButton

                    text: qsTr("Send")
                    enabled: messageField.length > 0
                    Layout.fillWidth: false
                    onClicked: {
                        listView.model.sendMessage(inConversationWith, messageField.text);
                        messageField.text = "";
                    }
                }

            }

        }

    }

    header: ToolBar {
        ToolButton {
            text: qsTr("Back")
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            onClicked: root.StackView.view.pop()
        }

        Label {
            id: pageTitle

            text: inConversationWith
            font.pixelSize: 20
            anchors.centerIn: parent
        }

    }

}
