#include <QApplication>
#include <QCoreApplication>
#include <QFile>
#include <QTimer>

#include "./ui/main_window.h"

#include "./clients/anilist.h"
#include "./detection/window_enumerator.h"
#include "./models/media_list.h"
#include "./models/user.h"
#include "./paths.h"
#include "./settings.h"
#include "./utilities/crash_handler.h"

#include <iostream>

void Cerr(const QtMsgType, const QMessageLogContext &, const QString &str) {
  QByteArray ba = str.toUtf8();
  const char *msgAsCstring = ba.constData();

  QString format = "[yyyy-MM-dd HH:mm:ss.zzz] ";
  QString date = QDateTime::currentDateTimeUtc().toString(format);

  std::cerr << qPrintable(date);
  std::cerr << msgAsCstring << std::endl;
}

void File(QtMsgType type, const QMessageLogContext &, const QString &str) {
  QByteArray ba = str.toUtf8();
  const char *msg = ba.constData();

  QString parser(msg);

  parser.replace(QString::fromWCharArray(L"\u2401"), "");
  parser.replace(QString::fromWCharArray(L"\u2406"), "");
  parser.replace(QString::fromWCharArray(L"\u2404"), "");
  parser.replace(QString::fromWCharArray(L"\u2405"), "");

  QFile logFile(Paths::logFileName());
  logFile.open(QFile::WriteOnly | QFile::Append);

  QString format = "[yyyy-MM-dd HH:mm:ss.zzz] ";
  QString date = QDateTime::currentDateTimeUtc().toString(format);
  logFile.write(date.toUtf8());

  switch (type) {
    case QtDebugMsg:
      logFile.write(QString("Debug: ").toUtf8());
      break;
    case QtWarningMsg:
      logFile.write(QString("Warning: ").toUtf8());
      break;
    case QtCriticalMsg:
      logFile.write(QString("Critical: ").toUtf8());
      break;
    case QtFatalMsg:
      logFile.write(QString("Fatal: ").toUtf8());
#if QT_VERSION >= 0x050500
    case QtInfoMsg:
      logFile.write(QString("Info:").toUtf8());
      break;
#endif
    default:
      logFile.write(QString("Unknown: ").toUtf8());
      break;
  }

  logFile.write(parser.toUtf8() + QString("\n").toUtf8());
  logFile.close();

  if (type == QtFatalMsg) {
    abort();
  }
}

int main(int argc, char *argv[]) {
#ifdef Q_OS_WIN
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
#endif

  QCoreApplication::setOrganizationName("Kazakuri");
  QCoreApplication::setOrganizationDomain("shinjiru.me");
  QCoreApplication::setApplicationName("Shinjiru");

  QFile styleSheet(":/res/style.qss");

  if (!styleSheet.open(QFile::ReadOnly)) {
    // TODO
    return EXIT_FAILURE;
  }

  QString style = styleSheet.readAll();

  QApplication a(argc, argv);
  a.setStyleSheet(style);

#ifdef QT_NO_DEBUG
  QFile logFile(Paths::logFileName());
  if (logFile.exists()) logFile.remove();

  qInstallMessageHandler(File);
#else
  qInstallMessageHandler(Cerr);
#endif

#ifndef QT_DEBUG
  Breakpad::CrashHandler::instance()->Init(qApp->applicationDirPath());
#endif

  {
    QPixmap window_icon;
    QFile iconFile(":/res/icon.svg");
    iconFile.open(QFile::ReadOnly);
    QByteArray icon_data = iconFile.readAll();
    window_icon.loadFromData(icon_data);
    qApp->setWindowIcon(QIcon(window_icon));
  }

  Settings s;

  // TODO: language
  QTranslator qtTranslator;
  qtTranslator.load("qt_en", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
  a.installTranslator(&qtTranslator);

  QTranslator shinjiruTranslator;
  shinjiruTranslator.load(":/lang/shinjiru_en");
  a.installTranslator(&shinjiruTranslator);

  MainWindow w;

  if (s.get(Setting::StartMinimized).toBool()) {
    if (s.get(Setting::MinimizeToTray).toBool()) {
      w.hide();
    } else {
      w.showMinimized();
    }
  } else {
    w.show();
  }

  AniList *anilist = &AniList::instance();

  WindowEnumerator::instance();

  QCoreApplication::connect(anilist, &AniList::authenticated, []() {
    User::instance().load();
    MediaList::instance().load();
  });

  QCoreApplication::connect(anilist, &AniList::reload, []() {
    User::instance().load();
    MediaList::instance().load();
  });

  anilist->grant();

  return a.exec();
}
