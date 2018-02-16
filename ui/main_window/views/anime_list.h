#ifndef UI_MAIN_WINDOW_VIEWS_ANIME_LIST_H__
#define UI_MAIN_WINDOW_VIEWS_ANIME_LIST_H__

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

 private:
  Ui::AnimeList *ui;
  MediaList *mediaList;
  QMap<QString, AnimeTable *> listTabs;
};
}  // namespace Views

#endif  // UI_MAIN_WINDOW_VIEWS_ANIME_LIST_H__
