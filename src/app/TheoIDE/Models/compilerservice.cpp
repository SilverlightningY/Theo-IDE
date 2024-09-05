#include "compilerservice.hpp"

#include <qdebug.h>

CompilerService::CompilerService(QObject* parent) : QObject(parent) {}

CompilerService::~CompilerService() {}

void CompilerService::compile(CompilationTask task) {
  qDebug() << "compile revision" << task.revision();
}
