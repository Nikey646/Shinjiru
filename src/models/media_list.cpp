#include "./media_list.h"

#include "../clients/anilist.h"
#include "../clients/graphql_query.h"
#include "../clients/masato.h"
#include "../settings.h"
#include "./user.h"

#include <string>
#include <vector>

#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>

MediaList::MediaList() {}

void MediaList::load() {
  GraphQLQuery request(":/queries/MediaList.gql");
  request.set("userId", AniList::instance().userId());

  auto reply = request.query();

  m_listLoading = true;

  connect(reply, &QNetworkReply::finished, this, [reply, this]() {
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
      qDebug() << reply->errorString();
      return;
    }

    const auto json = reply->readAll();
    const auto document = QJsonDocument::fromJson(json);
    Q_ASSERT(document.isObject());
    const auto rootObject = document.object().value("data").toObject();
    const auto collection = rootObject.value("MediaListCollection").toObject();
    const auto lists = collection.value("lists").toArray();

    for (auto &&list : lists) {
      auto entries = list.toObject().value("entries").toArray();

      for (auto &&entry : entries) {
        auto mediaObject = entry.toObject();
        auto id = mediaObject.value("media").toObject().value("id").toInt();

        auto media = getMediaById(id);

        if (media == nullptr) {
          media = new Media(this);
        }

        media->load(mediaObject);
        QApplication::processEvents();
      }
    }

    emit loadFinished();
    m_listLoading = false;
  });
}

void MediaList::updateMedia(Media *media, const QJsonObject &data) {
  GraphQLQuery request(":/queries/SaveMediaListEntry.gql");
  request.set("mediaId", media->id());

  for (auto &&key : data.keys()) {
    request.set(key, data.value(key));
  }

  auto reply = request.query();

  connect(reply, &QNetworkReply::finished, this, [media, reply, this]() {
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
      qDebug() << reply->errorString();
      return;
    }

    const auto json = reply->readAll();
    const auto document = QJsonDocument::fromJson(json);
    Q_ASSERT(document.isObject());
    const auto rootObject = document.object().value("data").toObject();
    const auto mediaObject = rootObject.value("SaveMediaListEntry").toObject();

    media->load(mediaObject);
    emit loadFinished();
  });
}

void MediaList::removeMedia(Media *media) {
  GraphQLQuery request(":/queries/DeleteMediaListEntry.gql");
  request.set("id", media->entryId());

  auto reply = request.query();

  connect(reply, &QNetworkReply::finished, this, [reply, this, media]() {
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
      qDebug() << reply->errorString();
      return;
    }

    const auto json = reply->readAll();
    const auto document = QJsonDocument::fromJson(json);
    Q_ASSERT(document.isObject());
    const auto rootObject = document.object().value("data").toObject();
    const auto object = rootObject.value("DeleteMediaListEntry").toObject();

    if (object.value("delete").toBool()) {
      std::for_each(m_lists.keyBegin(), m_lists.keyEnd(),
                    [media, this](auto list) { this->removeMediaFromList(list, media); });

      delete media;
      emit loadFinished();
    }
  });
}

Media *MediaList::getMediaById(const int id) const {
  return m_mediaHash.value(id, nullptr);
}

QSet<Media *> MediaList::getMediaByTitle(const QString &title) {
  QSet<Media *> results;

  for (auto &&media : m_mediaHash) {
    if (media->title().compare(title, Qt::CaseInsensitive) == 0 ||
        media->synonyms().contains(title, Qt::CaseInsensitive)) {
      results.insert(media);
    }
  }

  Settings s;
  auto useMasato = s.get(Setting::UseMasato).toBool();

  if (results.empty() && useMasato) {
    results = Masato::instance().lookup(title);
  }

  return results;
}

QSet<int> MediaList::getMediaList(const QString &key) const {
  return m_lists[key];
}

QList<QString> MediaList::getMediaLists() const {
  return m_lists.keys();
}

void MediaList::addMediaToList(const QString &list, Media *media) {
  auto id = media->id();

  if (getMediaById(id) == nullptr) {
    m_mediaHash.insert(id, media);
  }

  if (!m_lists.contains(list)) {
    m_lists.insert(list, {});
    emit mediaListsChanged();
  }

  if (!m_lists[list].contains(id)) {
    m_lists[list].insert(id);
    emit mediaListChanged(list);
  }
}

void MediaList::removeMediaFromList(const QString &list, Media *media) {
  auto id = media->id();

  if (m_lists.contains(list)) {
    if (m_lists[list].remove(id)) {
      emit mediaListChanged(list);
    }
  }
}

QHash<int, Media *> MediaList::allMedia() const {
  return m_mediaHash;
}

bool MediaList::loading() const {
  return m_listLoading;
}

void MediaList::cancelUpdate() {
  this->m_updateCancelled = true;
}

bool MediaList::updateCancelled() const {
  return this->m_updateCancelled;
}

void MediaList::resetCancel() {
  this->m_updateCancelled = false;
}
