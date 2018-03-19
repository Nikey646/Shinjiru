#ifndef SRC_CLIENTS_NEKOMIMI_H__
#define SRC_CLIENTS_NEKOMIMI_H__

#include <QtCore>
#include <QtNetwork>

#include <QNetworkAccessManager>

#include "../models/rss_item.h"
#include "../utilities/singleton.h"

class Nekomimi : public Singleton<Nekomimi> {
  Q_OBJECT

 public:
  Nekomimi();
  ~Nekomimi();

  QSet<RSSItem *> fetch();

 private:
  QNetworkAccessManager *nam;

  QSet<RSSItem *> readReply(QNetworkReply *reply);
};

#endif  // SRC_CLIENTS_NEKOMIMI_H__
