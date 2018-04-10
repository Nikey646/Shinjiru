#ifndef SRC_MODELS_MEDIA_H__
#define SRC_MODELS_MEDIA_H__

#include <QDateTime>
#include <QJsonObject>
#include <QMap>
#include <QObject>
#include <QString>

class Media : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString coverImage READ coverImage WRITE setCoverImage NOTIFY coverImageChanged)
  Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
  Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)

 public:
  explicit Media(QObject *parent = nullptr);

  int entryId() const;
  void setEntryId(int entryId);

  int id() const;
  void setId(int id);

  QString title() const;
  void setTitle(const QString &title);

  QString description() const;
  void setDescription(const QString &description);

  QString format() const;
  void setFormat(const QString &format);

  QString airingStatus() const;
  void setAiringStatus(const QString &airingStatus);

  int episodes() const;
  void setEpisodes(int episodes);

  int duration() const;
  void setDuration(int duration);

  QString coverImage() const;
  void setCoverImage(const QString &coverImage);

  QStringList synonyms() const;
  void setSynonyms(const QStringList &synonyms);

  QStringList genres() const;
  void setGenres(const QStringList &genres);

  QStringList tags() const;
  void setTags(const QStringList &tags);

  QStringList studios() const;
  void setStudios(const QStringList &studios);

  bool hasNextAiringEpisode() const;
  void setHasNextAiringEpisode(bool hasNextAiringEpisode);

  QDateTime airingAt() const;
  void setAiringAt(const QDateTime &airingAt);

  int nextAiringEpisode() const;
  void setNextAiringEpisode(int nextAiringEpisode);

  int sequel() const;
  void setSequel(int sequel);

  QString listStatus() const;
  void setListStatus(const QString &listStatus);

  double score() const;
  void setScore(double score);

  int progress() const;
  void setProgress(int progress);

  int repeat() const;
  void setRepeat(int repeat);

  int priority() const;
  void setPriority(int priority);

  QString notes() const;
  void setNotes(const QString &notes);

  bool hiddenFromStatusLists() const;
  void setHiddenFromStatusLists(bool hiddenFromStatusLists);

  QMap<QString, bool> customLists() const;
  void setCustomLists(const QMap<QString, bool> &customLists);

  bool isPrivate() const;
  void setIsPrivate(bool isPrivate);

  void load(const QJsonObject &mediaObject);
  void loadInnerMedia(const QJsonObject &innerMedia);

 signals:
  void coverImageChanged();
  void titleChanged();
  void idChanged();

 private:
  int m_entryId{0};
  int m_id{0};
  QString m_title{""};
  QString m_description{""};
  QString m_format{""};
  QString m_airingStatus{""};
  int m_episodes{0};
  int m_duration{0};
  QString m_coverImage{""};
  QStringList m_synonyms{};
  QStringList m_genres{};
  QStringList m_tags{};
  QStringList m_studios{};
  bool m_hasNextAiringEpisode{false};
  QDateTime m_airingAt{};
  int m_nextAiringEpisode{0};
  int m_sequel{0};

  QString m_listStatus{""};
  double m_score{0.0};
  int m_progress{0};
  int m_repeat{0};
  int m_priority{0};
  bool m_private{false};
  QString m_notes{""};
  bool m_hiddenFromStatusLists{false};
  QMap<QString, bool> m_customLists{};
};

#endif  // SRC_MODELS_MEDIA_H__
