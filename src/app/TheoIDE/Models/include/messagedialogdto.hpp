#ifndef _THEOIDE_MODELS_MESSAGEDIALOGMODEL_
#define _THEOIDE_MODELS_MESSAGEDIALOGMODEL_

#include <qmap.h>

#include <functional>
#include <optional>

#include "dialogbutton.hpp"

using DialogModelCallback = std::function<void(void)>;
using DialogModelText = std::optional<QString>;

class MessageDialogDTO {
 public:
  MessageDialogDTO(const QString& text, DialogModelText detailedText,
                   DialogModelText informativeText);

  QString text() const { return _text; }
  DialogModelText detailedText() const { return _detailedText; }
  DialogModelText informativeText() const { return _informativeText; }
  void setButtonWithCallback(DialogButton button, DialogModelCallback callback);
  void setButton(DialogButton button);
  bool hasCallbackFor(DialogButton button) const;
  void runCallbackFor(DialogButton button) const;
  QList<DialogButton> activeDialogButtons() const;

 private:
  QString _text;
  DialogModelText _detailedText;
  DialogModelText _informativeText;
  QMap<DialogButton, DialogModelCallback> _buttonCallbacks;
};

#endif
