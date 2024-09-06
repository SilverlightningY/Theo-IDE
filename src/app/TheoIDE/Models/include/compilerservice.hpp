#ifndef _THEOIDE_MODELS_COMPILERSERVICE_
#define _THEOIDE_MODELS_COMPILERSERVICE_

#include <qcontainerfwd.h>
#include <qmap.h>
#include <qobject.h>
#include <qqmlintegration.h>
#include <qsharedpointer.h>
#include <qtmetamacros.h>

#include "gen.hpp"

class CompilationTask {
 public:
  CompilationTask(int revision, QMap<QString, QString> content,
                  QString mainTabName)
      : _revision(revision), _content(content), _mainTabName(mainTabName) {}
  int revision() const { return _revision; }
  QMap<QString, QString> content() const { return _content; }
  QString mainTabName() const { return _mainTabName; }

 private:
  QString _mainTabName;
  int _revision;
  QMap<QString, QString> _content;
};

class CompilerService : public QObject {
  Q_OBJECT
  Q_PROPERTY(QSharedPointer<Theo::CodegenResult> result READ result NOTIFY
                 resultChanged)
  QML_ELEMENT
 public:
  CompilerService(QObject* parent = nullptr);
  ~CompilerService();
  QSharedPointer<Theo::CodegenResult> result() const;
  void compile(CompilationTask task);

 signals:
  void compilationFinished(int revision);
  void resultChanged();

 private:
  void setResult(QSharedPointer<Theo::CodegenResult> result);
};

#endif
