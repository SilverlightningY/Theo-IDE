import TheoIDE.Pages
import QtQuick.Controls.Material
import TheoIDE.Persistence
import TheoIDE.Models
import TheoIDE.Controls

ApplicationWindow {
    id: root
    width: 1000
    height: 1000
    visible: true

    Material.accent: ThemeSettings.accent
    Material.background: ThemeSettings.background
    Material.foreground: ThemeSettings.foreground
    Material.primary: ThemeSettings.primary
    Material.theme: ThemeSettings.theme

    property DialogService dialogService: DialogService {}
    property FileSystemService fileSystemService: FileSystemService {}

    ApplicationMessageDialog {
        dialogModel: MessageDialogModel {
            dialogService: root.dialogService
        }
    }

    StackView {
        id: stackView

        anchors.fill: parent

        initialItem: EditorPage {
            model: EditorModel {
                fileSystemService: root.fileSystemService
                dialogService: root.dialogService
            }
        }
    }

    Component.onCompleted: console.log(dialogService.toString())
}
