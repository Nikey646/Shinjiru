#ifndef SRC_DETECTION_MEDIA_STORE_H__
#define SRC_DETECTION_MEDIA_STORE_H__

#include <QHash>
#include <QObject>

#include <Robot.h>

#include <string>

#include "../models/media.h"
#include "../utilities/singleton.h"

class MediaStore : public Singleton<MediaStore> {
  Q_OBJECT

 public:
  MediaStore();

  QList<Robot::Process> mediaPlayers() const;
  QList<Robot::Process> processes() const;
  Media *mediaPlaying() const;
  int episodePlaying() const;

  bool hasMediaPlayer(int pid) const;
  void addMediaPlayer(const Robot::Process &player);

  bool hasProcess(int pid) const;
  void addProcess(const Robot::Process &process);

  void setMediaPlaying(Media *media, int episode);
  void setCurrentTitle(const std::wstring &title);

  void removeInvalid();
  void blackListCurrent();
  bool isBlackListed(std::wstring title, int id);

 signals:
  void mediaPlayersChanged();
  void processesChanged();
  void mediaPlayingChanged();

 private:
  QHash<int, Robot::Process> m_mediaPlayers;
  QHash<int, Robot::Process> m_processes;
  Media *m_mediaPlaying;
  int m_episode;
  std::wstring m_currentTitle;
  QMap<QString, QVariant> m_blackList;
};

#endif  // SRC_DETECTION_MEDIA_STORE_H__
