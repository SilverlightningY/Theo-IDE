import QtQuick
import QtQuick.Controls

ApplicationWindow {
    width: 540
    height: 960
    visible: true
    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: ContactPage {}
    }
}
