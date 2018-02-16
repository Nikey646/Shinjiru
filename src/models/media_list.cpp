#include "./media_list.h"

#include "../clients/anilist.h"
#include "../clients/graphql_query.h"

#include <string>
#include <vector>

#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>

void MediaList::load() {
  GraphQLQuery request(":/queries/MediaList.gql");
  request.set("userId", AniList::instance().userId());

  auto reply = request.query();

  m_listLoading = true;

  connect(reply, &QNetworkReply::finished, [=]() {
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
      qDebug() << reply->errorString();
      return;
    }

    const auto json = reply->readAll();
    const auto document = QJsonDocument::fromJson(json);
    Q_ASSERT(document.isObject());
    const auto rootObject = document.object().value("data").toObject();
    const auto mediaListCollection =
        rootObject.value("MediaListCollection").toObject();

    for (auto &&type : mediaListCollection.keys()) {
      auto lists = mediaListCollection.value(type).toObject();
      for (auto &&list_key : lists.keys()) {
        auto list = lists.value(list_key).toArray();

        for (auto &&entry : list) {
          auto mediaObject = entry.toObject();
          auto id = mediaObject.value("media").toObject().value("id").toInt();

          auto media = getMediaById(id);

          if (media == nullptr) {
            media = new Media(this);
          }

          media->load(mediaObject);

          addMediaToList(list_key, media);
          QApplication::processEvents();
        }
      }
    }

    emit loadFinished();
    m_listLoading = false;
  });
}

Media *MediaList::getMediaById(const int id) const {
  return m_mediaHash.value(id, nullptr);
}

QSet<Media *> MediaList::getMediaByTitle(const QString &title) {
  QSet<Media *> results;

  Q_UNUSED(title)
  // TODO

  return results;
}

QSet<int> MediaList::getMediaList(const QString &key) const {
  return m_lists[key];
}

QList<QString> MediaList::getMediaLists() const { return m_lists.keys(); }

void MediaList::addMediaToList(const QString &list, Media *media) {
  auto id = media->id();

  if (getMediaById(id) == nullptr) {
    m_mediaHash.insert(id, media);
  }

  if (!m_lists.contains(list)) {
    m_lists.insert(list, {});
    emit mediaListsChanged();
  }

  m_lists[list].insert(id);
  emit mediaListChanged(list);
}

bool MediaList::loading() const { return m_listLoading; }
