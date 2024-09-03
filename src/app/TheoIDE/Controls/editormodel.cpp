#include <qabstractitemmodel.h>
#include <qalgorithms.h>
#include <qcontainerfwd.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qfuturewatcher.h>
#include <qhash.h>
#include <qlogging.h>
#include <qmutex.h>
#include <qnamespace.h>
#include <qscopedpointer.h>
#include <qsharedpointer.h>
#include <qstringview.h>
#include <qttranslation.h>
#include <qurl.h>
#include <qvariant.h>

#include <algorithm>
#include <optional>

#include "editormodel.hpp"
#include "filesystemservice.hpp"

EditorModel::EditorModel(QObject* parent) : QAbstractListModel(parent) {}
EditorModel::~EditorModel() {}

QHash<int, QByteArray> EditorModel::roleNames() const {
  return QHash<int, QByteArray>{
      {Qt::DisplayRole, "display"},   {StoredTabTextRole, "storedTabText"},
      {TabNameRole, "tabName"},       {DisplayTabNameRole, "displayTabName"},
      {IsModifiedRole, "isModified"}, {IsTemporaryRole, "isTemporary"}};
}

int EditorModel::rowCount(const QModelIndex& index) const {
  return _tabs.count();
}

QVariant EditorModel::data(const QModelIndex& index, int role) const {
  switch (role) {
    case StoredTabTextRole:
      return storedTabTextAt(index.row());
    case Qt::DisplayRole:
    case DisplayTabNameRole:
      return displayTabNameAt(index.row());
    case TabNameRole:
      return tabNameAt(index.row());
    case IsModifiedRole:
      return isTabModifiedAt(index.row());
    case IsTemporaryRole:
      return isTabTemporaryAt(index.row());
  }
  return QVariant();
}

QString EditorModel::storedTabTextAt(int index) const {
  const TabModelOptional tab = tabAt(index);
  if (tab.has_value() && !tab.value().isNull()) {
    return tab.value()->storedText();
  }
  return QString();
}

QString EditorModel::tabNameAt(int index) const {
  const TabModelOptional tab = tabAt(index);
  if (tab.has_value() && !tab.value().isNull()) {
    return tab.value()->name();
  }
  return QString();
}

QString EditorModel::displayTabNameAt(int index) const {
  const TabModelOptional tab = tabAt(index);
  if (tab.has_value() && !tab.value().isNull()) {
    return tab.value()->displayName();
  }
  return QString();
}

bool EditorModel::isTabModifiedAt(int index) const {
  const TabModelOptional tab = tabAt(index);
  return tab.has_value() && !tab.value().isNull() && tab.value()->isModified();
}

bool EditorModel::isTabTemporaryAt(int index) const {
  const TabModelOptional tab = tabAt(index);
  return tab.has_value() && !tab.value().isNull() && tab.value()->isTemporary();
}

bool EditorModel::setData(const QModelIndex& index, const QVariant& data,
                          int role) {
  return false;
}

FileSystemService* EditorModel::fileSystemService() const {
  return _fileSystemService.get();
}

void EditorModel::setFileSystemService(FileSystemService* fileSystemService) {
  if (_fileSystemService != fileSystemService) {
    if (_fileSystemService) {
      disconnect(_fileSystemService, nullptr, this, nullptr);
    }
    _fileSystemService = QPointer<FileSystemService>(fileSystemService);
    if (_fileSystemService) {
      connect(_fileSystemService, &FileSystemService::fileRead, this,
              &EditorModel::createTabFromFile);
    }
    emit fileSystemServiceChanged();
  }
}

void EditorModel::openFile(const QUrl& url) {
  QSharedPointer<QFile> file(new QFile(url.toLocalFile()));
  if (_fileSystemService) {
    _fileSystemService->readFile(file);
  }
}

void EditorModel::createTabFromFile(QSharedPointer<QFile> file,
                                    const QString& content) {
  QMutexLocker locker(&_tabsMutex);
  const int index = _tabs.size();
  beginInsertRows(QModelIndex(), index, index);
  const QString tabName = createTabNameRelativeToMainTab(file);
  _tabs.append(QSharedPointer<TabModel>(new TabModel(tabName, file, content)));
  endInsertRows();
}

void EditorModel::saveAllTabs() {
  for (QSharedPointer<TabModel> tab : _tabs) {
    saveTab(tab);
  }
}

void EditorModel::saveTabAt(int index) {
  TabModelOptional tab = tabAt(index);
  if (tab.has_value()) {
    saveTab(tab.value());
  }
}

void EditorModel::saveTab(QSharedPointer<TabModel> tab) {
  qDebug() << "save tab" << tab->name();
}

void EditorModel::createNewTab() {
  QSharedPointer<TabModel> tab(new TabModel(qtTrId("Temporary")));
  const int temporaryIndex = appendTemporaryTabIndex(tab);
  tab->setName(QString("%1-%2").arg(tab->name()).arg(temporaryIndex));
  QMutexLocker locker(&_tabsMutex);
  const int tabIndex = _tabs.size();
  emit beginInsertRows(QModelIndex(), tabIndex, tabIndex);
  _tabs.append(tab);
  emit endInsertRows();
}

void EditorModel::runScript() {}

void EditorModel::runScriptInDebugMode() {}

void EditorModel::closeTabAt(int index) {
  QMutexLocker locker(&_tabsMutex);
  TabModelOptional tab = tabAt(index);
  if (!tab.has_value()) {
    return;
  }
  const QSharedPointer<TabModel> tabModel = tab.value();
  beginRemoveRows(QModelIndex(), index, index);
  _tabs.removeAt(index);
  removeTemporaryTabIndex(tabModel);
  endRemoveRows();
}

TabModelOptional EditorModel::tabAt(int index) const {
  if (tabIndexOutOfRange(index) || _tabs.at(index).isNull()) {
    return std::nullopt;
  }
  return _tabs.at(index);
}

bool EditorModel::tabIndexOutOfRange(int index) const {
  return index >= _tabs.size() || index < 0;
}

int EditorModel::appendTemporaryTabIndex(QSharedPointer<TabModel> tab) {
  QMutexLocker locker(&_temporaryTabIndexesMutex);
  if (_temporaryTabIndexes.isEmpty()) {
    const int nextIndex = 1;
    _temporaryTabIndexes.insert(tab, nextIndex);
    return nextIndex;
  }
  const auto indexes = _temporaryTabIndexes.values();
  const int highestIndex = *std::max_element(indexes.begin(), indexes.end());
  const int nextIndex = highestIndex + 1;
  _temporaryTabIndexes.insert(tab, nextIndex);
  return nextIndex;
}

void EditorModel::removeTemporaryTabIndex(QSharedPointer<TabModel> tab) {
  QMutexLocker locker(&_temporaryTabIndexesMutex);
  _temporaryTabIndexes.remove(tab);
}

TabModelOptional EditorModel::mainTab() const { return tabAt(_mainTabIndex); }

QString EditorModel::createTabNameRelativeToMainTab(
    QSharedPointer<QFile> file) const {
  const QFileInfo fileInfo(*file);
  const TabModelOptional mainTab = this->mainTab();
  if (!mainTab.has_value() || mainTab.value().isNull()) {
    return fileInfo.fileName();
  }
  const FileOptional mainTabFile = mainTab.value()->file();
  if (!mainTabFile.has_value() || mainTabFile.value().isNull()) {
    return fileInfo.fileName();
  }
  const QFileInfo mainTabFileInfo(*mainTabFile.value());
  const QDir mainTabFileDir = mainTabFileInfo.absoluteDir();
  return mainTabFileDir.relativeFilePath(file->fileName());
}

int EditorModel::mainTabIndex() const { return _mainTabIndex; }

void EditorModel::setMainTabIndex(int index) {
  if (_mainTabIndex != index) {
    _mainTabIndex = index;
    emit mainTabIndexChanged(index);
    updateAllTabNames();
  }
}

void EditorModel::updateAllTabNames() {
  unsigned int index = 0;
  for (auto tab : _tabs) {
    const FileOptional fileOptional = tab->file();
    if (!fileOptional.has_value()) {
      ++index;
      continue;
    }
    const auto file = fileOptional.value();
    const auto newTabName = createTabNameRelativeToMainTab(file);
    if (newTabName == tab->name()) {
      ++index;
      continue;
    }
    tab->setName(newTabName);
    const QModelIndex modelIndex = createIndex(index, 0);
    emit dataChanged(modelIndex, modelIndex);
    ++index;
  }
}
