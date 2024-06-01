#include <QGuiApplication>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QSqlDatabase>
#include <QSqlError>
#include <QTranslator>
#include <QtCore>
#include <QtQml>

#include "models/sqlcontactmodel.hpp"
#include "models/sqlconversationmodel.hpp"

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

  qmlRegisterType<SqlContactModel>("io.qt.examples.chattutorial", 1, 0,
                                   "SqlContactModel");
  qmlRegisterType<SqlConversationModel>("io.qt.examples.chattutorial", 1, 0,
                                        "SqlConversationModel");

  connectToDatabase();

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
  const QUrl url(u"qrc:/qt/qml/QtTest/main.qml"_s);
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
