#include "./masato.h"

#include <QNetworkReply>

Masato::Masato() {
  this->nam = new QNetworkAccessManager(nullptr);
}

Masato::~Masato() {
  delete nam;
}

QSet<Media *> Masato::lookup(const QString &title) {
  if (cache.contains(title)) {
    auto result = cache.value(title);

    if (result.second > QDateTime::currentDateTime()) {
      return result.first;
    }
  }

  QUrl url("https://masato.urus.ai/search/" + title);
  QNetworkRequest request(url);
  QNetworkReply *reply = nam->get(request);
  QEventLoop eventLoop;
  connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
  eventLoop.exec();

  auto result = readReply(reply);

  cache.insert(title, std::make_pair(result, QDateTime::currentDateTime().addSecs(60 * 60)));

  return result;
}

QSet<Media *> Masato::readReply(QNetworkReply *reply) {
  QSet<Media *> results;
  reply->deleteLater();

  if (reply->error() != QNetworkReply::NoError) {
    qDebug() << reply->errorString();
    return results;
  }

  const auto json = reply->readAll();
  const auto document = QJsonDocument::fromJson(json);
  Q_ASSERT(document.isArray());
  const auto array = document.array();

  auto &mediaList = MediaList::instance();

  for (auto &&result : array) {
    auto object = result.toObject();
    const auto id = object.value("id").toInt();

    const auto media = mediaList.getMediaById(id);

    if (media != nullptr) {
      results.insert(media);
    }
  }

  return results;
}
