#ifndef UI_MAIN_WINDOW_VIEWS_TORRENTS_H__
#define UI_MAIN_WINDOW_VIEWS_TORRENTS_H__

#include <QJsonObject>
#include <QList>
#include <QMutex>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QWidget>

#include "../../../src/models/rss_item.h"
#include "../components/rss_table_model.h"

namespace Ui {
class Torrents;
}

namespace Views {
class Torrents : public QWidget {
  Q_OBJECT

 public:
  explicit Torrents(QWidget *parent = 0);
  ~Torrents();

  void fetchTorrents();
  void checkForMatches();
  void checkRule(QJsonObject rule);
  void downloadOnce(RSSItem *item);
  void download(RSSItem *item);
  void setTrayIcon(QSystemTrayIcon *icon);

 public slots:
  void timerTick();
  void contextMenu();

 private:
  Ui::Torrents *ui;
  QList<RSSItem *> items;
  RSSTableModel *model;
  QTimer *timer;
  int refresh{1};
  QMutex refreshLock;
  QSystemTrayIcon *tray{nullptr};
};
}  // namespace Views

#endif  // UI_MAIN_WINDOW_VIEWS_TORRENTS_H__
