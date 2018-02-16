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

 public slots:
  void grant();

 signals:
  void authenticated();

 private:
  QOAuth2AuthorizationCodeFlow oauth2;
  QOAuthHttpServerReplyHandler *replyHandler{nullptr};
};

#endif  // SRC_CLIENTS_ANILIST_H__
