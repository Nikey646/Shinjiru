#include "./anilist.h"

#include <QDesktopServices>
#include <QSettings>

const QUrl graphqlUrl("https://graphql.anilist.co");

AniList::AniList() {
  oauth2.setAuthorizationUrl(QUrl("https://anilist.co/api/v2/oauth/authorize"));
  oauth2.setAccessTokenUrl(QUrl("https://auth.shinjiru.me/v2.php"));
  oauth2.setClientIdentifier("16");

  connect(&oauth2, &QOAuth2AuthorizationCodeFlow::statusChanged, this,
          &AniList::statusChanged);

  connect(&oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
          &QDesktopServices::openUrl);
}

AniList::~AniList() {
  if (replyHandler != nullptr) {
    delete replyHandler;
    replyHandler = nullptr;
  }
}

QNetworkReply *AniList::graphql(const QString &query,
                                const QJsonObject &variables) {
  return oauth2.post(
      graphqlUrl,
      {{"query", query}, {"variables", QJsonDocument(variables).toJson()}});
}

QNetworkReply *AniList::graphql(const QString &query) {
  return oauth2.post(graphqlUrl, {{"query", query}});
}

int AniList::userId() {
  auto jwt = oauth2.token().split(".");
  auto payload = QByteArray::fromBase64(jwt[1].toLocal8Bit());

  auto object = QJsonDocument::fromJson(payload).object();

  return object["sub"].toString().toInt();
}

void AniList::requestReload() { emit reload(); }

void AniList::grant() {
  if (replyHandler == nullptr) {
    replyHandler = new QOAuthHttpServerReplyHandler(42069, this);
    replyHandler->setCallbackPath("auth");

    oauth2.setReplyHandler(replyHandler);
  }

  QSettings settings;
  QString accessToken = settings.value("accessToken", "").toString();

  if (accessToken.size() != 0) {
    oauth2.setToken(accessToken);
    emit authenticated();
  } else {
    oauth2.grant();
  }
}

void AniList::statusChanged(QAbstractOAuth::Status status) {
  if (status == QAbstractOAuth::Status::Granted) {
    QSettings settings;
    settings.setValue("accessToken", oauth2.token());

    emit authenticated();
  }
}
