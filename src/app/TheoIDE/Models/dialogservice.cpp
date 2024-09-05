#include <qsharedpointer.h>

#include <functional>
#include <optional>

#include "dialogbutton.hpp"
#include "dialogservice.hpp"
#include "messagedialogdto.hpp"

DialogService::DialogService() : QObject() {}
DialogService::~DialogService() {}

void DialogService::add(QSharedPointer<MessageDialogDTO> dto) {
  _dialogDTOs.append(dto);
  emit elementAdded();
}

std::optional<QSharedPointer<MessageDialogDTO>> DialogService::remove() {
  if (isEmpty()) {
    return std::nullopt;
  }
  const auto dto = _dialogDTOs.at(0);
  _dialogDTOs.removeFirst();
  return dto;
}

void DialogService::addReadPermissionDenied(const QString& fileName) {
  auto dto = QSharedPointer<MessageDialogDTO>(
      new MessageDialogDTO(tr("Permission denied"), tr("File can not be read"),
                           tr("Read permission denied."), std::nullopt));
  dto->setButton(DialogButton::Ok);
  add(dto);
}

void DialogService::addFileDoesNotExist(const QString& fileName) {
  auto dto = QSharedPointer<MessageDialogDTO>(new MessageDialogDTO(
      tr("File missing"), tr("The file %1 does not exist").arg(fileName),
      std::nullopt, std::nullopt));
  dto->setButton(DialogButton::Ok);
  add(dto);
}

void DialogService::addUnsavedChangesInFile(
    const QString& fileName, std::function<void(void)> onSave,
    std::function<void(void)> onDiscard) {
  const QString text = tr("The document %1 has been modified").arg(fileName);
  const QString informativeText = tr("Do you want to save your changes?");
  auto dto = QSharedPointer<MessageDialogDTO>(new MessageDialogDTO(
      tr("Unsaved changes"), text, std::nullopt, informativeText));
  dto->setButtonWithCallback(DialogButton::Save, onSave);
  dto->setButtonWithCallback(DialogButton::Discard, onDiscard);
  add(dto);
}

void DialogService::addMaxReadFileSizeExceeded(const QString& fileName,
                                               const int maxFileSizeBytes) {
  const QString text = tr("Reading of the file %1 was aborted because the "
                          "maximum read size has been exceeded")
                           .arg(fileName);
  const QString detailedText =
      tr("The maximum read size is set to %1 bytes").arg(maxFileSizeBytes);
  auto dto = QSharedPointer<MessageDialogDTO>(new MessageDialogDTO(
      tr("Reading aborted"), text, detailedText, std::nullopt));
  dto->setButton(DialogButton::Ok);
  add(dto);
}

bool DialogService::isEmpty() const { return _dialogDTOs.empty(); }
