#ifndef SRC_CLIENTS_ANILIST_H__
#define SRC_CLIENTS_ANILIST_H__

#include <QtCore>
#include <QtNetwork>

#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>

#include "../utilities/singleton.h"

class AniList : public Singleton<AniList> {
  Q_OBJECT

 public:
  AniList();
  ~AniList();

  QNetworkReply *graphql(const QString &query);
  QNetworkReply *graphql(const QString &query, const QJsonObject &variables);

  int userId();

  void requestReload();

 public slots:
  void grant();
  void statusChanged(QAbstractOAuth::Status status);

 signals:
  void authenticated();
  void reload();

 private:
  QOAuth2AuthorizationCodeFlow oauth2;
  QOAuthHttpServerReplyHandler *replyHandler{nullptr};
};

#endif  // SRC_CLIENTS_ANILIST_H__
