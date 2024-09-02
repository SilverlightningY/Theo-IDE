#ifndef _THEOIDE_CONTROLS_MESSAGEDIALOGMODEL_
#define _THEOIDE_CONTROLS_MESSAGEDIALOGMODEL_

#include <qcontainerfwd.h>
#include <qjsvalue.h>
#include <qlist.h>
#include <qobject.h>
#include <qpointer.h>
#include <qqmlintegration.h>
#include <qsharedpointer.h>
#include <qtmetamacros.h>

#include "dialogbutton.hpp"
#include "dialogservice.hpp"
#include "messagedialogdto.hpp"

class MessageDialogModel : public QObject {
  Q_OBJECT
  Q_PROPERTY(DialogService* dialogService READ dialogService WRITE
                 setDialogService NOTIFY dialogServiceChanged)
  Q_PROPERTY(QJSValue text READ text NOTIFY textChanged)
  Q_PROPERTY(QJSValue detailedText READ detailedText NOTIFY detailedTextChanged)
  Q_PROPERTY(QJSValue informativeText READ informativeText NOTIFY
                 informativeTextChanged)
  Q_PROPERTY(bool open READ open NOTIFY openChanged)
  Q_PROPERTY(
      QVariantList dialogButtons READ dialogButtons NOTIFY dialogButtonsChanged)
  QML_ELEMENT
 public:
  MessageDialogModel(QObject* parent = nullptr);
  ~MessageDialogModel();
  DialogService* dialogService() const;
  QJSValue text() const;
  QJSValue detailedText() const;
  QJSValue informativeText() const;
  bool open() const;
  QVariantList dialogButtons() const;
  Q_INVOKABLE
  void runButtonAction(DialogButton button);

 public slots:
  void setDialogService(DialogService* dialogService);
  void setMessageDialogDTO(QSharedPointer<MessageDialogDTO> dto);
  void setOpen(bool value);
  void showMessage();

 signals:
  void dialogServiceChanged();
  void textChanged();
  void detailedTextChanged();
  void informativeTextChanged();
  void openChanged(bool open);
  void dialogButtonsChanged();

 private:
  QPointer<DialogService> _dialogService;
  QSharedPointer<MessageDialogDTO> _messageDialogDTO;
  bool _open;
  void disconnectDialogService();
  void connectDialogService();
};

#endif
