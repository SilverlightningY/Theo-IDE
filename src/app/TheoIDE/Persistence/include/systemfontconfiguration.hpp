#ifndef _THEOIDE_PERSISTENCE_SYSTEMFONTCONFIGURATION_
#define _THEOIDE_PERSISTENCE_SYSTEMFONTCONFIGURATION_

#include <QtQml/qqmlregistration.h>
#include <qfont.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qtmetamacros.h>

class SystemFontConfiguraton : public QObject {
  Q_OBJECT
  QML_ELEMENT
  QML_SINGLETON
  Q_PROPERTY(int defaultFontSize READ defaultFontSize NOTIFY
                 defaultFontSizeChanged FINAL)
  Q_PROPERTY(int defaultMonospaceFontSize READ defaultMonospaceFontSize NOTIFY
                 defaultMonospaceFontSizeChanged FINAL)
  Q_PROPERTY(QFont defaultFont READ defaultFont NOTIFY defaultFontChanged FINAL)
  Q_PROPERTY(QFont defaultMonospaceFont READ defaultMonospaceFont NOTIFY
                 defaultMonospaceFontChanged FINAL)
 public:
  SystemFontConfiguraton(QObject* parent = nullptr);
  ~SystemFontConfiguraton();
  QFont defaultFont() const;
  int defaultFontSize() const;
  QFont defaultMonospaceFont() const;
  int defaultMonospaceFontSize() const;
 signals:
  void defaultFontSizeChanged();
  void defaultFontChanged();
  void defaultMonospaceFontSizeChanged();
  void defaultMonospaceFontChanged();
};

#endif
