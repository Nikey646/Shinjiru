#ifndef SRC_DETECTION_MEDIA_STORE_H__
#define SRC_DETECTION_MEDIA_STORE_H__

#include <QHash>
#include <QObject>

#include <Robot.h>

#include "../utilities/singleton.h"

class MediaStore : public Singleton<MediaStore> {
  Q_OBJECT

 public:
  QList<Robot::Process> mediaPlayers() const;
  QList<Robot::Process> processes() const;

 public slots:
  bool hasMediaPlayer(int pid);
  void addMediaPlayer(const Robot::Process &player);

  bool hasProcess(int pid);
  void addProcess(const Robot::Process &process);

  void removeInvalid();

 signals:
  void mediaPlayersChanged();
  void processesChanged();

 private:
  QHash<int, Robot::Process> m_mediaPlayers;
  QHash<int, Robot::Process> m_processes;
};

#endif  // SRC_DETECTION_MEDIA_STORE_H__
