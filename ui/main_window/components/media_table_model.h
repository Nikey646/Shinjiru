#ifndef UI_MAIN_WINDOW_COMPONENTS_MEDIA_TABLE_MODEL_H__
#define UI_MAIN_WINDOW_COMPONENTS_MEDIA_TABLE_MODEL_H__

#include <QAbstractTableModel>
#include <QList>
#include <QMap>

#include "../../../src/models/media.h"
#include "../../../src/models/media_list.h"
#include "../../../src/models/user.h"

#include <better-enums/enum.h>

// clang-format off
BETTER_ENUM(
  ListRoles,
  int,
  ID,
  Title,
  Progress,
  Episodes,
  Score,
  Status,
  AiringStatus
)
// clang-format on

class MediaTableModel : public QAbstractTableModel {
  Q_OBJECT

 public:
  explicit MediaTableModel(QObject *parent = 0);

  void setList(const QSet<int> &list);
  void refresh();

  QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole);
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  Media *media(const QModelIndex &index) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;

  bool defaultHidden(int section) const;

 signals:
  void mediaChanged(Media *);

 private:
  QSet<int> m_list;
};

#endif  // UI_MAIN_WINDOW_COMPONENTS_MEDIA_TABLE_MODEL_H__
