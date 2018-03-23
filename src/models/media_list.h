#ifndef SRC_MODELS_MEDIA_LIST_H__
#define SRC_MODELS_MEDIA_LIST_H__

#include <QHash>
#include <QJsonObject>
#include <QObject>
#include <QSet>

#include "../utilities/singleton.h"
#include "./media.h"

class MediaList : public Singleton<MediaList> {
  Q_OBJECT

 public:
  MediaList();
  void load();
  void updateMedia(Media *media, const QJsonObject &data);
  void removeMedia(Media *media);

  Media *getMediaById(const int id) const;
  QSet<Media *> getMediaByTitle(const QString &title);
  QSet<int> getMediaList(const QString &key) const;
  QList<QString> getMediaLists() const;
  void addMediaToList(const QString &list, Media *media);
  void removeMediaFromList(const QString &list, Media *media);

  bool loading() const;

  void cancelUpdate();

 public slots:

 signals:
  void mediaListsChanged();
  void mediaListChanged(const QString &list);
  void mediaChanged(Media *media);
  void loadFinished();

 private:
  QHash<int, Media *> m_mediaHash;
  QHash<QString, QSet<int>> m_lists;
  bool m_listLoading{false};
  bool m_updateCancelled{false};
};

#endif  // SRC_MODELS_MEDIA_LIST_H__
