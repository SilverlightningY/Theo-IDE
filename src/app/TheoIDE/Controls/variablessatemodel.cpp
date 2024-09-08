#include <qabstractitemmodel.h>
#include <qnamespace.h>
#include <qvariant.h>

#include <algorithm>
#include <cstddef>
#include <iterator>

#include "variablesstatemodel.hpp"
#include "virtualmachineservice.hpp"

VariablesStateModel::VariablesStateModel(QObject* parent)
    : QAbstractListModel(parent) {}

VariablesStateModel::~VariablesStateModel() {}

int VariablesStateModel::rowCount(const QModelIndex& parent) const {
  if (_virtualMachineService.isNull()) {
    return 0;
  }
  return _virtualMachineService->variablesState().count();
}

QVariant VariablesStateModel::data(const QModelIndex& index, int role) const {
  switch (role) {
    case VariableNameRole:
      return variableNameAt(index.row());
    case VariableValueRole:
      return variableValueAt(index.row());
    case VariableNameColumnWidthRole:
      return variableNameColumnWidth();
    case VariableValueColumnWidthRole:
      return variableValueColumnWidth();
    case Qt::DisplayRole:
      return displayRoleAt(index.row());
  }
  return QVariant();
}

QHash<int, QByteArray> VariablesStateModel::roleNames() const {
  return QHash<int, QByteArray>{
      {Qt::DisplayRole, "display"},
      {VariableNameRole, "variableName"},
      {VariableValueRole, "variableValue"},
      {VariableNameColumnWidthRole, "variableNameColumnWidth"},
      {VariableValueColumnWidthRole, "variableValueColumnWidth"},
      {VariableNameColumnImplicitWidthRole, "variableNameColumnImplicitWidth"},
      {VariableValueColumnImplicitWidthRole,
       "variableValueColumnImplicitWidth"},
  };
}

bool VariablesStateModel::setData(const QModelIndex& index,
                                  const QVariant& value, int role) {
  switch (role) {
    case VariableNameColumnImplicitWidthRole:
      return setVariableNameColumnImplicitWidthVariant(index.row(), value);
    case VariableValueColumnImplicitWidthRole:
      return setVariableValueColumnImplicitWidthVariant(index.row(), value);
  }
  return false;
}

void VariablesStateModel::setVirtualMachineService(
    VirtualMachineService* virtualMachineService) {
  if (_virtualMachineService != virtualMachineService) {
    disconnectVirtualMachineService();
    _virtualMachineService = QPointer(virtualMachineService);
    emit virtualMachineServiceChanged();
    connectVirtualMachineService();
  }
}

void VariablesStateModel::setVariableNameColumnWidth(int value) {
  if (_variableNameColumnWidth != value) {
    _variableNameColumnWidth = value;
    if (_variables.empty()) {
      return;
    }
    const QModelIndex firstIndex = createIndex(0, 0);
    const QModelIndex lastIndex = createIndex(_variables.length() - 1, 0);
    emit dataChanged(firstIndex, lastIndex, {VariableNameColumnWidthRole});
  }
}

void VariablesStateModel::setVariableValueColumnWidth(int value) {
  if (_variableValueColumnWidth != value) {
    _variableValueColumnWidth = value;
    if (_variables.empty()) {
      return;
    }
    const QModelIndex firstIndex = createIndex(0, 0);
    const QModelIndex lastIndex = createIndex(_variables.length() - 1, 0);
    emit dataChanged(firstIndex, lastIndex, {VariableValueColumnWidthRole});
  }
}

void VariablesStateModel::updateVariableNameColumnWidth() {
  if (_variableNameColumnImplicitWidths.empty()) {
    return;
  }
  const auto maxWidthIterator =
      std::max_element(_variableNameColumnImplicitWidths.begin(),
                       _variableNameColumnImplicitWidths.end());
  const int maxWidth = *maxWidthIterator;
  setVariableNameColumnWidth(maxWidth);
}

void VariablesStateModel::updateVariableValueColumnWidth() {
  if (_variableValueColumnImplicitWidths.empty()) {
    return;
  }
  const auto maxWidthIterator =
      std::max_element(_variableValueColumnImplicitWidths.begin(),
                       _variableValueColumnImplicitWidths.end());
  const int maxWidth = *maxWidthIterator;
  setVariableValueColumnWidth(maxWidth);
}

void VariablesStateModel::updateVariablesState() {
  if (_virtualMachineService.isNull()) {
    return;
  }
  const auto variablesState(_virtualMachineService->variablesState());
  for (const QString& variableName : variablesState.keys()) {
    const auto element =
        std::find_if(_variables.begin(), _variables.end(),
                     [variableName](const VariableState& state) -> bool {
                       return state.name == variableName;
                     });
    const bool elementFound = element < _variables.end();
    if (elementFound) {
      const std::ptrdiff_t index(std::distance(_variables.begin(), element));
      setValueAt(index, variablesState[variableName]);
    } else {
      appendVariableState(
          VariableState(variableName, variablesState[variableName]));
    }
  }
  for (auto iterator = _variables.begin(); iterator != _variables.end();
       ++iterator) {
    if (!variablesState.contains(iterator->name)) {
      std::ptrdiff_t index(std::distance(_variables.begin(), iterator));
      removeVariableStateAt(index);
    }
  }
}

QString VariablesStateModel::variableNameAt(int index) const {
  if (indexOutOfRange(index)) {
    return QString();
  }
  return _variables.at(index).name;
}

int VariablesStateModel::variableValueAt(int index) const {
  if (indexOutOfRange(index)) {
    return 0;
  }
  return _variables.at(index).value;
}

QString VariablesStateModel::displayRoleAt(int index) const {
  if (indexOutOfRange(index)) {
    return QString();
  }
  const VariableState state = _variables.at(index);
  return QString("%1: %2").arg(state.name).arg(state.value);
}

int VariablesStateModel::variableNameColumnWidth() const {
  return _variableNameColumnWidth;
}

int VariablesStateModel::variableValueColumnWidth() const {
  return _variableValueColumnWidth;
}

bool VariablesStateModel::setVariableNameColumnImplicitWidthVariant(
    int index, const QVariant& value) {
  if (value.isValid() && value.canConvert<int>()) {
    const int width = value.toInt();
    _variableNameColumnImplicitWidths[index] = width;
    updateVariableNameColumnWidth();
    return true;
  }
  return false;
}

bool VariablesStateModel::setVariableValueColumnImplicitWidthVariant(
    int index, const QVariant& value) {
  if (value.isValid() && value.canConvert<int>()) {
    const int width = value.toInt();
    _variableValueColumnImplicitWidths[index] = width;
    updateVariableValueColumnWidth();
    return true;
  }
  return false;
}

bool VariablesStateModel::indexOutOfRange(int index) const {
  return index < 0 || index >= _variables.size();
}

void VariablesStateModel::setValueAt(int index, int value) {
  if (indexOutOfRange(index)) {
    return;
  }
  _variables[index].value = value;
  qDebug() << "Assigned value" << value << "actually has value"
           << _variables.at(index).value;
  const QModelIndex modelIndex = createIndex(index, 0);
  emit dataChanged(modelIndex, modelIndex,
                   {VariableValueRole, Qt::DisplayRole});
}

void VariablesStateModel::appendVariableState(const VariableState& state) {
  const int index = _variables.size();
  beginInsertRows(QModelIndex(), index, index);
  _variables.append(state);
  endInsertRows();
}

void VariablesStateModel::removeVariableStateAt(int index) {
  if (indexOutOfRange(index)) {
    return;
  }
  beginRemoveRows(QModelIndex(), index, index);
  _variables.remove(index);
  endRemoveRows();
}

void VariablesStateModel::connectVirtualMachineService() {
  if (_virtualMachineService.isNull()) {
    return;
  }
  connect(_virtualMachineService, &VirtualMachineService::variablesStateChanged,
          this, &VariablesStateModel::updateVariablesState);
}

void VariablesStateModel::disconnectVirtualMachineService() {
  if (_virtualMachineService.isNull()) {
    return;
  }
  disconnect(_virtualMachineService, nullptr, this, nullptr);
  disconnect(this, nullptr, _virtualMachineService, nullptr);
}
