#include "media.h"

#include <QJsonArray>
#include <QSet>

#include "./media_list.h"

Media::Media(QObject *parent) : QObject(parent) {}

int Media::entryId() const {
  return m_entryId;
}

void Media::setEntryId(int entryId) {
  m_entryId = entryId;
}

int Media::id() const {
  return m_id;
}

void Media::setId(int id) {
  m_id = id;
}

QString Media::title() const {
  return m_title;
}

void Media::setTitle(const QString &title) {
  m_title = title;
}

QString Media::description() const {
  return m_description;
}

void Media::setDescription(const QString &description) {
  m_description = description;
}

QString Media::format() const {
  return m_format;
}

void Media::setFormat(const QString &format) {
  m_format = format;
}

QString Media::airingStatus() const {
  return m_airingStatus;
}

void Media::setAiringStatus(const QString &airingStatus) {
  m_airingStatus = airingStatus;
}

int Media::episodes() const {
  return m_episodes;
}

void Media::setEpisodes(int episodes) {
  m_episodes = episodes;
}

int Media::duration() const {
  return m_duration;
}

void Media::setDuration(int duration) {
  m_duration = duration;
}

QString Media::coverImage() const {
  return m_coverImage;
}

void Media::setCoverImage(const QString &coverImage) {
  m_coverImage = coverImage;
}

QStringList Media::synonyms() const {
  return m_synonyms;
}

void Media::setSynonyms(const QStringList &synonyms) {
  m_synonyms = synonyms;
}

QStringList Media::genres() const {
  return m_genres;
}

void Media::setGenres(const QStringList &genres) {
  m_genres = genres;
}

QStringList Media::tags() const {
  return m_tags;
}

void Media::setTags(const QStringList &tags) {
  m_tags = tags;
}

bool Media::hasNextAiringEpisode() const {
  return m_hasNextAiringEpisode;
}

void Media::setHasNextAiringEpisode(bool hasNextAiringEpisode) {
  m_hasNextAiringEpisode = hasNextAiringEpisode;
}

QDateTime Media::airingAt() const {
  return m_airingAt;
}

void Media::setAiringAt(const QDateTime &airingAt) {
  m_airingAt = airingAt;
}

int Media::nextAiringEpisode() const {
  return m_nextAiringEpisode;
}

void Media::setNextAiringEpisode(int nextAiringEpisode) {
  m_nextAiringEpisode = nextAiringEpisode;
}

int Media::sequel() const {
  return m_sequel;
}

void Media::setSequel(int sequel) {
  m_sequel = sequel;
}

QString Media::listStatus() const {
  return m_listStatus;
}

void Media::setListStatus(const QString &listStatus) {
  m_listStatus = listStatus;
}

double Media::score() const {
  return m_score;
}

void Media::setScore(double score) {
  m_score = score;
}

int Media::progress() const {
  return m_progress;
}

void Media::setProgress(int progress) {
  m_progress = progress;
}

int Media::repeat() const {
  return m_repeat;
}

void Media::setRepeat(int repeat) {
  m_repeat = repeat;
}

int Media::priority() const {
  return m_priority;
}

void Media::setPriority(int priority) {
  m_priority = priority;
}

QString Media::notes() const {
  return m_notes;
}

void Media::setNotes(const QString &notes) {
  m_notes = notes;
}

bool Media::hiddenFromStatusLists() const {
  return m_hiddenFromStatusLists;
}

void Media::setHiddenFromStatusLists(bool hiddenFromStatusLists) {
  m_hiddenFromStatusLists = hiddenFromStatusLists;
}

QMap<QString, bool> Media::customLists() const {
  return m_customLists;
}

void Media::setCustomLists(const QMap<QString, bool> &customLists) {
  m_customLists = customLists;
}

bool Media::isPrivate() const {
  return m_private;
}

void Media::setIsPrivate(bool isPrivate) {
  m_private = isPrivate;
}

void Media::load(const QJsonObject &mediaObject) {
  if (mediaObject.contains("media")) {
    loadInnerMedia(mediaObject.value("media").toObject());
  }

  auto &mediaList = MediaList::instance();

  this->setEntryId(mediaObject.value("id").toInt());
  this->setListStatus(mediaObject.value("status").toString());
  this->setScore(mediaObject.value("score").toDouble());
  this->setProgress(mediaObject.value("progress").toInt());
  this->setRepeat(mediaObject.value("repeat").toInt());
  this->setPriority(mediaObject.value("priority").toInt());
  this->setIsPrivate(mediaObject.value("private").toBool());
  this->setNotes(mediaObject.value("notes").toString());
  this->setHiddenFromStatusLists(mediaObject.value("hiddenFromStatusLists").toBool());

  QMap<QString, bool> customLists;
  auto customListArray = mediaObject.value("customLists").toObject();

  for (auto &&customList : customListArray.keys()) {
    const auto isOnList = customListArray.value(customList).toBool();
    customLists[customList] = isOnList;

    if (isOnList) {
      mediaList.addMediaToList(customList, this);
    } else {
      mediaList.removeMediaFromList(customList, this);
    }
  }

  this->setCustomLists(customLists);

  mediaList.removeMediaFromList("CURRENT", this);
  mediaList.removeMediaFromList("PLANNING", this);
  mediaList.removeMediaFromList("COMPLETED", this);
  mediaList.removeMediaFromList("DROPPED", this);
  mediaList.removeMediaFromList("PAUSED", this);
  mediaList.removeMediaFromList("REPEATING", this);

  if (!this->hiddenFromStatusLists()) {
    mediaList.addMediaToList(this->listStatus(), this);
  }
}

void Media::loadInnerMedia(const QJsonObject &innerMedia) {
  auto titleObject = innerMedia.value("title").toObject();
  auto coverImageObject = innerMedia.value("coverImage").toObject();

  this->setId(innerMedia.value("id").toInt());
  this->setTitle(titleObject.value("userPreferred").toString());
  this->setDescription(innerMedia.value("description").toString());
  this->setFormat(innerMedia.value("format").toString());
  this->setAiringStatus(innerMedia.value("status").toString());
  this->setEpisodes(innerMedia.value("episodes").toInt());
  this->setDuration(innerMedia.value("duration").toInt());
  this->setCoverImage(coverImageObject.value("large").toString());

  QStringList synonyms;
  auto synonymArray = innerMedia.value("synonyms").toArray();

  synonyms.append(titleObject.value("romaji").toString());
  synonyms.append(titleObject.value("english").toString());
  synonyms.append(titleObject.value("native").toString());

  for (auto &&synonym : synonymArray) {
    auto strSynonym = synonym.toString();

    if (strSynonym.length() > 0) {
      synonyms.append(strSynonym);
    }
  }

  synonyms.removeAll(this->title());
  synonyms = synonyms.toSet().toList();

  this->setSynonyms(synonyms);

  QStringList genres;
  auto genreArray = innerMedia.value("genres").toArray();

  for (auto &&genre : genreArray) {
    genres.append(genre.toString());
  }

  this->setGenres(genres);

  QStringList tags;
  auto tagArray = innerMedia.value("tags").toArray();

  for (auto &&tag : tagArray) {
    tags.append(tag.toObject().value("name").toString());
  }

  this->setTags(tags);

  this->setHasNextAiringEpisode(!innerMedia.value("nextAiringEpisode").isNull());

  if (this->hasNextAiringEpisode()) {
    auto nextAiringEpisodeObject = innerMedia.value("nextAiringEpisode").toObject();
    this->setAiringAt(QDateTime::fromTime_t(nextAiringEpisodeObject.value("airingAt").toInt()));
    this->setNextAiringEpisode(nextAiringEpisodeObject.value("episode").toInt());
  }

  auto relationsObject = innerMedia.value("relations").toObject();
  auto edgesArray = relationsObject.value("edges").toArray();

  for (auto &&edge : edgesArray) {
    auto edgeObject = edge.toObject();
    auto relationType = edgeObject.value("relationType").toString();

    if (relationType == "SEQUEL") {
      auto node = edgeObject.value("node").toObject();
      auto id = node.value("id").toInt();
      this->setSequel(id);
    }
  }
}
