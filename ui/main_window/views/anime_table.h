#ifndef UI_MAIN_WINDOW_VIEWS_ANIME_TABLE_H__
#define UI_MAIN_WINDOW_VIEWS_ANIME_TABLE_H__

#include <QMenu>
#include <QSet>
#include <QTableView>

#include "../../../src/models/media.h"
#include "../../../src/models/media_list.h"
#include "../components/media_table_model.h"
#include "../components/media_table_proxy_model.h"

namespace Views {
class AnimeTable : public QTableView {
  Q_OBJECT

 public:
  AnimeTable(QWidget *parent = nullptr, QSet<int> list = {});

  void setList(const QSet<int> &list);
  void refresh();
  void setFilter(const QString &text);
  void contextMenuEvent(QContextMenuEvent *);

 public slots:
  void openAnimePanel();
  void incrementProgress();
  void deleteEntry();
  void setStatusCurrent();
  void setStatusPaused();
  void setStatusPlanning();
  void setStatusCompleted();
  void setStatusDropped();
  void toggleHiddenDefault(const bool checked);
  void setCustomList(const int list, const bool checked);

 private:
  MediaTableModel *model;
  MediaTableProxyModel *proxy_model;

  Media *selectedMedia;
};
}  // namespace Views

#endif  // UI_MAIN_WINDOW_VIEWS_ANIME_TABLE_H__
