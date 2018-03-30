#ifndef SRC_CLIENTS_MASATO_H__
#define SRC_CLIENTS_MASATO_H__

#include <QDateTime>
#include <QMap>
#include <QNetworkAccessManager>
#include <QtCore>
#include <QtNetwork>

#include "../models/media.h"
#include "../models/media_list.h"
#include "../utilities/singleton.h"

class Masato : public Singleton<Masato> {
  Q_OBJECT

  using CachedResult = std::pair<QSet<Media *>, QDateTime>;

 public:
  Masato();
  ~Masato();

  QSet<Media *> lookup(const QString &title);

 private:
  QNetworkAccessManager *nam;
  QMap<QString, CachedResult> cache;

  QSet<Media *> readReply(QNetworkReply *reply);
};

#endif  // SRC_CLIENTS_MASATO_H__
