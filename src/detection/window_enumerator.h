#ifndef SRC_DETECTION_WINDOW_ENUMERATOR_H__
#define SRC_DETECTION_WINDOW_ENUMERATOR_H__

#include <iterator>

#include <QJsonObject>
#include <QMap>
#include <QString>
#include <QTimer>
#include <QString>

#include <Robot.h>
#include <anitomy/anitomy.h>

#include "../settings.h"
#include "../utilities/singleton.h"

class WindowEnumerator : public Singleton<WindowEnumerator> {
  Q_OBJECT

 public:
  WindowEnumerator();

 public slots:
  void enumerateWindows();

 private:
  bool isMediaPlayer(const Robot::Process &process);
  bool detectMedia(const Robot::Process &process);
  void processTitle(const std::string &title);
  void readMediaPlayersJson(const QString &mediaPlayersJson);

 private:
  QJsonObject m_mediaPlayers;
  QMap<QString, QString> m_exeNames;
  anitomy::Anitomy m_anitomy;
  QTimer *windowTimer;
  Settings s;
};

#endif  // SRC_DETECTION_WINDOW_ENUMERATOR_H__
