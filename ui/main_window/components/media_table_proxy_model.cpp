#include "./media_table_proxy_model.h"

#include <functional>

#include "./media_table_model.h"

MediaTableProxyModel::MediaTableProxyModel(QObject *p)
    : QSortFilterProxyModel(p) {}

void MediaTableProxyModel::setFilter(const QString &filter) {
  m_filter = filter;
  emit filterChanged();
}

bool MediaTableProxyModel::filterAcceptsRow(int row,
                                            const QModelIndex &parent) const {
  if (m_filter.isEmpty()) {
    return true;
  }

  Q_UNUSED(parent)

  auto source = static_cast<MediaTableModel *>(sourceModel());
  QModelIndex index = createIndex(row, 0);

  auto media = source->media(index);

  if (media == nullptr) {
    return false;
  }

  return media->title().contains(m_filter, Qt::CaseInsensitive);
}

bool MediaTableProxyModel::lessThan(const QModelIndex &l,
                                    const QModelIndex &r) const {
  auto source = static_cast<MediaTableModel *>(sourceModel());
  auto column = l.column();

  auto leftMedia = source->media(l);
  auto rightMedia = source->media(r);

  if (column == ListRoles::Score) {
    return leftMedia->score() < rightMedia->score();
  } else if (column == ListRoles::ID) {
    return leftMedia->id() < rightMedia->id();
  } else if (column == ListRoles::Progress) {
    return leftMedia->progress() < rightMedia->progress();
  }

  return leftMedia->title() < rightMedia->title();
}
