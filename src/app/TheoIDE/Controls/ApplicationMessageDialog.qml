import QtQuick.Dialogs
import TheoIDE.Models
import TheoIDE.Controls

MessageDialog {
    id: root

    required property MessageDialogModel dialogModel

    text: dialogModel.text
    informativeText: dialogModel.informativeText
    detailedText: dialogModel.detailedText
    dialogModel.onOpenChanged: function (value) {
        if (value) {
            root.open();
        }
    }
    flags: dialogButtonsToMessageDialogFlags(dialogModel.dialogButtons)

    function dialogButtonsToMessageDialogFlags(dialogButtons): int {
        let flags = 0;
        for (let index in dialogButtons) {
            let dialogButton = dialogButtons[index];
            switch (dialogButton) {
            case DialogButton.Ok:
                flags |= MessageDialog.Ok;
                break;
            case DialogButton.Open:
                flags |= MessageDialog.Open;
                break;
            case DialogButton.Save:
                flags |= MessageDialog.Save;
                break;
            case DialogButton.Cancel:
                flags |= MessageDialog.Cancel;
                break;
            case DialogButton.Close:
                flags |= MessageDialog.Close;
                break;
            case DialogButton.Discard:
                flags |= MessageDialog.Discard;
                break;
            case DialogButton.Apply:
                flags |= MessageDialog.Apply;
                break;
            case DialogButton.Reset:
                flags |= MessageDialog.Reset;
                break;
            case DialogButton.RestoreDefaults:
                flags |= MessageDialog.RestoreDefaults;
                break;
            case DialogButton.Help:
                flags |= MessageDialog.Help;
                break;
            case DialogButton.SaveAll:
                flags |= MessageDialog.SaveAll;
                break;
            case DialogButton.Yes:
                flags |= MessageDialog.Yes;
                break;
            case DialogButton.YesToAll:
                flags |= MessageDialog.YesToAll;
                break;
            case DialogButton.No:
                flags |= MessageDialog.No;
                break;
            case DialogButton.NoToAll:
                flags |= MessageDialog.NoToAll;
                break;
            case MessageDialog.Abort:
                flags |= MessageDialog.Abort;
                break;
            case MessageDialog.Retry:
                flags |= MessageDialog.Retry;
                break;
            case MessageDialog.Ignore:
                flags |= MessageDialog.Ignore;
                break;
            case MessageDialog.NoButton:
                flags |= MessageDialog.NoButton;
                break;
            }
        }
        return flags;
    }
}
