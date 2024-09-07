#include "include/systemfontconfiguration.hpp"

#include <qdebug.h>
#include <qfontdatabase.h>
#include <qfontmetrics.h>
#include <qwindowdefs.h>

SystemFontConfiguraton::SystemFontConfiguraton(QObject* parent)
    : QObject(parent) {}
SystemFontConfiguraton::~SystemFontConfiguraton() {}

QFont SystemFontConfiguraton::defaultFont() const {
  return QFontDatabase::systemFont(QFontDatabase::GeneralFont);
}

int SystemFontConfiguraton::defaultFontSize() const {
  return defaultFont().pointSize();
}

QFont SystemFontConfiguraton::defaultMonospaceFont() const {
  QFont monospaceFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
  monospaceFont.setStyleHint(QFont::TypeWriter);
  return monospaceFont;
}

int SystemFontConfiguraton::defaultMonospaceFontSize() const {
  return defaultMonospaceFont().pointSize();
}

qreal SystemFontConfiguraton::calculateSpaceWidthOfFont(
    const QFont& font) const {
  const QFontMetrics fontMetrics(font);
  return fontMetrics.horizontalAdvance(' ');
}
