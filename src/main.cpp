#include <QApplication>
#include <QCoreApplication>
#include <QFile>
#include <QTimer>

#include "./ui/main_window.h"

#include "./clients/anilist.h"
#include "./models/media_list.h"
#include "./models/user.h"

#include "./detection/window_enumerator.h"

int main(int argc, char *argv[]) {
#ifdef Q_OS_WIN
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
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
  MainWindow w;
  w.show();

  AniList *anilist = &AniList::instance();

  // TODO: this should probably be moved somewhere else
  QTimer *windowTimer = new QTimer(&a);

  a.connect(windowTimer, &QTimer::timeout,
            []() { WindowEnumerator::instance().enumerateWindows(); });

  QCoreApplication::connect(anilist, &AniList::authenticated, [&windowTimer]() {
    User::instance().load();
    MediaList::instance().load();
    windowTimer->start(5000);
  });

  anilist->grant();

  return a.exec();
}
