#ifndef SRC_MODELS_USER_H__
#define SRC_MODELS_USER_H__

#include <QObject>

#include "../utilities/singleton.h"

class User : public Singleton<User> {
  Q_OBJECT

  Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName NOTIFY
                 displayNameChanged)
  Q_PROPERTY(QString avatar READ avatar WRITE setAvatar NOTIFY avatarChanged)
  Q_PROPERTY(QString scoreFormat READ scoreFormat WRITE setScoreFormat NOTIFY
                 scoreFormatChanged)

 public:
  void load();

  QString displayName() const;
  QString avatar() const;
  QString scoreFormat() const;

 public slots:
  void setDisplayName(const QString &displayName);
  void setAvatar(const QString &avatar);
  void setScoreFormat(const QString &scoreFormat);

 signals:
  void displayNameChanged();
  void avatarChanged();
  void scoreFormatChanged();

 private:
  QString m_displayName{""};
  QString m_avatar{""};
  QString m_scoreFormat{""};
};

#endif  // SRC_MODELS_USER_H__
