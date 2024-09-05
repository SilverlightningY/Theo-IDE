#ifndef _THEOIDE_CONTROLS_EDITORMODEL_
#define _THEOIDE_CONTROLS_EDITORMODEL_

#include <qabstractitemmodel.h>
#include <qdir.h>
#include <qhash.h>
#include <qmap.h>
#include <qmutex.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpointer.h>
#include <qqmlintegration.h>
#include <qscopedpointer.h>
#include <qsharedpointer.h>
#include <qstringview.h>
#include <qtextdocument.h>
#include <qtmetamacros.h>
#include <qurl.h>
#include <qvariant.h>

#include <optional>

#include "dialogservice.hpp"
#include "filesystemservice.hpp"

using FileOptional = std::optional<QSharedPointer<QFile>>;

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
  Q_PROPERTY(FileSystemService* fileSystemService READ fileSystemService WRITE
                 setFileSystemService NOTIFY fileSystemServiceChanged)
  Q_PROPERTY(DialogService* dialogService READ dialogService WRITE
                 setDialogService NOTIFY dialogServiceChanged)
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
  };
  QHash<int, QByteArray> roleNames() const override;
  int rowCount(const QModelIndex& index) const override;
  QVariant data(const QModelIndex& index, int role) const override;
  bool setData(const QModelIndex& index, const QVariant& data,
               int role) override;
  int mainTabIndex() const;
  FileSystemService* fileSystemService() const;
  DialogService* dialogService() const;

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
  Q_INVOKABLE
  void saveTabAt(int index);
  Q_INVOKABLE
  void closeTabAt(int index);

 public slots:
  void setMainTabIndex(int index);
  void setFileSystemService(FileSystemService* fileSystemService);
  void createTabFromFile(QSharedPointer<QFile> file, const QString& storedText);
  void setDialogService(DialogService* dialogService);
  void displayFileReadFailure(QSharedPointer<QFile> file,
                              const FileError& error);

 signals:
  void mainTabIndexChanged(int index);
  void fileSystemServiceChanged();
  void dialogServiceChanged();

 private:
  int _mainTabIndex = 0;
  QPointer<FileSystemService> _fileSystemService;
  QPointer<DialogService> _dialogService;
  QList<QSharedPointer<TabModel>> _tabs;
  mutable QMutex _tabsMutex;
  mutable QMutex _temporaryTabIndexesMutex;
  QMap<QSharedPointer<TabModel>, int> _temporaryTabIndexes;

  QString tabNameAt(int index) const;
  QString storedTabTextAt(int index) const;
  bool isTabModifiedAt(int index) const;
  bool isTabTemporaryAt(int index) const;
  bool tabIndexOutOfRange(int index) const;
  bool setTabTextAt(const QModelIndex& index, const QVariant& data);
  TabModelOptional mainTab() const;
  TabModelOptional tabAt(int index) const;
  QString createTabNameRelativeToMainTab(QSharedPointer<QFile> file) const;
  void saveTab(QSharedPointer<TabModel> tab);
  QString displayTabNameAt(int index) const;
  int appendTemporaryTabIndex(QSharedPointer<TabModel> tab);
  void removeTemporaryTabIndex(QSharedPointer<TabModel> tab);
  void updateAllTabNames();
  void disconnectFileSystemService();
  void connectFileSystemService();
  bool setTextDocumentVariantAt(int index, const QVariant& data);
  bool setOpenAt(int index, const QVariant& data);
};

#endif
