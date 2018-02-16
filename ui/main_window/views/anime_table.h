#ifndef UI_MAIN_WINDOW_VIEWS_ANIME_TABLE_H__
#define UI_MAIN_WINDOW_VIEWS_ANIME_TABLE_H__

#include <QSet>
#include <QTableView>
#include <QWidget>

#include "../components/media_table_model.h"
#include "../components/media_table_proxy_model.h"

namespace Views {
class AnimeTable : public QWidget {
 public:
  AnimeTable(QWidget *parent = nullptr, QSet<int> list = {});

  void setList(const QSet<int> &list);
  void refresh();
  void setFilter(const QString &text);

 private:
  QTableView *table;
  MediaTableModel *model;
  MediaTableProxyModel *proxy_model;
};
}  // namespace Views

#endif  // UI_MAIN_WINDOW_VIEWS_ANIME_TABLE_H__
