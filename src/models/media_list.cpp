#include "./media_list.h"

#include "../clients/anilist.h"
#include "../clients/graphql_query.h"
#include "../clients/masato.h"
#include "../detection/media_store.h"
#include "../settings.h"

#include <chrono>
#include <string>
#include <vector>

#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>

MediaList::MediaList() {
  auto &mediaStore = MediaStore::instance();
  connect(&mediaStore, &MediaStore::mediaPlayingChanged, [this, &mediaStore]() {
    auto media = mediaStore.mediaPlaying();

    if (media != nullptr) {
      auto episodePlaying = mediaStore.episodePlaying();

      if (episodePlaying > media->progress()) {
        QTimer *updateTimer = new QTimer;
        this->m_updateCancelled = false;
        connect(updateTimer, &QTimer::timeout,
                [this, updateTimer, media, episodePlaying, &mediaStore]() {
                  if (!this->m_updateCancelled &&
                      episodePlaying == mediaStore.episodePlaying() &&
                      media == mediaStore.mediaPlaying()) {
                    QJsonObject data;
                    data["progress"] = episodePlaying;

                    this->updateMedia(media, data);
                  }
                  updateTimer->deleteLater();
                });
        updateTimer->setSingleShot(true);
        using namespace std::literals::chrono_literals;
        updateTimer->start(std::chrono::milliseconds(2min));
      }
    }
  });
}

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

void MediaList::updateMedia(Media *media, const QJsonObject &data) {
  GraphQLQuery request(":/queries/SaveMediaListEntry.gql");
  request.set("mediaId", media->id());

  for (auto &&key : data.keys()) {
    request.set(key, data.value(key));
  }

  auto reply = request.query();

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
    const auto mediaObject = rootObject.value("SaveMediaListEntry").toObject();

    media->load(mediaObject);
  });
}

Media *MediaList::getMediaById(const int id) const {
  return m_mediaHash.value(id, nullptr);
}

QSet<Media *> MediaList::getMediaByTitle(const QString &title) {
  QSet<Media *> results;

  for (auto &&media : m_mediaHash) {
    if (media->title() == title || media->synonyms().contains(title)) {
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

void MediaList::cancelUpdate() { this->m_updateCancelled = true; }
