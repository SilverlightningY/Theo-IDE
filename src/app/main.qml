import TheoIDE.Pages
import QtQuick.Controls.Material
import TheoIDE.Persistence

ApplicationWindow {
    width: 1000
    height: 1000
    visible: true

    Material.accent: ThemeSettings.accent
    Material.background: ThemeSettings.background
    Material.foreground: ThemeSettings.foreground
    Material.primary: ThemeSettings.primary
    Material.theme: ThemeSettings.theme

    StackView {
        id: stackView

        anchors.fill: parent

        initialItem: EditorPage {}
    }
}
