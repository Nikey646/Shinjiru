#include "./nekomimi.h"

#include <QNetworkReply>
#include <QXmlStreamReader>

#include <anitomy/anitomy.h>

Nekomimi::Nekomimi() {
  this->nam = new QNetworkAccessManager(nullptr);
}

Nekomimi::~Nekomimi() {
  delete nam;
}

QList<RSSItem *> Nekomimi::fetch() {
  QUrl url("https://rss.urus.ai/");
  QNetworkRequest request(url);
  QNetworkReply *reply = nam->get(request);
  QEventLoop eventLoop;
  connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
  eventLoop.exec();

  return readReply(reply);
}

QList<RSSItem *> Nekomimi::readReply(QNetworkReply *reply) {
  QList<RSSItem *> results;
  reply->deleteLater();

  if (reply->error() != QNetworkReply::NoError) {
    qDebug() << reply->errorString();
    return results;
  }

  const auto xml = reply->readAll();
  QXmlStreamReader item(xml);
  Q_ASSERT(!item.hasError());

  RSSItem *currentItem{nullptr};
  QStringRef tag;
  anitomy::Anitomy titleParser;

  while (!item.atEnd()) {
    item.readNext();

    auto validItem = item.isCharacters() && !item.isWhitespace();

    if (item.isStartElement()) {
      if (item.name() == "item") {
        currentItem = new RSSItem;
      }

      tag = item.qualifiedName();
    } else if (item.isEndElement()) {
      if (item.name() == "item") {
        results.push_back(currentItem);
        currentItem = nullptr;
      }

      tag.clear();
    } else if (validItem && currentItem != nullptr) {
      auto text = item.text().toString();

      if (tag == "title") {
        std::wstring ws = text.replace("<!CDATA[", "").replace("]]>", "").toStdWString();
        titleParser.Parse(ws);

        const auto &elements = titleParser.elements();

        const auto &mediaTitle = elements.get(anitomy::kElementAnimeTitle);
        const auto &mediaEpisode = elements.get(anitomy::kElementEpisodeNumber);
        const auto &subGroup = elements.get(anitomy::kElementReleaseGroup);
        const auto &quality = elements.get(anitomy::kElementVideoResolution);

        currentItem->fileName = QString::fromStdWString(ws);
        currentItem->title = QString::fromStdWString(mediaTitle);
        currentItem->episode = QString::fromStdWString(mediaEpisode);
        currentItem->subGroup = QString::fromStdWString(subGroup);
        currentItem->quality = QString::fromStdWString(quality);

        if (currentItem->quality.length() == 0) {
          currentItem->quality = "720p";
        }
      } else if (tag == "link") {
        currentItem->link = text;
      } else if (tag == "nyaa:size") {
        currentItem->size = text;
      } else if (tag == "anilist:id") {
        currentItem->mediaId = text.toInt();
      }
    }
  }

  return results;
}
