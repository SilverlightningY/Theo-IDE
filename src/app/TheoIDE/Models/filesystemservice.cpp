#include <qcontainerfwd.h>
#include <qdir.h>
#include <qfiledevice.h>
#include <qfuture.h>
#include <qfuturewatcher.h>
#include <qlist.h>
#include <qmutex.h>
#include <qsharedpointer.h>
#include <qtconcurrentrun.h>

#include "filesystemservice.hpp"

const int DEFAULT_MAX_FILE_SIZE_BYTES = 2 * 1024 * 1024;
const int DEFAULT_BLOCK_SIZE_BYTES = 1024;

FileSystemService::FileSystemService(QObject* parent)
    : QObject(parent),
      _maxFileSizeBytes(DEFAULT_MAX_FILE_SIZE_BYTES),
      _blockSizeBytes(DEFAULT_BLOCK_SIZE_BYTES) {}

FileSystemService::~FileSystemService() {}

int FileSystemService::maxFileSizeBytes() const { return _maxFileSizeBytes; }

int FileSystemService::blockSizeBytes() const { return _blockSizeBytes; }

QList<QSharedPointer<QFile>> FileSystemService::filesBeingRead() const {
  return _filesBeingRead;
}

void FileSystemService::readFile(QSharedPointer<QFile> file) {
  QFuture future = readFileAsync(file)
                       .then([this, file](const QString& content) -> void {
                         removeFileFromBeingRead(file);
                         emit fileRead(file, content);
                       })
                       .onFailed([this, file](const FileError& error) -> void {
                         removeFileFromBeingRead(file);
                         emit fileReadFailed(file, error);
                       })
                       .onCanceled([this, file]() -> void {
                         removeFileFromBeingRead(file);
                         emit fileReadCanceled(file);
                       });
  QFutureWatcher<void>* watcher = new QFutureWatcher<void>(this);
  connect(watcher, &QFutureWatcher<void>::finished, [watcher]() -> void {
    qDebug() << "File read finished";
    watcher->deleteLater();
  });
  watcher->setFuture(future);
  addFileToBeingRead(file);
}

void FileSystemService::setMaxFileSizeBytes(int maxFileSize) {
  if (_maxFileSizeBytes != maxFileSize) {
    _maxFileSizeBytes = maxFileSize;
    emit maxFileSizeBytesChanged(maxFileSize);
  }
}

void FileSystemService::setBlockSizeBytes(int blockSizeBytes) {
  if (_blockSizeBytes != blockSizeBytes) {
    _blockSizeBytes = blockSizeBytes;
    emit blockSizeBytesChanged(blockSizeBytes);
  }
}

void FileSystemService::openFileReadable(QSharedPointer<QFile> file) const {
  ensureFileExists(file);
  if (file->isOpen()) {
    const bool readableFlagSet =
        file->openMode() & QIODevice::OpenModeFlag::ReadOnly;
    if (readableFlagSet) {
      return;
    }
    file->close();
  }
  if (!file->open(QIODevice::OpenModeFlag::ReadOnly)) {
    throw FileOpenReadableError(file->fileName());
  }
}

void FileSystemService::ensureFileExists(QSharedPointer<QFile> file) const {
  if (!file->exists()) {
    throw FileDoesNotExistError(file->fileName());
  }
}

QString FileSystemService::readFileSync(QSharedPointer<QFile> file) const {
  FileAutoCloser fileCloser(file.data());
  openFileReadable(file);
  unsigned int bytesRead = 0;
  QByteArray content;
  while (!file->atEnd()) {
    if (bytesRead > maxFileSizeBytes()) {
      throw MaxReadFileSizeExceededError(file->fileName(), maxFileSizeBytes());
    }
    QByteArray block = file->read(blockSizeBytes());
    if (file->error() == QFileDevice::ReadError) {
      throw FileReadError(file->fileName(), bytesRead + block.size());
    }
    content.append(block);
    bytesRead += block.size();
  }
  return QString::fromUtf8(content);
}

QFuture<QString> FileSystemService::readFileAsync(
    QSharedPointer<QFile> file) const {
  return QtConcurrent::run(&FileSystemService::readFileSync, this, file);
}

void FileSystemService::addFileToBeingRead(QSharedPointer<QFile> file) {
  QMutexLocker locker(&_filesBeingReadMutex);
  _filesBeingRead.append(file);
  emit filesBeingReadChanged(_filesBeingRead.size());
}

void FileSystemService::removeFileFromBeingRead(QSharedPointer<QFile> file) {
  QMutexLocker locker(&_filesBeingReadMutex);
  _filesBeingRead.removeOne(file);
  emit filesBeingReadChanged(_filesBeingRead.size());
}
