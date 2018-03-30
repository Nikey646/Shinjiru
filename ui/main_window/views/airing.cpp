#include "airing.h"
#include "ui_airing.h"

#include <QQuickView>
#include <QUrl>
#include <QWidget>

#include "../../anime_panel.h"
#include "../../src/models/media_list.h"

namespace Views {

Airing::Airing(QWidget *parent) : QWidget(parent), ui(new Ui::Airing) {
  ui->setupUi(this);

  QQuickView *view = new QQuickView();
  container = QWidget::createWindowContainer(view, this);

  context = view->rootContext();
  context->setContextProperty("airingObject", this);
  context->setContextProperty("parentWidth", this->width());
  context->setContextProperty("airingMedia", QVariant::fromValue(m_media));

  view->setResizeMode(QQuickView::SizeRootObjectToView);
  view->setSource(QUrl("qrc:/qml/AiringView.qml"));

  auto *mediaList = &MediaList::instance();

  connect(mediaList, &MediaList::mediaListChanged, this, [this, mediaList](auto list) {
    auto mediaSet = mediaList->getMediaList(list);

    for (auto id : mediaSet) {
      auto media = mediaList->getMediaById(id);

      if (media->hasNextAiringEpisode() && media->listStatus() == "CURRENT") {
        if (media->format() == "TV" || media->format() == "TV_SHORT") {
          if (!m_media.contains(media)) {
            m_media.append(media);

            context->setContextProperty("airingMedia", QVariant::fromValue(m_media));
          }
        }
      }
    }
  });

  ui->verticalLayout->addWidget(container);
}

Airing::~Airing() {
  delete ui;
}

void Airing::resizeEvent(QResizeEvent *event) {
  context->setContextProperty("parentWidth", this->width());
}

QList<QObject *> Airing::media() const {
  return this->m_media;
}

void Airing::showAnimePanel(int id) {
  auto media = MediaList::instance().getMediaById(id);
  auto panel = new AnimePanel(media);

  connect(panel, &AnimePanel::finished, [panel]() { panel->deleteLater(); });

  panel->show();
}

}  // namespace Views
