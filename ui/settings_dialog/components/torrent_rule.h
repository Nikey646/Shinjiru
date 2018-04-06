#ifndef TORRENT_RULE_H
#define TORRENT_RULE_H

#include <QList>
#include <QString>

class TorrentRule {
 public:
  explicit TorrentRule(TorrentRule *parentItem = 0);
  ~TorrentRule();

  void appendChild(TorrentRule *child);
  void removeChild(TorrentRule *child);

  TorrentRule *child(int row);
  int childCount() const;
  int columnCount() const;
  QVariant data(int column) const;
  int row() const;
  TorrentRule *parentItem();
  void setParentItem(TorrentRule *parent);

 public:
  bool isTag{false};
  QString tag{""};
  int id{0};
  QString subGroup{""};
  QString quality{""};

 private:
  QList<TorrentRule *> m_childItems;
  TorrentRule *m_parentItem;
};

#endif  // TORRENT_RULE_H
