#ifndef SRC_CLIENTS_MASATO_H__
#define SRC_CLIENTS_MASATO_H__

#include <QtCore>
#include <QtNetwork>

#include <QNetworkAccessManager>

#include "../models/media.h"
#include "../models/media_list.h"
#include "../utilities/singleton.h"

class Masato : public Singleton<Masato> {
  Q_OBJECT

 public:
  Masato();
  ~Masato();

  QSet<Media *> lookup(const QString &title);

 private:
  QNetworkAccessManager *nam;

  QSet<Media *> readReply(QNetworkReply *reply);
};

#endif  // SRC_CLIENTS_MASATO_H__
