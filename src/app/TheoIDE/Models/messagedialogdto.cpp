#include "messagedialogdto.hpp"

#include "dialogbutton.hpp"

MessageDialogDTO::MessageDialogDTO(const QString& text,
                                   DialogModelText detailedText,
                                   DialogModelText informativeText)
    : _text(text),
      _detailedText(detailedText),
      _informativeText(informativeText) {}

void MessageDialogDTO::setButtonWithCallback(DialogButton button,
                                             DialogModelCallback callback) {
  _buttonCallbacks.insert(button, callback);
}

void MessageDialogDTO::setButton(DialogButton button) {
  _buttonCallbacks.insert(button, []() -> void {});
}

bool MessageDialogDTO::hasCallbackFor(DialogButton button) const {
  return _buttonCallbacks.find(button) != _buttonCallbacks.end();
}

void MessageDialogDTO::runCallbackFor(DialogButton button) const {
  if (!hasCallbackFor(button)) {
    return;
  }
  _buttonCallbacks[button]();
}

QList<DialogButton> MessageDialogDTO::activeDialogButtons() const {
  return _buttonCallbacks.keys();
}
