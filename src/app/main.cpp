#include <QGuiApplication>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTranslator>
#include <QtCore>
#include <QtQml>

using namespace Qt::Literals::StringLiterals;

static void connectToDatabase() {
  QSqlDatabase database = QSqlDatabase::database();
  if (!database.isValid()) {
    database = QSqlDatabase::addDatabase("QSQLITE");
    if (!database.isValid()) {
      qFatal("Cannot add database: %s",
             qPrintable(database.lastError().text()));
    }
  }

  const QDir writeDir =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  if (!writeDir.mkpath(".")) {
    qFatal("Failed to create writable directory at %s",
           qPrintable(writeDir.absolutePath()));
  }

  const QString fileName = writeDir.absolutePath() + "/chat-database.sqlite3";
  database.setDatabaseName(fileName);
  if (!database.open()) {
    qFatal("Cannot open database: %s", qPrintable(database.lastError().text()));
    QFile::remove(fileName);
  }
}

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);
  app.setApplicationName("TheoIDE");
  app.setApplicationDisplayName("Theo IDE");
  app.setOrganizationName("Theo IDE Development Team");
  app.setOrganizationDomain("Theo-IDE.github.io");

  // connectToDatabase();

  QTranslator translator;
  const QStringList uiLanguages = QLocale::system().uiLanguages();
  for (const QString &locale : uiLanguages) {
    const QString baseName = "QtTest_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + baseName)) {
      app.installTranslator(&translator);
      break;
    }
  }

  QQmlApplicationEngine engine;
  const QUrl url(u"qrc:/qt/qml/app/main.qml"_s);
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
