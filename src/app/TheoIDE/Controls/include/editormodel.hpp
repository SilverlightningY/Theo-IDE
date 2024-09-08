#ifndef _THEOIDE_CONTROLS_EDITORMODEL_
#define _THEOIDE_CONTROLS_EDITORMODEL_

#include <QAbstractItemModel>
#include <QDir>
#include <QList>
#include <QMap>
#include <QMutex>
#include <QPointer>
#include <QQuickTextDocument>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QTextDocument>
#include <QUrl>
#include <QVariant>
#include <QtQmlIntegration>
#include <optional>
#include <stdexcept>

#include "compilerservice.hpp"
#include "dialogservice.hpp"
#include "filesystemservice.hpp"

using FileOptional = std::optional<QSharedPointer<QFile>>;

class NoMainTabError : public std::runtime_error {
 public:
  NoMainTabError() : std::runtime_error("Main tab is unset") {}
  ~NoMainTabError() {}
};

class EmptyMainTabError : public std::runtime_error {
 public:
  EmptyMainTabError() : std::runtime_error("Main tab is empty") {}
  ~EmptyMainTabError() {}
};

class MainTabInvalidStateError : public std::runtime_error {
 public:
  MainTabInvalidStateError()
      : std::runtime_error("Main tab is in invalid state") {}
  ~MainTabInvalidStateError() {}
};

class TabModel {
 public:
  TabModel(const QString& name, FileOptional file, const QString& storedText)
      : _file(file),
        _name(name),
        _storedText(storedText),
        _textDocument(nullptr) {}
  TabModel(const QString& name) : TabModel(name, std::nullopt, QString()) {}
  TabModel(const TabModel& tabModel)
      : _file(tabModel._file),
        _name(tabModel._name),
        _storedText(tabModel._storedText),
        _activeBreakPoints(tabModel._activeBreakPoints),
        _textDocument(tabModel._textDocument) {}

  QString name() const { return _name; }
  QString displayName() const {
    if (isModified()) {
      return name() + " *";
    }
    return name();
  }
  FileOptional file() const { return _file; }
  QString storedText() const { return _storedText; }
  QList<int> activeBreakPoints() const { return _activeBreakPoints; }
  QPointer<QTextDocument> textDocument() const { return _textDocument; }
  bool isModified() const {
    return _textDocument && _textDocument->isModified();
  }
  bool isTemporary() const { return !_file.has_value(); }

  void setName(const QString& name) { _name = name; }
  void setStoredText(const QString& text) { _storedText = text; }
  void setActiveBreakPoints(QList<int> activeBreakPoints) {
    _activeBreakPoints = activeBreakPoints;
  }
  void setFile(FileOptional file) { _file = file; }
  void setTextDocument(QTextDocument* textDocument) {
    _textDocument = textDocument;
  }

 private:
  QPointer<QTextDocument> _textDocument;
  std::optional<QSharedPointer<QFile>> _file;
  QString _name;
  QString _storedText;
  QList<int> _activeBreakPoints;
};

using TabModelOptional = std::optional<QSharedPointer<TabModel>>;

class EditorModel : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(int mainTabIndex READ mainTabIndex WRITE setMainTabIndex NOTIFY
                 mainTabIndexChanged)
  Q_PROPERTY(int currentTabIndex READ currentTabIndex WRITE setCurrentTabIndex
                 NOTIFY currentTabIndexChanged)
  Q_PROPERTY(FileSystemService* fileSystemService READ fileSystemService WRITE
                 setFileSystemService NOTIFY fileSystemServiceChanged)
  Q_PROPERTY(DialogService* dialogService READ dialogService WRITE
                 setDialogService NOTIFY dialogServiceChanged)
  Q_PROPERTY(CompilerService* compilerService READ compilerService WRITE
                 setCompilerService NOTIFY compilerServiceChanged)
  Q_PROPERTY(bool isRunning READ isRunning NOTIFY isRunningChanged)
  QML_ELEMENT
 public:
  EditorModel(QObject* parent = nullptr);
  ~EditorModel();
  enum EditorModelRoles {
    StoredTabTextRole = Qt::UserRole + 1,
    TabNameRole,
    DisplayTabNameRole,
    IsModifiedRole,
    IsTemporaryRole,
    TextDocumentRole,
    OpenRole,
    IsReadOnlyRole,
  };
  CompilerService* compilerService() const;
  DialogService* dialogService() const;
  FileSystemService* fileSystemService() const;
  QHash<int, QByteArray> roleNames() const override;
  QVariant data(const QModelIndex& index, int role) const override;
  bool setData(const QModelIndex& index, const QVariant& data,
               int role) override;
  int mainTabIndex() const;
  int currentTabIndex() const;
  int rowCount(const QModelIndex& index) const override;
  bool isRunning() const;
  void saveTabAt(qsizetype index);
  void closeTabAt(qsizetype index);

  Q_INVOKABLE
  void openFile(const QUrl& url);
  Q_INVOKABLE
  void saveAllTabs();
  Q_INVOKABLE
  void createNewTab();
  Q_INVOKABLE
  void runScript();
  Q_INVOKABLE
  void runScriptInDebugMode();

 public slots:
  void createTabFromFile(QSharedPointer<QFile> file, const QString& storedText);
  void displayFileReadFailure(QSharedPointer<QFile> file,
                              const FileError& error);
  void setCompilerService(CompilerService* compilerservice);
  void setDialogService(DialogService* dialogService);
  void setFileSystemService(FileSystemService* fileSystemService);
  void setMainTabIndex(int index);
  void setCurrentTabIndex(int index);
  void displayFileReadMaxReadFileSizeExceededFailure(
      QSharedPointer<QFile> file, const MaxReadFileSizeExceededError& error);
  void displayFileReadFileReadPermissionFailure(QSharedPointer<QFile> file,
                                                const FileReadError& error);
  void displayFileReadFileDoesNotExistFailure(
      QSharedPointer<QFile> file, const FileDoesNotExistError& error);

 signals:
  void currentTabIndexChanged(int index);
  void compilerServiceChanged();
  void dialogServiceChanged();
  void fileSystemServiceChanged();
  void mainTabIndexChanged(int index);
  void isRunningChanged(bool isRunning);

 protected slots:
  void updateMainTabIndex();
  void updateAllTabNames();
  void compilationRevisionAvailable(int revision);
  void displayExecutionFailedForInternalReason();

 private:
  int _mainTabIndex = -1;
  int _currentTabIndex = -1;
  QPointer<FileSystemService> _fileSystemService;
  QPointer<DialogService> _dialogService;
  QPointer<CompilerService> _compilerService;
  QList<QSharedPointer<TabModel>> _tabs;
  mutable QMutex _tabsMutex;
  mutable QMutex _temporaryTabIndexesMutex;
  QMap<QSharedPointer<TabModel>, int> _temporaryTabIndexes;
  long long _currentRunRevision = 0;
  bool _isRunning = false;

  QString tabNameAt(qsizetype index) const;
  QString storedTabTextAt(qsizetype index) const;
  bool isTabModifiedAt(qsizetype index) const;
  bool isTabTemporaryAt(qsizetype index) const;
  bool isTabReadOnlyAt(qsizetype index) const;
  bool tabIndexOutOfRange(qsizetype index) const;
  TabModelOptional mainTab() const;
  TabModelOptional tabAt(qsizetype index) const;
  QString createTabNameRelativeToMainTab(QSharedPointer<QFile> file) const;
  void saveTab(QSharedPointer<TabModel> tab);
  QString displayTabNameAt(qsizetype index) const;
  int appendTemporaryTabIndex(QSharedPointer<TabModel> tab);
  void removeTemporaryTabIndex(QSharedPointer<TabModel> tab);
  void disconnectFileSystemService();
  void connectFileSystemService();
  void connectCompilerService();
  void disconnectCompilerService();
  bool setTextDocumentVariantAt(qsizetype index, const QVariant& data);
  bool setOpenAt(qsizetype index, const QVariant& data);
  CompilationTask createCompilationTaskFromTabContent() const;
  void setIsRunning(bool isRunning);
  void updateTabNameAt(qsizetype index);
  bool tryPushCompilationTask();
  bool compilationPreconditionsFulfilled() const;
  bool isMainTabIndex(int index) const;
};

#endif
