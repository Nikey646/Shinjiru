#include "torrent_rule.h"

#include <QApplication>

#include "../../../src/models/media_list.h"

TorrentRule::TorrentRule(TorrentRule *parentItem) : m_parentItem(parentItem) {}

TorrentRule::~TorrentRule() {
  qDeleteAll(m_childItems);
}

void TorrentRule::appendChild(TorrentRule *child) {
  m_childItems.append(child);
  child->setParentItem(this);
}

void TorrentRule::removeChild(TorrentRule *child) {
  m_childItems.removeAll(child);
  child->setParentItem(nullptr);

  if (m_childItems.count() == 0) {
    if (this->parentItem()) {
      this->parentItem()->removeChild(this);
    }
  }
}

TorrentRule *TorrentRule::child(int row) {
  return m_childItems.value(row);
}

int TorrentRule::childCount() const {
  return m_childItems.count();
}

int TorrentRule::columnCount() const {
  return 1;
}

QVariant TorrentRule::data(int column) const {
  if (isTag) {
    return tag;
  }

  Media *media = MediaList::instance().getMediaById(id);

  if (media) {
    return media->title();
  } else {
    return "-- " + QApplication::tr("No Anime Set") + " --";
  }
}

int TorrentRule::row() const {
  if (m_parentItem) return m_parentItem->m_childItems.indexOf(const_cast<TorrentRule *>(this));

  return 0;
}

TorrentRule *TorrentRule::parentItem() {
  return m_parentItem;
}

void TorrentRule::setParentItem(TorrentRule *parent) {
  m_parentItem = parent;
}
