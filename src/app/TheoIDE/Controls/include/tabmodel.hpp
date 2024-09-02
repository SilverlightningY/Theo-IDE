#ifndef _THEOIDE_CONTROLS_TABMODEL_
#define _THEOIDE_CONTROLS_TABMODEL_

#include <qabstractitemmodel.h>
#include <qlist.h>
#include <qqmlintegration.h>
#include <qtmetamacros.h>

class TabModel : public QAbstractItemModel {
 public:
  Q_OBJECT
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
  Q_PROPERTY(int currentLineNumber READ currentLineNumber WRITE
                 setCurrentLineNumber NOTIFY currentLineNumberChanged)
  QML_ELEMENT

  Q_INVOKABLE
  QList<int> getActiveBreakPoints() const;

  TabModel(QObject* parent = nullptr);
  ~TabModel();
};

#endif
