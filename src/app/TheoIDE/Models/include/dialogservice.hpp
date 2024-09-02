#ifndef _THEOIDE_MODELS_DIALOGSERVICE_
#define _THEOIDE_MODELS_DIALOGSERVICE_

#include <qlist.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qqmlintegration.h>
#include <qsharedpointer.h>
#include <qtmetamacros.h>

#include <functional>
#include <optional>

#include "messagedialogdto.hpp"

class DialogService : public QObject {
  Q_OBJECT
  QML_ELEMENT
 public:
  DialogService();
  ~DialogService();
  void addReadPermissionDenied(const QString& fileName);
  void addFileDoesNotExist(const QString& fileName);
  void addUnsavedChangesInFile(const QString& fileName,
                               std::function<void(void)> onSave,
                               std::function<void(void)> onDiscard);
  void addMaxReadFileSizeExceeded(const QString& fileName,
                                  const int maxFileSizeBytes);

  std::optional<QSharedPointer<MessageDialogDTO>> remove();
  bool isEmpty() const;

 signals:
  void elementAdded();

 protected:
  void add(QSharedPointer<MessageDialogDTO> dialogDTO);

 private:
  QList<QSharedPointer<MessageDialogDTO>> _dialogDTOs;
};

#endif
