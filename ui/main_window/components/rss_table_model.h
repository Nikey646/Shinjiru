#ifndef UI_MAIN_WINDOW_COMPONENTS_RSS_TABLE_MODEL_H__
#define UI_MAIN_WINDOW_COMPONENTS_RSS_TABLE_MODEL_H__

#include <QAbstractTableModel>
#include <QList>
#include <QMap>

#include "../../../src/models/rss_item.h"

#include <better-enums/enum.h>

// clang-format off
BETTER_ENUM(
  RSSRoles,
  int,
  Title,
  Episode,
  Quality,
  SubGroup,
  Size,
  Link
)
// clang-format on

class RSSTableModel : public QAbstractTableModel {
  Q_OBJECT

 public:
  explicit RSSTableModel(QObject *parent = 0);

  void setList(const QList<RSSItem *> &list);
  void refresh();

  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

 private:
  QList<RSSItem *> m_list;
};

#endif  // UI_MAIN_WINDOW_COMPONENTS_RSS_TABLE_MODEL_H__
