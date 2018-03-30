#include "./media_table_proxy_model.h"

#include <functional>

#include "./media_table_model.h"

MediaTableProxyModel::MediaTableProxyModel(QObject *p) : QSortFilterProxyModel(p) {}

void MediaTableProxyModel::setFilter(const QString &filter) {
  m_filter = filter;
  this->invalidateFilter();
}

bool MediaTableProxyModel::filterAcceptsRow(int row, const QModelIndex &parent) const {
  if (m_filter.isEmpty()) {
    return true;
  }

  auto source = static_cast<MediaTableModel *>(sourceModel());
  QModelIndex index = source->index(row, 0, parent);

  auto media = source->media(index);

  if (media == nullptr) {
    return false;
  }

  return media->title().contains(m_filter, Qt::CaseInsensitive);
}

bool MediaTableProxyModel::lessThan(const QModelIndex &l, const QModelIndex &r) const {
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
  } else if (column == ListRoles::Episodes) {
    return leftMedia->episodes() < rightMedia->episodes();
  } else if (column == ListRoles::Status) {
    return leftMedia->listStatus() < rightMedia->listStatus();
  } else if (column == ListRoles::AiringStatus) {
    return leftMedia->airingStatus() < rightMedia->airingStatus();
  }

  return leftMedia->title() < rightMedia->title();
}
