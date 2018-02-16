#include "./user.h"

#include "../clients/graphql_query.h"

#include <QJsonDocument>
#include <QJsonObject>

QString User::displayName() const { return this->m_displayName; }

QString User::avatar() const { return this->m_avatar; }

QString User::scoreFormat() const { return this->m_scoreFormat; }

void User::setDisplayName(const QString &displayName) {
  if (this->m_displayName != displayName) {
    this->m_displayName = displayName;
    emit displayNameChanged();
  }
}

void User::setAvatar(const QString &avatar) {
  if (this->m_avatar != avatar) {
    this->m_avatar = avatar;
    emit avatarChanged();
  }
}

void User::setScoreFormat(const QString &scoreFormat) {
  if (this->m_scoreFormat != scoreFormat) {
    this->m_scoreFormat = scoreFormat;
    emit scoreFormatChanged();
  }
}

void User::load() {
  GraphQLQuery request(":/queries/Viewer.gql");
  auto reply = request.query();

  connect(reply, &QNetworkReply::finished, [=]() {
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
      qDebug() << reply->errorString();
      return;
    }

    const auto json = reply->readAll();
    const auto document = QJsonDocument::fromJson(json);
    Q_ASSERT(document.isObject());
    const auto rootObject = document.object().value("data").toObject();
    const auto viewer = rootObject.value("Viewer").toObject();

    this->setDisplayName(viewer.value("name").toString());
    this->setAvatar(
        viewer.value("avatar").toObject().value("large").toString());
    this->setScoreFormat(viewer.value("mediaListOptions")
                             .toObject()
                             .value("scoreFormat")
                             .toString());
  });
}
