#include "./rss_table_model.h"

RSSTableModel::RSSTableModel(QObject *parent) : QAbstractTableModel(parent) {}

void RSSTableModel::setList(const QList<RSSItem *> &list) {
  m_list = list;
  refresh();
}

void RSSTableModel::refresh() {
  emit layoutAboutToBeChanged();
  emit layoutChanged();
}

int RSSTableModel::rowCount(const QModelIndex &p) const {
  Q_UNUSED(p);
  return m_list.count();
}

int RSSTableModel::columnCount(const QModelIndex &p) const {
  Q_UNUSED(p);
  return static_cast<int>(RSSRoles::_size());
}

QVariant RSSTableModel::headerData(int s, Qt::Orientation o, int r) const {
  if (r == Qt::DisplayRole && o == Qt::Horizontal) {
    const RSSRoles role = RSSRoles::_from_integral(s);
    return tr(role._to_string());
  }

  return QVariant();
}

QVariant RSSTableModel::data(const QModelIndex &index, int role) const {
  if (index.row() < 0 || index.row() >= m_list.count() || role != Qt::DisplayRole) {
    return QVariant();
  }

  RSSItem *item = *(m_list.begin() + index.row());

  switch (index.column()) {
    case RSSRoles::Title:
      return item->title;
      break;
    case RSSRoles::Episode:
      return item->episode;
      break;
    case RSSRoles::SubGroup:
      return item->subGroup;
      break;
    case RSSRoles::Quality:
      return item->quality;
      break;
    case RSSRoles::Size:
      return item->size;
      break;
    case RSSRoles::Link:
      return item->link;
      break;
  }

  return "";
}
