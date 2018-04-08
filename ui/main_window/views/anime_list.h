#ifndef UI_MAIN_WINDOW_VIEWS_ANIME_LIST_H__
#define UI_MAIN_WINDOW_VIEWS_ANIME_LIST_H__

#include <QKeyEvent>
#include <QTimer>
#include <QWidget>

#include "../../../src/models/media_list.h"
#include "./anime_table.h"

namespace Ui {
class AnimeList;
}

namespace Views {
class AnimeList : public QWidget {
  Q_OBJECT

 public:
  explicit AnimeList(QWidget *parent = 0);
  ~AnimeList();

  void keyPressEvent(QKeyEvent *event);

 private:
  void sortTabs();
  int getTabValue(const QString &text);

 private:
  Ui::AnimeList *ui;
  MediaList *mediaList;
  QMap<QString, AnimeTable *> listTabs;
  QTimer *refreshTimer;
};
}  // namespace Views

#endif  // UI_MAIN_WINDOW_VIEWS_ANIME_LIST_H__
