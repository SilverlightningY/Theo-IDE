#include <QSqlQueryModel>

class SqlContactModel : public QSqlQueryModel {
 public:
  SqlContactModel(QObject *parent = nullptr);
};
